# ESP-IDF firmware for the inclinometer UI on the LCDWIKI ES3C35P
# (ESP32-S3, 3.5" ST77922 QSPI, 320x480 panel shown as 480x320 landscape).
cmake_minimum_required(VERSION 3.22)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(inclin_fw)
