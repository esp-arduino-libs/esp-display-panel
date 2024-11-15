# 常见问题解答

* [English Version](./FAQ.md)

## 目录

- [常见问题解答](#常见问题解答)
  - [目录](#目录)
  - [Arduino 库的目录在哪儿？](#arduino-库的目录在哪儿)
  - [如何在 Arduino IDE 中安装 ESP32\_Display\_Panel？](#如何在-arduino-ide-中安装-esp32_display_panel)
  - [arduino-eps32 的安装目录以及 SDK 的目录在哪儿？](#arduino-eps32-的安装目录以及-sdk-的目录在哪儿)
  - [使用 ESP32-S3 驱动 RGB LCD 时出现画面漂移问题的解决方案](#使用-esp32-s3-驱动-rgb-lcd-时出现画面漂移问题的解决方案)
  - [如何在 PlatformIO 上使用 ESP32\_Display\_Panel？](#如何在-platformio-上使用-esp32_display_panel)

## Arduino 库的目录在哪儿？

您可以在 Arduino IDE 的菜单栏中选择 `File` > `Preferences` > `Settings` > `Sketchbook location` 来查找和修改 Arduino 库的目录路径。

## 如何在 Arduino IDE 中安装 ESP32_Display_Panel？

- 如果您想要在线安装，可以在 Arduino IDE 中导航到 `Sketch` > `Include Library` > `Manage Libraries...`，然后搜索 `ESP32_Display_Panel`，点击 `Install` 按钮进行安装。
- 如果您想要手动安装，可以从 [ESP32_Display_Panel](https://github.com/esp-arduino-libs/ESP32_Display_Panel) 下载所需版本的 `.zip` 文件，然后在 Arduino IDE 中导航到 `Sketch` > `Include Library` > `Add .ZIP Library...`，选择下载的 `.zip` 文件并点击 `Open` 按钮进行安装。
- 您还可以查阅 [Arduino IDE v1.x.x](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries) 或者 [Arduino IDE v2.x.x](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-installing-a-library) 文档中关于安装库的指南。

## arduino-eps32 的安装目录以及 SDK 的目录在哪儿？

arduino-esp32 的默认安装路径取决于您的操作系统：

- Windows: `C:\Users\<user name>\AppData\Local\Arduino15\packages\esp32`
- Linux: `~/.arduino15/packages/esp32`

arduino-esp32 v3.x.x 版本的 SDK 位于默认安装路径下的 `tools > esp32-arduino-libs > idf-release_x` 目录中。

## 使用 ESP32-S3 驱动 RGB LCD 时出现画面漂移问题的解决方案

当您在使用 ESP32-S3 驱动 RGB LCD 时遇到画面漂移的问题时，您可以采用以下步骤来解决：

1. **查看文档**：详细了解问题的说明，您可以参考[这篇文档](https://docs.espressif.com/projects/esp-faq/zh_CN/latest/software-framework/peripherals/lcd.html#esp32-s3-rgb-lcd)。

2. **启用 `Bounce Buffer + XIP on PSRAM` 特性**：为了解决问题，推荐启用 `Bounce Buffer + XIP on PSRAM` 特性。具体步骤如下：

   - **Step1**：从 [arduino-esp32-sdk](https://github.com/esp-arduino-libs/arduino-esp32-sdk) 下载 "high_perf" 版本的 SDK，并将其替换到 [arduino-esp32 的安装目录](#arduino-eps32-的安装目录以及-sdk-的目录在哪儿)中。

   - **Step2**：如果您使用的是支持的开发板，则通常无需修改代码，因为它们默认设置了 `ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE` 为 `(ESP_PANEL_BOARD_WIDTH * 10)`。如果问题仍然存在，请参考下面的示例代码来增大 `Bounce Buffer` 的大小。

   - **Step3**：如果您使用的是自定义的开发板，请在 `esp_panel_board_custom.h` 文件中确认 `ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE` 是否设置为非 0。如果问题仍然存在，请增大 `Bounce Buffer` 的大小。

   - **Step4**：如果您使用的是独立的驱动，请参考下面的示例代码来设置 `Bounce Buffer` 的大小。

   - **Step5**：如果您正在开发 LVGL 应用，将执行 RGB 外设初始化的任务与执行 LVGL lv_timer_handler() 的任务分配在同一个核上，请参考 [代码](../examples/LVGL/v8/Porting/lvgl_port_v8.h#L53)。

3. **示例代码**：以下示例代码展示了如何通过 `ESP_Panel` 驱动或独立的驱动来修改 `Bounce Buffer` 的大小：

   **Example1**：使用 `ESP_Panel` 驱动修改 `Bounce Buffer` 大小：

    ```c
    ...
    ESP_Panel *panel = new ESP_Panel();
    panel->init();
    // Start
    ESP_PanelBus_RGB *rgb_bus = static_cast<ESP_PanelBus_RGB *>(panel->getLcd()->getBus());
    // The size of the bounce buffer must satisfy `width_of_lcd * height_of_lcd = size_of_buffer * N`, where N is an even number.
    rgb_bus->configRgbBounceBufferSize((ESP_PANEL_BOARD_WIDTH * 20));
    // End
    panel->begin();
    ...
    ```

   **Example2**：使用独立的驱动修改 `Bounce Buffer` 大小：

    ```c
    ...
    ESP_PanelBus_RGB *lcd_bus = new ESP_PanelBus_RGB(...);
    // Start
    // The size of the bounce buffer must satisfy `width_of_lcd * height_of_lcd = size_of_buffer * N`, where N is an even number.
    lcd_bus->configRgbBounceBufferSize(EXAMPLE_LCD_WIDTH * 10);
    // End
    lcd_bus->begin();
    ...
    ```

## 如何在 PlatformIO 上使用 ESP32_Display_Panel？

您可以参考示例 [PlatformIO](../examples/PlatformIO/) 在 PlatformIO 中使用 ESP32_Display_Panel 库，它默认情况下适用于 **ESP32-S3-LCD-EV-Board** and **ESP32-S3-LCD-EV-Board-2** 开发板，您需要根据实际情况修改 [boards/ESP-LCD.json](../examples/PlatformIO/boards/ESP-LCD.json) 文件。
