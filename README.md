# lean-interface

Motorcycle lean angle and G-meter display for the LCDWIKI ES3C35P
(ESP32-S3, 3.5" 480x320 touch display) with an MPU-6050.

- [`inclin_ui/`](inclin_ui) - the UI, written in LVGL Pro XML. Export it with **Ctrl+E** in the LVGL Pro Editor.
- [`inclin_fw/`](inclin_fw) - the ESP-IDF v6.1 firmware. It compiles the exported UI from `../inclin_ui`, so keep both folders side by side.

See [inclin_fw/README.md](inclin_fw/README.md) for wiring, building and flashing.
