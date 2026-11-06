/**
 * @file imu_task.h
 *
 * MPU-6050 (GY-521) on the board's I2C connector (SDA = IO38, SCL = IO39)
 * -> lean angle and longitudinal / lateral g -> the UI subjects.
 *
 * ESP-IDF v5.2+ only. Not part of the LVGL Pro export and not built by the
 * simulator: add both files to your ESP-IDF main component.
 */
#ifndef IMU_TASK_H
#define IMU_TASK_H

#include "driver/i2c_master.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Start reading the MPU-6050 at 100 Hz in its own FreeRTOS task.
 *
 * Keep the device still, mounted as it will be used and upright, for about
 * 2 s after this call: that is when the gyro bias and the "upright" position
 * are calibrated.
 *
 * @param bus  An I2C bus already created on IO38/IO39 (e.g. by the touch
 *             driver, if it shares the connector), or NULL to create one.
 */
esp_err_t imu_task_start(i2c_master_bus_handle_t bus);

/**
 * Copy the latest IMU values into the UI subjects every 33 ms.
 * Call it once, from the LVGL thread (or with the LVGL lock held),
 * after inclin_ui_init() and after the screen is created.
 */
void imu_ui_bind(void);

/** Re-run the 2 s calibration. The device must be still and upright. */
void imu_request_calibration(void);

#ifdef __cplusplus
}
#endif

#endif /* IMU_TASK_H */
