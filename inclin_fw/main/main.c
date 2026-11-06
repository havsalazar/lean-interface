/**
 * @file main.c
 *
 * Inclinometer: lean angle bars + accel/brake G-meter on the ES3C35P,
 * fed by an MPU-6050 on the board's I2C connector (IO38/IO39).
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "lvgl.h"
#include "inclin_ui.h"      /* exported by the LVGL Pro Editor from ../inclin_ui */
#include "board.h"
#include "imu_task.h"

static const char * TAG = "main";

void app_main(void)
{
    i2c_master_bus_handle_t i2c_bus;
    ESP_ERROR_CHECK(board_init(&i2c_bus));

    lv_lock();
    inclin_ui_init("");                     /* no file-based assets, everything is compiled in */
    lv_screen_load(lean_screen_create());
    imu_ui_bind();                          /* copies IMU values into the UI subjects */
    lv_unlock();

    vTaskDelay(pdMS_TO_TICKS(50));          /* let the first frame reach the panel */
    board_backlight_on();

    /* Keep still and upright now: 2 s calibration */
    esp_err_t err = imu_task_start(i2c_bus);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "MPU-6050 not found (%s). Check the 4 wires on the I2C connector.", esp_err_to_name(err));
    }
}
