# Board Instructions

## Internal Supported Development Boards

### [Espressif](https://www.espressif.com/en/products/devkits)

|                                                                     **Picture**                                                                      |                                                              **Name**                                                              |   **LCD Bus**    | **LCD Controller** | **LCD resolution** | **Touch Bus** | **Touch Controller** |
| :--------------------------------------------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------------------------------: | :--------------: | :----------------: | :----------------: | :-----------: | :------------------: |
|             <img src="https://docs.espressif.com/projects/esp-dev-kits/en/latest/_images/esp32-c3-lcdkit-isometric-raw.png" width="150">             |          [ESP32-C3-LCDkit](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32c3/esp32-c3-lcdkit/index.html)          |       SPI        |       GC9A01       |      240x240       |       -       |          -           |
|                        <img src="https://github.com/espressif/esp-box/raw/master/docs/_static/esp32_s3_box.png" width="150">                         |                                  [ESP32-S3-BOX](https://github.com/espressif/esp-box/tree/master)                                  |       SPI        |      ILI9342       |      320x240       |      I2C      |       TT21100        |
|                       <img src="https://github.com/espressif/esp-box/raw/master/docs/_static/esp32_s3_box_3.png" width="150">                        |                        [ESP32-S3-BOX-3 & ESP32-S3-BOX-3B](https://github.com/espressif/esp-box/tree/master)                        |       SPI        |      ILI9342       |      320x240       |      I2C      |        GT911         |
| <img src="https://raw.githubusercontent.com/espressif/esp-box/c4c954888e11250423f083df0067d99e22d59fbe/docs/_static/esp32_s3_box_3.png" width="150"> |             [ESP32-S3-BOX-3(beta)](https://github.com/espressif/esp-box/tree/c4c954888e11250423f083df0067d99e22d59fbe)             |       SPI        |      ILI9342       |      320x240       |      I2C      |       TT21100        |
|                      <img src="https://github.com/espressif/esp-box/raw/master/docs/_static/esp32_s3_box_lite.png" width="150">                      |                               [ESP32-S3-BOX-Lite](https://github.com/espressif/esp-box/tree/master)                                |       SPI        |       ST7789       |      320x240       |       -       |          -           |
|             <img src="https://github.com/espressif/esp-who/raw/master/docs/_static/get-started/ESP32-S3-EYE-isometric.png" width="100">              |     [ESP32-S3-EYE](https://github.com/espressif/esp-who/blob/master/docs/en/get-started/ESP32-S3-EYE_Getting_Started_Guide.md)     |       SPI        |       ST7789       |      240x240       |       -       |          -           |
|               <img src="https://docs.espressif.com/projects/esp-adf/en/latest/_images/esp32-s3-korvo-2-v3.0-overview.png" width="150">               | [ESP32-S3-Korvo-2](https://docs.espressif.com/projects/esp-adf/en/latest/design-guide/dev-boards/user-guide-esp32-s3-korvo-2.html) |       SPI        |      ILI9342       |      320x240       |      I2C      |       TT21100        |
|             <img src="https://docs.espressif.com/projects/esp-dev-kits/en/latest/_images/ESP32-S3-LCD-EV-Board_480x480.png" width="150">             |    [ESP32-S3-LCD-EV-Board](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-lcd-ev-board/index.html)    | 3-wire SPI + RGB |       GC9503       |      480x480       |      I2C      |        FT5x06        |
|             <img src="https://docs.espressif.com/projects/esp-dev-kits/en/latest/_images/ESP32-S3-LCD-EV-Board_800x480.png" width="150">             |   [ESP32-S3-LCD-EV-Board-2](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-lcd-ev-board/index.html)   |       RGB        |     ST7262E43      |      800x480       |      I2C      |        GT1151        |
|               <img src="https://docs.espressif.com/projects/esp-dev-kits/en/latest/_images/pic_product_esp32_s3_otg.png" width="150">                |         [ESP32-S3-USB-OTG](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-usb-otg/index.html)         |       SPI        |       ST7789       |      240x240       |       -       |          -           |
|               <img src="https://www.espressif.com/sites/default/files/dev-board/ESP32-P4-Function-EV-Board_L_2.png" width="150">                |         [ESP32-P4-Function-EV-Board](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32p4/esp32-p4-function-ev-board/index.html)         |       MIPI-DSI        |       EK79007       |      1024x600       |       I2C       |          GT911           |

### [Elecrow](https://www.elecrow.com/)

|                                                                 **Picture**                                                                  |                                                       **Name**                                                        | **LCD Bus** |   **LCD Controller**    | **LCD resolution** | **Touch Bus** | **Touch Controller** |
| :------------------------------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------------: | :---------: | :---------------------: | :----------------: | :-----------: | :------------------: |
| <img src="https://www.elecrow.com/media/catalog/product/cache/acf3559c3a3e20af42aec3d2d8cc99f6/e/s/esp32_7inch_display_1_1.png" width="150"> | [CrowPanel 7.0"](https://www.elecrow.com/esp32-display-7-inch-hmi-display-rgb-tft-lcd-touch-screen-support-lvgl.html) |     RGB     | EK9716BD3 & EK73002ACGB |      800x480       |      I2C      |        GT911         |

### [M5Stack](https://m5stack.com/)

|                                                               **Picture**                                                               |                          **Name**                           | **LCD Bus** | **LCD Controller** | **LCD resolution** | **Touch Bus** | **Touch Controller** |
| --------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------- | ----------- | ------------------ | ------------------ | ------------- | -------------------- |
| <img src="https://static-cdn.m5stack.com/resource/docs/products/core/core2/core2_01.webp" width="150">                                  | [M5STACK_M5CORE2](https://docs.m5stack.com/en/core/core2)   | SPI         | ILI9342C           | 320x240            | I2C           | FT6336U              |
| <img src="https://static-cdn.m5stack.com/resource/docs/products/core/M5Dial/img-2afd549e-8af8-47b4-823a-e90e063a0139.webp" width="150"> | [M5STACK_M5DIAL](https://docs.m5stack.com/en/core/M5Dial)   | SPI         | GC9A01             | 240x240            | I2C           | FT5x06               |
| <img src="https://static-cdn.m5stack.com/resource/docs/products/core/CoreS3/img-96063e2a-637a-4d11-ac47-1ce4f1cdfd3e.webp" width="150"> | [M5STACK_M5CORES3](https://docs.m5stack.com/en/core/CoreS3) | SPI         | ILI9342C           | 320x240            | I2C           | FT6336U              |


### [Shenzhen Jingcai Intelligent](https://www.displaysmodule.com/)

|                                                                                                                                       **Picture**                                                                                                                                       |                                                **Name**                                                 |   **LCD Bus**    | **LCD Controller** | **LCD resolution** | **Touch Bus** | **Touch Controller** |
| :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------------------: | :--------------: | :----------------: | :----------------: | :-----------: | :------------------: |
| [<img src="https://www.displaysmodule.com/photo/ps162171631-experience_the_power_of_the_esp32_display_module_sku_esp32_4848s040c_i_y_3.jpg" width="150">](https://www.displaysmodule.com/sale-41828962-experience-the-power-of-the-esp32-display-module-sku-esp32-4848s040c-i-y-3.html) | [ESP32-4848S040C_I_Y_3](http://pan.jczn1688.com/directlink/1/ESP32%20module/4.0inch_ESP32-4848S040.zip) | 3-wire SPI + RGB |       ST7701       |      480x480       |      I2C      |        GT911         |

## [Waveshare](https://www.waveshare.com/)

|                                                                                                                                       **Picture**                                                                                                                                       |                                                **Name**                                                 |   **LCD Bus**    | **LCD Controller** | **LCD resolution** | **Touch Bus** | **Touch Controller** |
| :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------------------: | :--------------: | :----------------: | ------------------ | :-----------: | :------------------: |
| <img src="https://www.waveshare.com/w/upload/5/5f/ESP32-S3-Touch-LCD-1.85_Entity.jpg" width="150"> | [ESP32-S3-Touch-LCD-1.85](https://www.waveshare.com/esp32-s3-touch-lcd-1.85.htm) | QSPI |       ST77916       |      360x360       |      I2C      |        CST816         |
| <img src="https://www.waveshare.com/w/upload/thumb/1/10/ESP32-S3-Touch-LCD-2.1.jpg/300px-ESP32-S3-Touch-LCD-2.1.jpg" width="150"> | [ESP32-S3-Touch-LCD-2.1](https://www.waveshare.com/esp32-s3-touch-lcd-2.1.htm) | RGB |       ST7701       |      480x480       |      I2C      |        CST820 (CST816-like)         |
| <img src="https://www.waveshare.com/media/catalog/product/cache/1/image/800x800/9df78eab33525d08d6e5fb8d27136e95/e/s/esp32-s3-touch-lcd-4.3-1.jpg" width="150"> | [ESP32-S3-Touch-LCD-4.3](https://www.waveshare.com/esp32-s3-touch-lcd-4.3.htm) | RGB |       ST7262       |      800x480       |      I2C      |        GT911         |
| <img src="https://www.waveshare.com/media/catalog/product/cache/1/image/800x800/9df78eab33525d08d6e5fb8d27136e95/e/s/esp32-s3-touch-lcd-4.3b-1.jpg" width="150"> | [ESP32-S3-Touch-LCD-4.3B](https://www.waveshare.com/esp32-s3-touch-lcd-4.3B.htm) | RGB |       ST7262       |      800x480       |      I2C      |        GT911         |
| <img src="https://www.waveshare.com/media/catalog/product/cache/1/image/800x800/9df78eab33525d08d6e5fb8d27136e95/e/s/esp32-s3-touch-lcd-5-1.jpg" width="150"> | [ESP32-S3-Touch-LCD-5](https://www.waveshare.com/esp32-s3-touch-lcd-5.htm?sku=28117) | RGB |       ST7262       |      800x480       |      I2C      |        GT911         |
| <img src="https://www.waveshare.com/media/catalog/product/cache/1/image/800x800/9df78eab33525d08d6e5fb8d27136e95/e/s/esp32-s3-touch-lcd-5-1.jpg" width="150"> | [ESP32-S3-Touch-LCD-5B](https://www.waveshare.com/esp32-s3-touch-lcd-5.htm?sku=28151) | RGB |       ST7262       |      1024x600       |      I2C      |        GT911         |
| <img src="https://www.waveshare.com/media/catalog/product/cache/1/image/800x800/9df78eab33525d08d6e5fb8d27136e95/e/s/esp32-s3-touch-lcd-7-1.jpg" width="150"> | [ESP32-S3-Touch-LCD-7](https://www.waveshare.com/esp32-s3-touch-lcd-7.htm) | RGB |       ST7262       |      800x480       |      I2C      |        GT911         |
| <img src="https://www.waveshare.com/media/catalog/product/cache/1/image/800x800/9df78eab33525d08d6e5fb8d27136e95/e/s/esp32-p4-nano-1.jpg" width="150"> | [ESP32-P4-NANO](https://www.waveshare.com/esp32-p4-nano.htm) |  MIPI-DSI   |       JD9365       | 800x1280           |      I2C      |        GT9271 (GT911-like)        |

## [VIEWE](https://viewedisplay.com/)

|                                                                                                                                       **Picture**                                                                                                                                       |                                                **Name**                                                 |   **LCD Bus**    |**LCD Controller**| **LCD resolution** | **Touch Bus** | **Touch Controller** |
| :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------------------: | :--------------: | :----------------: | ------------------ | :-----------: | :------------------: |
| <img src="https://viewedisplay.com/wp-content/uploads/2024/11/UEDX24320024E-WB-A.jpg" width="150"> | [UEDX24320028E-WB-A-2.4](https://viewedisplay.com/product/esp32-2-4-inch-240x320-rgb-ips-tft-display-touch-screen-arduino-lvgl-wifi-ble-uart-smart-module/) | SPI |       GC9307(like GC9A01)       |      240x320       |      I2C      |        CHSC6540         |
| <img src="https://viewedisplay.com/wp-content/uploads/2024/11/UEDX24320028E-WB-A.jpg" width="150"> | [UEDX24320028E-WB-A-2.8](https://viewedisplay.com/product/esp32-2-8-inch-240x320-mcu-ips-tft-display-touch-screen-arduino-lvgl-wifi-ble-uart-smart-module/) | SPI |       GC9307(like GC9A01)       |      240x320       |      I2C      |        CHSC6540         |
| <img src="https://viewedisplay.com/wp-content/uploads/2024/11/UEDX24320035E-WB-A-1.jpg" width="150"> | [UEDX24320028E-WB-A-3.5-240x320](https://viewedisplay.com/product/esp32-3-5-inch-240x320-mcu-ips-tft-display-touch-screen-arduino-lvgl-wifi-ble-uart-smart-module/) | SPI |       GC9307(like GC9A01)       |      240x320       |      I2C      |        CHSC6540         |
| <img src="https://viewedisplay.com/wp-content/uploads/2024/11/UEDX32480035E-WB-A.jpg" width="150"> | [UEDX24320028E-WB-A-3.5-320x480](https://viewedisplay.com/product/esp32-3-5-inch-320x4-mcu-ips-tft-display-touch-screen-arduino-lvgl-wifi-ble-uart-smart-module/) | SPI |       ST7365P(like ST7789)       |      320x480       |      I2C      |        CHSC6540         |
| <img src="https://viewedisplay.com/wp-content/uploads/2024/07/DX48480040E-WB-A-%E6%AD%A3.jpg" width="150"> | [UEDX48480040E-WB-A](https://viewedisplay.com/product/esp32-4-inch-tft-display-touch-screen-arduino-lvgl/) | 3-wire SPI + RGB |       GC9503       |      480x480       |      I2C      |         FT6336U(like FT5x06)        |
| <img src="https://viewedisplay.com/wp-content/uploads/2024/07/UEDX80480043E-13.jpg" width="150"> | [UEDX80480043E-WB-A-800x480](https://viewedisplay.com/product/esp32-4-3-inch-800x480-rgb-ips-tft-display-touch-screen-arduino-lvgl/) | RGB |       ST7262       |      800x480       |      I2C      |        GT911         |
| <img src="https://viewedisplay.com/wp-content/uploads/2024/07/UEDX80480043E-13.jpg" width="150"> | [UEDX80480043E-WB-A-480x272](https://viewedisplay.com/product/esp32-4-3-inch-480x272-rgb-ips-tft-display-touch-screen-arduino-lvgl-wifi-ble-uart-smart-module/) | RGB |       ST7262       |      480x272      |      I2C      |        GT911         |
| <img src="https://viewedisplay.com/wp-content/uploads/2024/06/DX80480050E-aa.jpg" width="150"> | [UEDX80480050E-WB-A](https://viewedisplay.com/product/esp32-5-inch-800x480-rgb-ips-tft-display-touch-screen-arduino-lvgl/) | RGB |       ST7262E43-G4(like ST7262)       |      800x480       |      I2C      |        GT911         |
| <img src="https://viewedisplay.com/wp-content/uploads/2024/08/DX80480070E-a2.jpg" width="150"> | [UEDX80480070E-WB-A](https://viewedisplay.com/product/esp32-7-inch-800x480-rgb-ips-tft-display-touch-screen-arduino-lvgl-uart/) | RGB |       EK9716BD3+EK73002AB2(like ST7262)       |      800x480       |      I2C      |        GT911         |

## Recommended Configurations in the Arduino IDE

Below are recommended configurations for developing GUI applications on different development boards. These settings can be adjusted according to specific requirements, and users can navigate to the `Tools` menu in the Arduino IDE to configure the following settings.

|         Supported Boards          |   Selected Board   |  PSRAM   | Flash Mode | Flash Size | USB CDC On Boot |    Partition Scheme     |
|:---------------------------------:|:------------------:|:--------:|:----------:|:----------:|:---------------:|:-----------------------:|
|          ESP32-C3-LCDkit          | ESP32C3 Dev Module | Disabled |    QIO     | 4MB (32Mb) |     Enabled     | Default 4MB with spiffs |
|           ESP32-S3-BOX            |    ESP32-S3-BOX    |    -     |     -      |     -      |        -        |     16M Flash (3MB)     |
| ESP32-S3-BOX-3 & ESP32-S3-BOX-3B  | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB    |     Enabled     |     16M Flash (3MB)     |
|       ESP32-S3-BOX-3(beta)        | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB    |     Enabled     |     16M Flash (3MB)     |
|         ESP32-S3-BOX-Lite         |    ESP32-S3-BOX    |    -     |     -      |     -      |        -        |     16M Flash (3MB)     |
|           ESP32-S3-EYE            | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    8MB     |     Enabled     |     8M with spiffs      |
|         ESP32-S3-Korvo-2          | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB    |    Disabled     |     16M Flash (3MB)     |
|       ESP32-S3-LCD-EV-Board       | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB    | **See Note 1**  |     16M Flash (3MB)     |
|      ESP32-S3-LCD-EV-Board-2      | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB    | **See Note 1**  |     16M Flash (3MB)     |
|         ESP32-S3-USB-OTG          |  ESP32-S3-USB-OTG  |    -     |     -      |     -      |        -        |     8M with spiffs      |
|    ESP32-P4-Function-EV-Board     | ESP32P4 Dev Module | Enabled  |    QIO     |    16MB    |    Disabled     |     16M Flash (3MB)     |
|          M5STACK-M5CORE2          |   M5Stack-Core2    | Enabled  |     -      |     -      |        -        |         Default         |
|          M5STACK-M5DIAL           | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    8MB     |    Disabled     |         Default         |
|         M5STACK-M5CORES3          | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB    |     Enabled     | Default 4MB with spiffs |
|       ESP32-4848S040C_I_Y_3       | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB    |    Disabled     |     16M Flash (3MB)     |
|       ElecrowCrowPanel 7.0"       | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    4MB     |    Disabled     |     Huge App (3MB)      |
| Waveshare-ESP32-S3-Touch-LCD-1.85 | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB    |     Enabled     |     16M Flash (3MB)     |
| Waveshare-ESP32-S3-Touch-LCD-2.1  | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB    |     Enabled     |     16M Flash (3MB)     |
| Waveshare-ESP32-S3-Touch-LCD-4.3  | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    8MB     |    Disabled     |     8M with spiffs      |
| Waveshare-ESP32-S3-Touch-LCD-4.3B | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB    |    Enabled      |     16M Flash (3MB)     |
| Waveshare-ESP32-S3-Touch-LCD-5    | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB    |    Enabled      |     16M Flash (3MB)     |
| Waveshare-ESP32-S3-Touch-LCD-5B   | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB    |    Enabled      |     16M Flash (3MB)     |
| Waveshare-ESP32-S3-Touch-LCD-7    | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    8MB     |    Disabled     |     8M with spiffs      |
|      Waveshare-ESP32-P4-NANO      | ESP32P4 Dev Module | Enabled  |    QIO     |    16MB    |    Disabled     |     16M Flash (3MB)     |
|      UEDX24320028E-WB-A-2.4       | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB     |    Enabled     |     16M Flash (3MB)     |
|      UEDX24320028E-WB-A-2.8       | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB     |    Enabled     |     16M Flash (3MB)     |
|   UEDX24320028E-WB-A-3.5-240x320  | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB     |    Enabled     |     16M Flash (3MB)     |
|   UEDX24320028E-WB-A-3.5-320x480  | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB     |    Enabled     |     16M Flash (3MB)     |
|        UEDX48480040E-WB-A         | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB     |    Enabled     |     16M Flash (3MB)     |
|    UEDX80480043E-WB-A-800x480     | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB     |    Enabled     |     16M Flash (3MB)     |
|    UEDX80480043E-WB-A-480x272     | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB     |    Enabled     |     16M Flash (3MB)     |
|        UEDX80480050E-WB-A         | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB     |    Enabled     |     16M Flash (3MB)     |
|        UEDX80480070E-WB-A         | ESP32S3 Dev Module |   OPI    | QIO 80MHz  |    16MB     |    Enabled     |     16M Flash (3MB)     |

**Notes:**

1. Enable or disable `USB CDC On Boot` based on the type of port used:

   * Disable this configuration if using **UART** port; enable it if using **USB** port.
   * If this configuration differs from previous flashing, first enable `Erase All Flash Before Sketch Upload`, then it can be disabled after flashing.
   * If this configuration does not match the actual port type, it will prevent the development board from printing serial logs correctly.

2. To view more output logs, set `Core Debug Level` to `Info` or a lower level.
3. If the predefined partition schemes provided by ESP32 do not meet the requirements, users can also select `Custom` in the "Partition Scheme" and create a custom partition table file `Custom.csv` in the `hardware/esp32/3.x.x/tools/partitions` directory under the [arduino-esp32 installation directory](#where-are-the-installation-directory-for-arduino-esp32-and-the-sdk-located). For detailed information on partition tables, please refer to the [ESP-IDF documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/partition-tables.html).
