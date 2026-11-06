/**
 * @file board.h
 *
 * LCDWIKI ES3C35P: ESP32-S3-N16R8, 3.5" ST77922 IPS (320x480, QSPI) with the
 * ST77922's built-in capacitive touch on I2C. Pins from the board's IO table and
 * the XiaoZhi board definition (main/boards/lcdwiki-es3c35p).
 */
#ifndef BOARD_H
#define BOARD_H

#include "driver/i2c_master.h"
#include "esp_err.h"
#include "lvgl.h"

/* Display, QSPI on SPI2 */
#define BOARD_LCD_CS            10
#define BOARD_LCD_CLK           12
#define BOARD_LCD_D0            11
#define BOARD_LCD_D1            13
#define BOARD_LCD_D2            14
#define BOARD_LCD_D3            9
#define BOARD_LCD_RST           (-1)    /* tied to EN */
#define BOARD_LCD_BACKLIGHT     41      /* high = on */
#define BOARD_LCD_PCLK_HZ       (80 * 1000 * 1000)

/* Physical panel size: portrait */
#define BOARD_LCD_H_RES         320
#define BOARD_LCD_V_RES         480

/* The ST77922 cannot swap X/Y in hardware, so landscape is rotated by LVGL in
 * software. If the picture is upside down, swap between _90 and _270. */
#define BOARD_ROTATION          LV_DISPLAY_ROTATION_270

/* If colors look inverted (black background shows white), flip this */
#define BOARD_LCD_INVERT_COLOR  true

/* Shared I2C: touch (0x55), ES8311 codec (0x18), and the "I2C" connector (MPU-6050 at 0x68) */
#define BOARD_I2C_SDA           38
#define BOARD_I2C_SCL           39
#define BOARD_TOUCH_ADDR        0x55
#define BOARD_TOUCH_RST         48
#define BOARD_TOUCH_INT         47
#define BOARD_TOUCH_DEBUG       0       /* 1: log every touch press/release with its screen position */

/**
 * Start the I2C bus, the display, the touch and the LVGL task.
 * Afterwards create the UI only while holding lv_lock().
 * @param i2c_bus  returns the shared I2C bus, for the MPU-6050
 */
esp_err_t board_init(i2c_master_bus_handle_t * i2c_bus);

/** Turn the backlight on (it starts off, so the boot noise isn't visible) */
void board_backlight_on(void);

#endif /* BOARD_H */
