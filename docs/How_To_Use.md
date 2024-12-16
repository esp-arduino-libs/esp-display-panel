# How to Use

* [中文版本](./How_To_Use_CN.md)

## Table of Contents

- [How to Use](#how-to-use)
  - [Table of Contents](#table-of-contents)
  - [ESP-IDF Framework](#esp-idf-framework)
    - [Dependencies and Versions](#dependencies-and-versions)
    - [Adding to Project](#adding-to-project)
    - [Configuration Instructions](#configuration-instructions)
  - [Arduino IDE](#arduino-ide)
    - [Dependencies and Versions](#dependencies-and-versions-1)
    - [Installing the Library](#installing-the-library)
    - [Configuration Instructions](#configuration-instructions-1)
      - [Configuring Drivers](#configuring-drivers)
      - [Using Supported Development Boards](#using-supported-development-boards)
      - [Using Custom Development Boards](#using-custom-development-boards)
    - [Usage Examples](#usage-examples)
      - [LCD](#lcd)
      - [Touch](#touch)
      - [Panel](#panel)
      - [LVGL v8](#lvgl-v8)
      - [SquareLine](#squareline)
    - [PlatformIO](#platformio)
  - [Other Relevant Instructions](#other-relevant-instructions)
    - [Configuring Supported Development Boards](#configuring-supported-development-boards)
    - [Configuring LVGL](#configuring-lvgl)
    - [Porting SquareLine Project](#porting-squareline-project)

## ESP-IDF Framework

### Dependencies and Versions

| **Dependency** | **Version** |
| -------------- | ----------- |
| [esp-idf](https://github.com/espressif/esp-idf) | >= 5.1 |
| [esp32_io_expander](https://components.espressif.com/components/espressif/esp32_io_expander) | ^0.1.0 |

### Adding to Project

ESP32_Display_Panel has been uploaded to the [Espressif Component Registry](https://components.espressif.com/), and users can add it to their project using the `idf.py add-dependency` command, for example:

```bash
idf.py add-dependency "espressif/esp32_display_panel"
```

Alternatively, users can create or modify the `idf_component.yml` file in the project directory. For more details, please refer to the [Espressif Documentation - IDF Component Manager](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-component-manager.html).

### Configuration Instructions

When developing with esp-idf, users can configure ESP32_Display_Panel through the menuconfig:

1. Run the command `idf.py menuconfig`.
2. Navigate to `Component config` > `ESP Display Panel Configurations`.

## Arduino IDE

### Dependencies and Versions

| **Dependency** | **Version** |
| -------------- | ----------- |
| [arduino-esp32](https://github.com/espressif/arduino-esp32) | >= v3.0.0-alpha3 |
| [ESP32_IO_Expander](https://github.com/esp-arduino-libs/ESP32_IO_Expander) | >= 0.1.0 && < 0.2.0 |

### Installing the Library

For installation of the ESP32_Display_Panel library, refer to [How to Install ESP32_Display_Panel in Arduino IDE](./FAQ.md#how-to-install-esp32_display_panel-in-arduino-ide).

### Configuration Instructions

Below are detailed instructions on how to configure ESP32_Display_Panel, mainly including [Configuring Drivers](#configuring-drivers), [Using Supported Development Boards](#using-supported-development-boards), and [Using Custom Development Boards](#using-custom-development-boards). These are all optional operations and are configured through specified header files. Users can choose to use them according to their needs, with the following characteristics:

1. The path sequence for ESP32_Display_Panel to search for configuration files is: `Current Project Directory` > `Arduino Library Directory` > `ESP32_Display_Panel Directory`.
2. All examples in ESP32_Display_Panel include their required configuration files by default, which users can directly modify macro definitions.
3. For projects without configuration files, users can copy them from the root directory or examples of ESP32_Display_Panel to their own projects.
4. If multiple projects need to use the same configuration, users can place the configuration files in the [Arduino Library Directory](./FAQ.md#where-is-the-directory-for-arduino-libraries), so that all projects can share the same configuration.

> [!WARNING]
> * The same directory can simultaneously contain both `esp_panel_board_supported.h` and `esp_panel_board_custom.h` configuration files, but they cannot be enabled at the same time, meaning `ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED` and `ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM` can only have one set to `1`.
> * If neither of the above two configuration files is enabled, users cannot use the `ESP_Panel` driver and can only use other standalone device drivers, such as `ESP_PanelBus`, `ESP_PanelLcd`, etc.
> * Since the configurations within these files might change, such as adding, deleting, or renaming, to ensure the compatibility of the program, the library manages the versions of these files independently and checks whether the configuration files currently used by the user are compatible with the library during compilation. Detailed version information and checking rules can be found at the end of the file.

#### Configuring Drivers

ESP32_Display_Panel configures driver functionality and parameters based on the [esp_panel_conf.h](../esp_panel_conf.h) file. Users can update the behavior or default parameters of the driver by modifying macro definitions in this file. For example, to enable debug log output, here is a snippet of the modified `esp_panel_conf.h` file:

```c
...
/* Set to 1 if print log message for debug */
#define ESP_PANEL_ENABLE_LOG                (1)         // 0/1
...
```

#### Using Supported Development Boards

ESP32_Display_Panel configures `ESP_Panel` as the driver for the target development board based on the [esp_panel_board_supported.h](../esp_panel_board_supported.h) file. Users can select supported development boards by modifying macro definitions in this file. For example, to use the *ESP32-S3-BOX-3* development board, follow these steps:

1. Set the `ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED` macro definition in the `esp_panel_board_supported.h` file to `1`.
2. Uncomment the corresponding macro definition for the target development board model.

Here is a snippet of the modified `esp_panel_board_supported.h` file:

```c
...
/* Set to 1 if using a supported board */
#define ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED       (1)         // 0/1

#if ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED
...
// #define BOARD_ESP32_C3_LCDKIT
// #define BOARD_ESP32_S3_BOX
#define BOARD_ESP32_S3_BOX_3
// #define BOARD_ESP32_S3_BOX_3_BETA
...
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_SUPPORTED */
```

#### Using Custom Development Boards

ESP32_Display_Panel configures `ESP_Panel` as the driver for custom development boards based on the [esp_panel_board_custom.h](../esp_panel_board_custom.h) file. Users need to modify this file according to the actual parameters of the custom development board. For example, to use a custom development board with a *480x480 RGB ST7701 LCD + I2C GT911 Touch*, follow these steps:

1. Set the `ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM` macro definition in the `esp_panel_board_custom.h` file to `1`.
2. Set the LCD-related macro definitions:
   a. Set `ESP_PANEL_BOARD_DEFAULT_USE_LCD` to `1`.
   b. Set `ESP_PANEL_BOARD_WIDTH` and `ESP_PANEL_BOARD_HEIGHT` to `480`.
   c. Set `ESP_PANEL_BOARD_LCD_BUS_TYPE` to `ESP_PANEL_BUS_TYPE_RGB`.
   d. Set LCD signal pins and other parameters below `ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB`.
   e. Uncomment and modify the `ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD` macro definition according to the initialization command parameters provided by the screen vendor.
   f. Modify other LCD configurations as needed.
3. Set the Touch-related macro definitions:
   a. Set `ESP_PANEL_BOARD_DEFAULT_USE_TOUCH` to `1`.
   b. Set Touch signal pins and other parameters below `ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C`.
   c. Modify other Touch configurations as needed.
4. Enable other driver macro definitions as needed, such as `ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT`, `ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER`, etc.

Here is a snippet of the modified `esp_panel_board_custom.h` file:

```c
...
/* Set to 1 if using a custom board */
#define ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM  (1)         // 0/1

/* Set to 1 when using an LCD panel */
#define ESP_PANEL_BOARD_DEFAULT_USE_LCD           (1)     // 0/1

#if ESP_PANEL_BOARD_DEFAULT_USE_LCD
/**
 * LCD Controller Name
 */
#define ESP_PANEL_BOARD_LCD_CONTROLLER          ST7701

/* LCD resolution in pixels */
#define ESP_PANEL_BOARD_WIDTH         (480)
#define ESP_PANEL_BOARD_HEIGHT        (480)
...
/**
 * LCD Bus Type.
 */
#define ESP_PANEL_BOARD_LCD_BUS_TYPE      (ESP_PANEL_BUS_TYPE_RGB)
/**
 * LCD Bus Parameters.
 *
 * Please refer to https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html and
 * https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/index.html for more details.
 */
#if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB
...
#endif /* ESP_PANEL_BOARD_LCD_BUS_TYPE */
...
/**
 * LCD Vendor Initialization Commands.
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code. Please uncomment and change the following macro definitions. Otherwise, the LCD driver
 * will use the default initialization sequence code.
 *
 * There are two formats for the sequence code:
 *   1. Raw data: {command, (uint8_t []){ data0, data1, ... }, data_size, delay_ms}
 *   2. Formatter: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... }) and
 *                ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */
#define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD() \
    { \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x3B, 0x00}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x0D, 0x02}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC2, {0x31, 0x05}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xCD, {0x00}), \
        ...
        ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x11), \
    }
...
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_LCD */

/* Set to 1 when using a touch panel */
#define ESP_PANEL_BOARD_DEFAULT_USE_TOUCH         (1)         // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_TOUCH
/**
 * Touch controller name
 */
#define ESP_PANEL_BOARD_TOUCH_CONTROLLER        GT911
...
/**
 * Touch panel bus type
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_TYPE    (ESP_PANEL_BUS_TYPE_I2C)
/* Touch panel bus parameters */
#if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C
...
#endif /* ESP_PANEL_BOARD_TOUCH_BUS_TYPE */
...
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_TOUCH */
...
#define ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT     (1)         // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
...
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT */
...
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM */
```

### Usage Examples

You can access them in the Arduino IDE by navigating to `File` > `Examples` > `ESP32_Display_Panel`. If you cannot find the `ESP32_Display_Panel` option, please check if the library has been installed correctly and ensure that an ESP development board is selected.

#### LCD

The following examples demonstrate how to develop different interface and model LCDs using standalone drivers and test them by displaying color bars:

* [SPI](../examples/LCD/SPI/)
* [QSPI](../examples/LCD/QSPI/)
* [Single RGB](../examples/LCD/RGB/)
* [3-wire SPI + RGB](../examples/LCD/3wireSPI_RGB/)
* [MIPI-DSI](../examples/LCD/MIPI_DSI/)

#### Touch

The following example demonstrates how to develop touch screens of different interfaces and models using standalone drivers and test them by printing touch point coordinates:

* [I2C](../examples/Touch/I2C/)
* [SPI](../examples/Touch/SPI/)

#### Panel

The following example demonstrates how to develop built-in or custom development boards using the `ESP_Panel` driver:

* [Panel Test](../examples/Panel/PanelTest/): This example tests by displaying color bars and printing touch point coordinates.

#### LVGL v8

For configuring LVGL (v8.3.x), please refer to [here](#configuring-lvgl) for more detailed information.

* [Porting](../examples/LVGL/v8/Porting/): This example demonstrates how to port LVGL (v8.3.x). And for RGB LCD, it can enable the avoid tearing function.
* [Rotation](../examples/LVGL/v8/Rotation/): This example demonstrates how to use LVGL to rotate the display.

> [!WARNING]
> Currently, the anti-tearing feature is only supported for RGB LCD and requires LVGL version >= v8.3.9. If you are using a different type of LCD or an LVGL version that does not meet the requirements, please do not enable this feature.

#### SquareLine

To port the SquareLine project (v1.3.x), please refer to [here](#porting-squareline-project) for more detailed information.

- [Porting](../examples/SquareLine/v8/Porting/): This example demonstrates how to port the SquareLine project.
- [WiFiClock](../examples/SquareLine/v8/WiFiClock/): This example implements a simple Wi-Fi clock and can display weather information.

### PlatformIO

- [PlatformIO](../examples/PlatformIO/): This example demonstrates how to use ESP32_Display_Panel in PlatformIO. By default, it is suitable for the **ESP32-S3-LCD-EV-Board** and **ESP32-S3-LCD-EV-Board-2** development boards. Users need to modify the [boards/ESP-LCD.json](../examples/PlatformIO/boards/ESP-LCD.json) file according to the actual situation.

## Other Relevant Instructions

### Configuring Supported Development Boards

For details on how to configure the supported development boards in the Arduino IDE, see [Board_Instructions - Recommended Configurations in the Arduino IDE](../docs/Board_Instructions.md#recommended-configurations-in-the-arduino-ide).

### Configuring LVGL

The functionality and parameters of LVGL can be configured by editing the `lv_conf.h` file, where users can modify macro definitions to update the behavior or default parameters of the driver. Here are some features for configuring LVGL:

1. When using arduino-esp32 v3.x.x version, LVGL will search for the configuration file in the following order: `current project directory` > `Arduino library directory`. If the configuration file is not found, a compilation error indicating the absence of the configuration file will be prompted. Therefore, users need to ensure that at least one directory contains the `lv_conf.h` file.

2. If multiple projects need to use the same configuration, users can place the configuration file in the [Arduino library directory](./FAQ.md#where-is-the-directory-for-arduino-libraries), so that all projects can share the same configuration.

Below are detailed steps for sharing the same LVGL configuration:

1. Navigate to the [Arduino library directory](./FAQ.md#where-is-the-directory-for-arduino-libraries).

2. Enter the `lvgl` folder, copy the `lv_conf_template.h` file, and place the copy at the same level as the `lvgl` folder. Then, rename the copied file to `lv_conf.h`.

3. Finally, the layout of the Arduino library folder should look like this:

   ```
   Arduino
       |-libraries
           |-lv_conf.h
           |-lvgl
           |-other_lib_1
           |-other_lib_2
   ```

4. Open the `lv_conf.h` file, and change the first `#if 0` to `#if 1` to enable the contents of the file.

5. Set other configurations according to requirements. Here are some examples of common configuration options for LVGL v8:

   ```c
   #define LV_COLOR_DEPTH          16  // Typically use 16-bit color depth (RGB565),
                                       // but can also set it to `32` to support 24-bit color depth (RGB888)
   #define LV_COLOR_16_SWAP        0   // If using SPI/QSPI LCD (e.g., ESP32-C3-LCDkit), set this to `1`
   #define LV_COLOR_SCREEN_TRANSP  1
   #define LV_MEM_CUSTOM           1
   #define LV_MEMCPY_MEMSET_STD    1
   #define LV_TICK_CUSTOM          1
   #define LV_ATTRIBUTE_FAST_MEM   IRAM_ATTR
                                      // Get higher performance but use more SRAM
   #define LV_FONT_MONTSERRAT_N    1  // Enable all internal fonts needed (`N` should be replaced with font size)
   ```

6. For more information, please refer to the [LVGL official documentation](https://docs.lvgl.io/8.3/get-started/platforms/arduino.html).

### Porting SquareLine Project

SquareLine Studio (v1.3.x) allows for the rapid design of beautiful UIs through visual editing. If you want to use UI source files exported from SquareLine in the Arduino IDE, you can follow these steps for porting:

1. First, create a new project in SquareLine Studio. Go to `Create` -> `Arduino`, select `Arduino with TFT-eSPI` as the project template, then configure the LCD properties for the target development board in the `PROJECT SETTINGS` section, such as `Resolution` and `Color depth`. Finally, click the `Create` button to create the project.

2. For existing projects, you can also click on `File` -> `Project Settings` in the navigation bar to enter the project settings. Then, in the `BOARD PROPERTIES` section, configure `Board Group` as `Arduino` and `Board` as `Arduino with TFT-eSPI`. Additionally, configure the LCD properties for the target development board in the `DISPLAY PROPERTIES` section. Finally, click the `Save` button to save the project settings.

3. Once the UI design is complete and the export path is configured, click on `Export` -> `Create Template Project` and `Export UI Files` buttons in the menu bar to export the project and UI source files. The layout of the project directory will be as follows:

    ```
    Project
        |-libraries
            |-lv_conf.h
            |-lvgl
            |-readme.txt
            |-TFT_eSPI
            |-ui
        |-README.md
        |-ui
    ```

4. Copy the `lv_conf.h`, `lvgl`, and `ui` folders from the `libraries` folder in the project directory to the Arduino library directory. If you need to use a locally installed `lvgl`, skip copying `lvgl` and `lv_conf.h`, then refer to the steps in the [LVGL Configuration](#configuring-lvgl) section to configure LVGL. The layout of the Arduino library folder will be as follows:

    ```
    Arduino
        |-libraries
            |-ESP32_Display_Panel
            |-esp_panel_conf.h (optional)
            |-lv_conf.h (optional)
            |-lvgl
            |-ui
            |-other_lib_1
            |-other_lib_2
    ```
