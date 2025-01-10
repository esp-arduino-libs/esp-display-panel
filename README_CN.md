[![Arduino Lint](https://github.com/esp-arduino-libs/ESP32_Display_Panel/actions/workflows/arduino_lint.yml/badge.svg)](https://github.com/esp-arduino-libs/ESP32_Display_Panel/actions/workflows/arduino_lint.yml) [![Version Consistency](https://github.com/esp-arduino-libs/ESP32_Display_Panel/actions/workflows/check_lib_versions.yml/badge.svg)](https://github.com/esp-arduino-libs/ESP32_Display_Panel/actions/workflows/check_lib_versions.yml)

**最新 Arduino 库版本**: [![GitHub Release](https://img.shields.io/github/v/release/esp-arduino-libs/ESP32_Display_Panel)](https://github.com/esp-arduino-libs/ESP32_Display_Panel/releases)

**最新 Espressif 组件版本**: [![Espressif Release](https://components.espressif.com/components/espressif/esp32_display_panel/badge.svg)](https://components.espressif.com/components/espressif/esp32_display_panel)

# ESP Display Panel

* [English Version](./README.md)

ESP32_Display_Panel 是专为 ESP SoCs 设计的用于驱动显示屏并实现快速 GUI 开发的库。用户不仅可以直接开发多款[内部支持的开发板](docs/Board_Instructions.md)，还可以通过简单的适配来开发自定义的开发板。此外，ESP32_Display_Panel 还适配了多款 LCD 和触摸的驱动，用户也可以根据需要使用独立的驱动进行开发。

ESP32_Display_Panel 内部集成了多个[乐鑫组件库](https://components.espressif.com/)中显示屏相关的驱动组件，它可以直接从该组件库或从 Arduino IDE 中下载获取，因此用户可以基于 [Arduino](https://github.com/espressif/arduino-esp32) IDE 或 [ESP-IDF](https://github.com/espressif/esp-idf) 框架进行开发。

## 概述

ESP32_Display_Panel 的功能框图如下所示，主要包含以下特性：

- 支持多种 **Espressif** 官方及第三方开发板，包括 **M5Stack**、**Elecrow**、**Waveshare**、**VIEWE** 等。
- 支持适配 **自定义的开发板**。
- 支持多种类型的设备驱动，包括 **接口总线**、**LCD**、**触摸**、**背光** 和 **IO 扩展**。
- 支持动态配置驱动，如开启调试 LOG 等。
- 支持使用 **Arduino** IDE 或 **ESP-IDF** 框架进行编译。

<div align="center"><img src="docs/_static/block_diagram.png" alt ="块图" width="600"></div>

## 如何使用

请参阅文档 - [如何使用](./docs/How_To_Use_CN.md) 。

## 支持的开发板和驱动

### 开发板

下面是支持的[开发板列表](docs/Board_Instructions.md)：

| **厂商** | **开发板型号** |
| -------- | -------------- |
| [Espressif](docs/Board_Instructions.md#espressif) | ESP32-C3-LCDkit, ESP32-S3-BOX, ESP32-S3-BOX-3, ESP32-S3-BOX-3B, ESP32-S3-BOX-3(beta), ESP32-S3-BOX-Lite, ESP32-S3-EYE, ESP32-S3-Korvo-2, ESP32-S3-LCD-EV-Board, ESP32-S3-LCD-EV-Board-2, ESP32-S3-USB-OTG, ESP32-P4-Function-EV-Board |
| [M5Stack](docs/Board_Instructions.md#m5stack) | M5STACK-M5CORE2, M5STACK-M5DIAL, M5STACK-M5CORES3 |
| [Elecrow](docs/Board_Instructions.md#elecrow) | CrowPanel 7.0" |
| [Jingcai](docs/Board_Instructions.md#shenzhen-jingcai-intelligent) | ESP32-4848S040C_I_Y_3 |
| [Waveshare](docs/Board_Instructions.md#waveshare) | ESP32-S3-Touch-LCD-1.85, ESP32-S3-Touch-LCD-2.1, ESP32-S3-Touch-LCD-4.3, ESP32-S3-Touch-LCD-4.3B, ESP32-S3-Touch-LCD-5, ESP32-S3-Touch-LCD-5B, ESP32-S3-Touch-LCD-7, ESP32-P4-NANO |
| [VIEWE](docs/Board_Instructions.md#viewe) | UEDX24320028E-WB-A-2.4, UEDX24320028E-WB-A-2.8, UEDX24320028E-WB-A-3.5-240x320, UEDX24320028E-WB-A-3.5-320x480, DX48480040E-WB-A, UEDX80480043E-WB-A-4.3-800x480, UEDX80480043E-WB-A-4.3-480x272, UEDX80480050E-WB-A, UEDX80480070E-WB-A|

欢迎开发者和厂商贡献 PR 来添加更多的开发板，详细说明请参考 [`开发板贡献指南`](./docs/Board_Contribution_Guide_CN.md)。

### LCD 控制器

下面是支持的 [LCD 控制器列表](docs/LCD_Controllers.md)：

| **厂商** | **型号** |
| -------- | -------- |
| Fitipower | EK9716B, EK79007 |
| GalaxyCore | GC9A01, GC9B71, GC9503 |
| Ilitek | ILI9341, ILI9881C |
| JADARD | JD9365 |
| NewVision | NV3022B |
| Sitronix | ST7262, ST7701, ST7789, ST7796, ST77916, ST77922 |

### 触摸控制器

下面是支持的 [触摸控制器列表](docs/Touch_Controllers.md)：

| **厂商** | **型号** |
| -------- | -------- |
| FocalTech | FT5x06 |
| GOODiX | GT911, GT1151 |
| Hynitron | CST816S |
| Parade | TT21100 |
| Sitronix | ST7123 |
| Xptek | XPT2046 |
|Chipsemicorp | CHSC6540 |

## 常见问题解答

请参阅文档 - [常见问题解答](./docs/FAQ_CN.md) 。
