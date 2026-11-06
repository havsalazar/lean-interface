# inclin_fw

ESP-IDF v6.1 firmware for the inclinometer UI on the LCDWIKI ES3C35P
(ESP32-S3, 3.5" ST77922 QSPI touch display) with an MPU-6050 on the "I2C"
connector (SDA IO38, SCL IO39).

The UI itself lives in `../inclin_ui` (LVGL Pro). After changing any XML there,
export it with **Ctrl+E** in the LVGL Pro Editor, then rebuild here.

## Build and flash

VS Code with the ESP-IDF extension: open this folder, then **Build, Flash and Monitor**.

Or in an ESP-IDF terminal:

```
idf.py -p COM4 flash monitor
```

If flashing doesn't start: hold BOOT, tap RST, release BOOT, and retry.

## Files

- `main/main.c`: starts the board, creates the UI, starts the IMU
- `main/board.c/.h`: pins, ST77922 panel and touch, LVGL port (landscape by software rotation)
- `main/imu_task.c/.h`: MPU-6050 reading, lean and g calculation, writes the UI subjects

## Adjusting

| Problem | Fix |
|---|---|
| Picture upside down | `BOARD_ROTATION` in `board.h`: `LV_DISPLAY_ROTATION_270` |
| Colors inverted | `BOARD_LCD_INVERT_COLOR` in `board.h` |
| Left/right lean swapped | `IMU_LEAN_SIGN` in `imu_task.c` |
| Accel/brake swapped | `IMU_ACCEL_SIGN` in `imu_task.c` |
| Accel/brake shows lean instead | `IMU_FWD` in `imu_task.c`: the sensor axis pointing forward |

Keep the device still and upright for 2 s after power-on (calibration).
