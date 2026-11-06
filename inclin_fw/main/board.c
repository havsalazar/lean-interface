/**
 * @file board.c
 *
 * ES3C35P bring-up: I2C, ST77922 QSPI panel, ST77922 touch, LVGL 9 port.
 *
 * Panel init sequence, color order and touch protocol are taken from the
 * XiaoZhi ESP32 project's board definition for this exact module
 * (github.com/78/xiaozhi-esp32, main/boards/lcdwiki-es3c35p), which runs on it.
 */
#include "board.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_check.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_st77922.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char * TAG = "board";

/* Render in strips of this many lines (logical, landscape: 480 px wide).
 * A multiple of 4 because the ST77922 needs 4-pixel aligned columns. */
#define DRAW_BUF_LINES      40
#define DRAW_BUF_PX         (BOARD_LCD_V_RES * DRAW_BUF_LINES)
#define DRAW_BUF_BYTES      (DRAW_BUF_PX * sizeof(uint16_t))

static esp_lcd_panel_handle_t s_panel;
static lv_display_t * s_disp;
static void * s_rot_buf;            /* the rotated copy of a strip, sent by DMA */
static i2c_master_dev_handle_t s_touch;

/**********************
 *  PANEL INIT SEQUENCE
 **********************/
static const st77922_lcd_init_cmd_t lcd_init_cmds[] = {
    {0xF1, (uint8_t[]){0x00}, 1, 0},
    {0x60, (uint8_t[]){0x00, 0x00, 0x00}, 3, 0},
    {0x65, (uint8_t[]){0x80}, 1, 0},
    {0x79, (uint8_t[]){0x06}, 1, 0},
    {0x7B, (uint8_t[]){0x00, 0x08, 0x08}, 3, 0},
    {0x80, (uint8_t[]){0x55, 0x62, 0x2F, 0x17, 0xF0, 0x52, 0x70, 0xD2, 0x52, 0x62, 0xEA}, 11, 0},
    {0x81, (uint8_t[]){0x26, 0x52, 0x72, 0x27}, 4, 0},
    {0x84, (uint8_t[]){0x92, 0x25}, 2, 0},
    {0x87, (uint8_t[]){0x10, 0x10, 0x58, 0x00, 0x02, 0x3A}, 6, 0},
    {0x88, (uint8_t[]){0x00, 0x00, 0x2C, 0x10, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x06}, 15, 0},
    {0x89, (uint8_t[]){0x00, 0x00, 0x00}, 3, 0},
    {0x8A, (uint8_t[]){0x13, 0x00, 0x2C, 0x00, 0x00, 0x2C, 0x10, 0x10, 0x00, 0x3E, 0x19}, 11, 0},
    {0x8B, (uint8_t[]){0x15, 0xB1, 0xB1, 0x44, 0x96, 0x2C, 0x10, 0x97, 0x8E}, 9, 0},
    {0x8C, (uint8_t[]){0x1D, 0xB1, 0xB1, 0x44, 0x96, 0x2C, 0x10, 0x50, 0x0F, 0x01, 0xC5, 0x12, 0x09}, 13, 0},
    {0x8D, (uint8_t[]){0x0C}, 1, 0},
    {0x8E, (uint8_t[]){0x33, 0x01, 0x0C, 0x13, 0x01, 0x01}, 6, 0},
    {0xB3, (uint8_t[]){0x00, 0x30}, 2, 0},
    {0xF1, (uint8_t[]){0x00}, 1, 0},
    {0x71, (uint8_t[]){0xD0}, 1, 0},
    {0x66, (uint8_t[]){0x02, 0x3F}, 2, 0},
    {0xBE, (uint8_t[]){0x26, 0x00, 0x9D}, 3, 0},
    {0x70, (uint8_t[]){0x01, 0xA0, 0x11, 0x40, 0xE0, 0x00, 0x11, 0x69, 0x11, 0x00, 0x00, 0x1A}, 12, 0},
    {0x90, (uint8_t[]){0x04, 0x04, 0x55, 0x74, 0x00, 0x40, 0x43, 0x27, 0x27}, 9, 0},
    {0x91, (uint8_t[]){0x04, 0x04, 0x55, 0x75, 0x00, 0x40, 0x42, 0x27, 0x27}, 9, 0},
    {0x92, (uint8_t[]){0x04, 0x44, 0x55, 0xC0, 0x06, 0x00, 0x07, 0x05, 0x90, 0x27}, 10, 0},
    {0x93, (uint8_t[]){0x04, 0x43, 0x11, 0x00, 0x00, 0x00, 0x00, 0x05, 0x90, 0x27}, 10, 0},
    {0x94, (uint8_t[]){0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 6, 0},
    {0x95, (uint8_t[]){0x96, 0x16, 0x00, 0x00, 0xFF}, 5, 0},
    {0x96, (uint8_t[]){0x44, 0x53, 0x03, 0x12, 0x23, 0x24, 0x06, 0x05, 0x94, 0x27, 0x00, 0x44}, 12, 0},
    {0x97, (uint8_t[]){0x44, 0x53, 0x47, 0x56, 0x20, 0x20, 0x02, 0x01, 0x94, 0x27, 0x00, 0x44}, 12, 0},
    {0xBA, (uint8_t[]){0x55, 0x94, 0x2D, 0x94, 0x27}, 5, 0},
    {0x9A, (uint8_t[]){0x40, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00}, 7, 0},
    {0x9B, (uint8_t[]){0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00}, 7, 0},
    {0x9C, (uint8_t[]){0x5C, 0x12, 0x00, 0x00, 0x10, 0x12, 0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0x00}, 13, 0},
    {0x9D, (uint8_t[]){0x8A, 0x51, 0x00, 0x00, 0x00, 0x80, 0x1E, 0x01}, 8, 0},
    {0x9E, (uint8_t[]){0x51, 0x00, 0x00, 0x00, 0x80, 0x1E, 0x01}, 7, 0},
    {0xB4, (uint8_t[]){0x1D, 0x1C, 0x1E, 0x0B, 0x14, 0x02, 0x13, 0x09, 0x1E, 0x00, 0x1E, 0x10}, 12, 0},
    {0xB5, (uint8_t[]){0x1D, 0x1C, 0x1E, 0x0A, 0x15, 0x03, 0x11, 0x08, 0x1E, 0x01, 0x1E, 0x12}, 12, 0},
    {0xB6, (uint8_t[]){0x77, 0x77, 0x00, 0x0A, 0xFF, 0x0A, 0xFF}, 7, 0},
    {0x86, (uint8_t[]){0xCD, 0x04, 0xB1, 0x02, 0x58, 0x12, 0x58, 0x0C, 0x13, 0x01, 0xA5, 0x00, 0xA5, 0xA5}, 14, 0},
    {0xB7, (uint8_t[]){0x07, 0x0A, 0x0E, 0x06, 0x05, 0x03, 0x2B, 0x03, 0x03, 0x42, 0x07, 0x10, 0x10, 0x2E, 0x3F, 0x0D}, 16, 0},
    {0xB8, (uint8_t[]){0x07, 0x0A, 0x0D, 0x05, 0x05, 0x02, 0x2B, 0x02, 0x03, 0x42, 0x06, 0x10, 0x0F, 0x2E, 0x3F, 0x0D}, 16, 0},
    {0xB9, (uint8_t[]){0x23, 0x23}, 2, 0},
    {0xBF, (uint8_t[]){0x10, 0x14, 0x14, 0x0B, 0x0B, 0x0B}, 6, 0},
    {0xF2, (uint8_t[]){0x00}, 1, 0},
    {0x73, (uint8_t[]){0x04, 0xDA, 0x12, 0x54, 0x47}, 5, 0},
    {0x77, (uint8_t[]){0x6B, 0x5B, 0xFD, 0xC3, 0xC5}, 5, 0},
    {0x7A, (uint8_t[]){0x15, 0x27}, 2, 0},
    {0x7B, (uint8_t[]){0x04, 0x57}, 2, 0},
    {0x7E, (uint8_t[]){0x01, 0x0E}, 2, 0},
    {0xBF, (uint8_t[]){0x36}, 1, 0},
    {0xE3, (uint8_t[]){0x40, 0x40}, 2, 0},
    {0xF0, (uint8_t[]){0x00}, 1, 0},
    {0xD0, (uint8_t[]){0x00}, 1, 0},
    {0x2A, (uint8_t[]){0x00, 0x00, 0x01, 0x3F}, 4, 0},   /* columns 0..319 */
    {0x2B, (uint8_t[]){0x00, 0x00, 0x01, 0xDF}, 4, 0},   /* rows 0..479 */
    {0x21, (uint8_t[]){0x00}, 0, 0},
    {0x11, (uint8_t[]){0x00}, 0, 120},
    {0x29, (uint8_t[]){0x00}, 0, 0},
    {0x2C, (uint8_t[]){0x00}, 0, 0},
    {0x3A, (uint8_t[]){0x01}, 1, 0},
    {0x36, (uint8_t[]){0x00}, 1, 0},
    {0x35, (uint8_t[]){0x01}, 1, 20},
};

/**********************
 *  LVGL DISPLAY
 **********************/
static bool flush_done_cb(esp_lcd_panel_io_handle_t io, esp_lcd_panel_io_event_data_t * edata, void * ctx)
{
    (void)io;
    (void)edata;
    (void)ctx;
    if(s_disp) lv_display_flush_ready(s_disp);
    return false;
}

static void flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    lv_area_t a = *area;
    uint8_t * out = px_map;
    lv_display_rotation_t rot = lv_display_get_rotation(disp);

    if(rot != LV_DISPLAY_ROTATION_0) {
        lv_color_format_t cf = lv_display_get_color_format(disp);
        int32_t w = lv_area_get_width(&a);
        int32_t h = lv_area_get_height(&a);
        uint32_t src_stride = lv_draw_buf_width_to_stride(w, cf);
        bool quarter = (rot == LV_DISPLAY_ROTATION_90 || rot == LV_DISPLAY_ROTATION_270);
        uint32_t dst_stride = lv_draw_buf_width_to_stride(quarter ? h : w, cf);
        lv_draw_sw_rotate(px_map, s_rot_buf, w, h, src_stride, dst_stride, rot, cf);
        lv_display_rotate_area(disp, &a);   /* now in panel coordinates */
        out = s_rot_buf;
    }

    /* LVGL keeps RGB565 little-endian, the panel wants the high byte first */
    lv_draw_sw_rgb565_swap(out, (uint32_t)lv_area_get_size(&a));

    esp_lcd_panel_draw_bitmap(s_panel, a.x1, a.y1, a.x2 + 1, a.y2 + 1, out);
    /* lv_display_flush_ready() is called from flush_done_cb when the DMA ends */
}

