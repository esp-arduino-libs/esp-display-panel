[![Arduino Lint](https://github.com/esp-arduino-libs/ESP32_Display_Panel/actions/workflows/arduino_lint.yml/badge.svg)](https://github.com/esp-arduino-libs/ESP32_Display_Panel/actions/workflows/arduino_lint.yml) [![Version Consistency](https://github.com/esp-arduino-libs/ESP32_Display_Panel/actions/workflows/check_lib_versions.yml/badge.svg)](https://github.com/esp-arduino-libs/ESP32_Display_Panel/actions/workflows/check_lib_versions.yml)

**Latest Arduino Library Version**: [![GitHub Release](https://img.shields.io/github/v/release/esp-arduino-libs/ESP32_Display_Panel)](https://github.com/esp-arduino-libs/ESP32_Display_Panel/releases)

**Latest Espressif Component Version**: [![Espressif Release](https://components.espressif.com/components/espressif/esp32_display_panel/badge.svg)](https://components.espressif.com/components/espressif/esp32_display_panel)

# ESP Display Panel

* [中文版本](./README_CN.md)

ESP32_Display_Panel is a library designed specifically for ESP SoCs to drive display screens and enable rapid GUI development. Users can develop on multiple [internally supported development boards](#Development-Boards) directly or use simple adaptations for custom boards. Additionally, ESP32_Display_Panel supports various LCD and touch drivers, allowing users to develop with standalone drivers as needed.

ESP32_Display_Panel integrates multiple display-related driver components from the [ESP Component Registry](https://components.espressif.com/). It can be obtained directly from the Espressif's server or downloaded from the Arduino IDE, enabling development with either the [Arduino](https://github.com/espressif/arduino-esp32) IDE or the [ESP-IDF](https://github.com/espressif/esp-idf) framework.

## Overview

The functional block diagram of ESP32_Display_Panel is shown below and includes the following features:

- Supports a variety of **Espressif** official and third-party development boards, including **M5Stack**, **Elecrow**, **Waveshare**,**VIEWE** and others.
- Supports **custom development board** adaptation.
- Supports a variety of device drivers, including interface **Bus**, **LCD**, **Touch**, **Backlight** and **IO Expander**.
- Supports dynamic driver configuration, such as enabling debug logs.
- Compatible with the **Arduino** IDE and **ESP-IDF** framework for compilation.

<div align="center"><img src="docs/_static/block_diagram.png" alt ="Block Diagram" width="600"></div>

## How to Use

Please refer to the documentation - [How to Use](./docs/How_To_Use.md).

## Supported Development Boards and Drivers

### Development Boards

Below is the list of [Supported Development Boards](docs/Board_Instructions.md):

| **Manufacturer** | **Board Model** |
| ---------------- | --------------- |
| [Espressif](docs/Board_Instructions.md#espressif) | ESP32-C3-LCDkit, ESP32-S3-BOX, ESP32-S3-BOX-3, ESP32-S3-BOX-3B, ESP32-S3-BOX-3(beta), ESP32-S3-BOX-Lite, ESP32-S3-EYE, ESP32-S3-Korvo-2, ESP32-S3-LCD-EV-Board, ESP32-S3-LCD-EV-Board-2, ESP32-S3-USB-OTG, ESP32-P4-Function-EV-Board |
| [Elecrow](docs/Board_Instructions.md#elecrow) | CrowPanel 7.0" |
| [M5Stack](docs/Board_Instructions.md#m5stack) | M5STACK-M5CORE2, M5STACK-M5DIAL, M5STACK-M5CORES3 |
| [Jingcai](docs/Board_Instructions.md#shenzhen-jingcai-intelligent) | ESP32-4848S040C_I_Y_3 |
| [Waveshare](docs/Board_Instructions.md#waveshare) | ESP32-S3-Touch-LCD-1.85, ESP32-S3-Touch-LCD-2.1, ESP32-S3-Touch-LCD-4.3, ESP32-S3-Touch-LCD-4.3B, ESP32-S3-Touch-LCD-5, ESP32-S3-Touch-LCD-5B, ESP32-S3-Touch-LCD-7, ESP32-P4-NANO |
| [VIEWE](docs/Board_Instructions.md#viewe) | UEDX24320028E-WB-A-2.4, UEDX24320028E-WB-A-2.8, UEDX24320028E-WB-A-3.5-240x320, UEDX24320028E-WB-A-3.5-320x480, DX48480040E-WB-A, UEDX80480043E-WB-A-4.3-800x480, UEDX80480043E-WB-A-4.3-480x272, UEDX80480050E-WB-A, UEDX80480070E-WB-A|

Developers and manufacturers are welcome to contribute PRs to add more boards. For details, please refer to the [Board Contribution Guide](./docs/Board_Contribution_Guide.md).

### LCD Controllers

Below is the list of [Supported LCD Controllers](docs/LCD_Controllers.md):

| **Manufacturer** | **Model** |
| ---------------- | --------- |
| Fitipower | EK9716B, EK79007 |
| GalaxyCore | GC9A01, GC9B71, GC9503 |
| Ilitek | ILI9341, ILI9881C |
| JADARD | JD9365 |
| NewVision | NV3022B |
| Sitronix | ST7262, ST7701, ST7789, ST7796, ST77916, ST77922 |

### Touch Controllers

Below is the list of [Supported Touch Controllers](docs/Touch_Controllers.md):

| **Manufacturer** | **Model** |
| ---------------- | --------- |
| FocalTech | FT5x06 |
| GOODiX | GT911, GT1151 |
| Hynitron | CST816S |
| Parade | TT21100 |
| Sitronix | ST7123 |
| Xptek | XPT2046 |
|Chipsemicorp | CHSC6540 |

## FAQ

Please refer to the documentation - [FAQ](./docs/FAQ.md).