/* The ST77922 only accepts windows whose column start is a multiple of 4 and
 * whose width is a multiple of 4. After rotation the panel's columns come from
 * LVGL's rows, so round both axes (480 and 320 are multiples of 4). */
static void rounder_cb(lv_event_t * e)
{
    lv_area_t * area = lv_event_get_param(e);
    area->x1 &= ~3;
    area->x2 |= 3;
    area->y1 &= ~3;
    area->y2 |= 3;
}

static uint32_t tick_cb(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000);
}

/**********************
 *  TOUCH (ST77922 built-in, FT6336G-like register map with 16-bit addresses)
 **********************/
#define TOUCH_REG_MAX_TOUCHES   0x0009
#define TOUCH_REG_INFO          0x0010
#define TOUCH_MAX_POINTS        5

static uint8_t s_touch_points = 1;      /* points the controller reports, read at init */

static esp_err_t touch_read(uint16_t reg, uint8_t * data, size_t len)
{
    uint8_t w[2] = { (uint8_t)(reg >> 8), (uint8_t)reg };
    return i2c_master_transmit_receive(s_touch, w, sizeof(w), data, len, 50);
}

static void touch_read_cb(lv_indev_t * indev, lv_indev_data_t * data)
{
    (void)indev;
    static int32_t last_x, last_y;
    static bool was_pressed;
    data->point.x = last_x;
    data->point.y = last_y;
    data->state = LV_INDEV_STATE_RELEASED;

    /* One burst from TOUCH_INFO: 4 header bytes, then 7 bytes per point, then a
     * checksum byte. The controller only refreshes its report after the whole
     * frame is read in one go, so reading header and point separately (as
     * before) left the first touch latched forever. Same as Espressif's driver
     * for the sister chip, esp_lcd_touch_st77926. */
    uint8_t buf[4 + 7 * TOUCH_MAX_POINTS + 1] = { 0 };
    size_t len = 4 + 7 * s_touch_points + 1;
    bool ok = touch_read(TOUCH_REG_INFO, buf, len) == ESP_OK;
    uint8_t info = buf[0];
    const uint8_t * p = &buf[4];            /* first point */
    bool pressed = ok && !(info & 0x80) && (p[0] & 0x80);   /* 0x80 in info: controller wants a reset */

    if(pressed) {
        /* Panel (portrait) coordinates; LVGL applies the display rotation itself */
        last_x = ((p[0] & 0x3F) << 8) | p[1];
        last_y = ((p[2] & 0x3F) << 8) | p[3];
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_PRESSED;
    }

#if BOARD_TOUCH_DEBUG
    if(pressed != was_pressed) {
        lv_point_t ui = { last_x, last_y };
        lv_display_rotate_point(s_disp, &ui);
        ESP_LOGI(TAG, "touch %s raw=(%d,%d) ui=(%d,%d)", pressed ? "DOWN" : "UP",
                 (int)last_x, (int)last_y, (int)ui.x, (int)ui.y);
    }
    was_pressed = pressed;
#else
    (void)was_pressed;
#endif
}

static void touch_init(i2c_master_bus_handle_t bus)
{
    gpio_config_t rst = { .pin_bit_mask = 1ULL << BOARD_TOUCH_RST, .mode = GPIO_MODE_OUTPUT };
    gpio_config(&rst);
    gpio_set_level(BOARD_TOUCH_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(BOARD_TOUCH_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    i2c_device_config_t cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = BOARD_TOUCH_ADDR,
        .scl_speed_hz = 400000,
    };
    if(i2c_master_bus_add_device(bus, &cfg, &s_touch) != ESP_OK) {
        ESP_LOGW(TAG, "touch: cannot add I2C device, touch disabled");
        return;
    }

    uint8_t max_pts = 0;
    if(touch_read(TOUCH_REG_MAX_TOUCHES, &max_pts, 1) != ESP_OK) {
        ESP_LOGW(TAG, "touch: no answer at 0x%02X, touch disabled", BOARD_TOUCH_ADDR);
        return;
    }
    if(max_pts >= 1 && max_pts <= TOUCH_MAX_POINTS) s_touch_points = max_pts;
    gpio_config_t intr = { .pin_bit_mask = 1ULL << BOARD_TOUCH_INT, .mode = GPIO_MODE_INPUT,
                           .pull_up_en = GPIO_PULLUP_ENABLE };
    gpio_config(&intr);
    ESP_LOGI(TAG, "touch: ready (%d points)", max_pts);

    lv_indev_t * indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touch_read_cb);
    lv_indev_set_display(indev, s_disp);
}

/**********************
 *  LVGL TASK
 **********************/
static void lvgl_task(void * arg)
{
    (void)arg;
    while(1) {
        lv_lock();
        uint32_t idle_ms = lv_timer_handler();
        lv_unlock();
        if(idle_ms == LV_NO_TIMER_READY || idle_ms > 30) idle_ms = 30;
        if(idle_ms < 5) idle_ms = 5;
        vTaskDelay(pdMS_TO_TICKS(idle_ms));
    }
}

/**********************
 *  INIT
 **********************/
esp_err_t board_init(i2c_master_bus_handle_t * i2c_bus)
{
    /* Backlight off until the first frame is drawn */
    gpio_config_t bl = { .pin_bit_mask = 1ULL << BOARD_LCD_BACKLIGHT, .mode = GPIO_MODE_OUTPUT };
    ESP_RETURN_ON_ERROR(gpio_config(&bl), TAG, "backlight gpio");
    gpio_set_level(BOARD_LCD_BACKLIGHT, 0);

    /* Shared I2C bus */
    i2c_master_bus_config_t i2c_cfg = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = BOARD_I2C_SDA,
        .scl_io_num = BOARD_I2C_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_RETURN_ON_ERROR(i2c_new_master_bus(&i2c_cfg, i2c_bus), TAG, "i2c bus");

    /* QSPI bus and panel */
    const spi_bus_config_t bus_cfg = ST77922_PANEL_BUS_QSPI_CONFIG(BOARD_LCD_CLK, BOARD_LCD_D0, BOARD_LCD_D1,
                                                                   BOARD_LCD_D2, BOARD_LCD_D3, DRAW_BUF_BYTES);
    ESP_RETURN_ON_ERROR(spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO), TAG, "spi bus");

    esp_lcd_panel_io_handle_t io = NULL;
    esp_lcd_panel_io_spi_config_t io_cfg = ST77922_PANEL_IO_QSPI_CONFIG(BOARD_LCD_CS, flush_done_cb, NULL);
    io_cfg.pclk_hz = BOARD_LCD_PCLK_HZ;
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &io_cfg, &io), TAG, "panel io");

    const st77922_vendor_config_t vendor_cfg = {
        .init_cmds = lcd_init_cmds,
        .init_cmds_size = sizeof(lcd_init_cmds) / sizeof(lcd_init_cmds[0]),
        .flags = { .use_qspi_interface = 1 },
    };
    const esp_lcd_panel_dev_config_t panel_cfg = {
        .reset_gpio_num = BOARD_LCD_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
        .bits_per_pixel = 16,
        .vendor_config = (void *)&vendor_cfg,
    };
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_st77922(io, &panel_cfg, &s_panel), TAG, "panel");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_reset(s_panel), TAG, "panel reset");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_init(s_panel), TAG, "panel init");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_invert_color(s_panel, BOARD_LCD_INVERT_COLOR), TAG, "invert");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_disp_on_off(s_panel, true), TAG, "display on");

    /* LVGL */
    lv_init();
    lv_tick_set_cb(tick_cb);

    s_disp = lv_display_create(BOARD_LCD_H_RES, BOARD_LCD_V_RES);
    lv_display_set_color_format(s_disp, LV_COLOR_FORMAT_RGB565);
    lv_display_set_rotation(s_disp, BOARD_ROTATION);     /* 480x320 for the UI */

    void * buf1 = heap_caps_malloc(DRAW_BUF_BYTES, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    void * buf2 = heap_caps_malloc(DRAW_BUF_BYTES, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    s_rot_buf   = heap_caps_malloc(DRAW_BUF_BYTES, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    ESP_RETURN_ON_FALSE(buf1 && buf2 && s_rot_buf, ESP_ERR_NO_MEM, TAG, "draw buffers");
    lv_display_set_buffers(s_disp, buf1, buf2, DRAW_BUF_BYTES, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(s_disp, flush_cb);
    lv_display_add_event_cb(s_disp, rounder_cb, LV_EVENT_INVALIDATE_AREA, NULL);

    touch_init(*i2c_bus);

    xTaskCreatePinnedToCore(lvgl_task, "lvgl", 8192, NULL, 2, NULL, 1);
    return ESP_OK;
}

void board_backlight_on(void)
{
    gpio_set_level(BOARD_LCD_BACKLIGHT, 1);
}
