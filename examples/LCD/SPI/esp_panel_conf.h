/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// Driver: LCD Configurations ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * The following configurations are used to enable the LCD drivers which are used in the board. Disable useless LCDs to
 * reduce the code size.
 *
 */
#define ESP_PANEL_CONF_LCD_ENABLE_AXS15231B     (0)
#define ESP_PANEL_CONF_LCD_ENABLE_EK9716B       (0)
#define ESP_PANEL_CONF_LCD_ENABLE_EK79007       (0)
#define ESP_PANEL_CONF_LCD_ENABLE_GC9A01        (0)
#define ESP_PANEL_CONF_LCD_ENABLE_GC9B71        (0)
#define ESP_PANEL_CONF_LCD_ENABLE_GC9503        (0)
#define ESP_PANEL_CONF_LCD_ENABLE_HX8399        (0)
#define ESP_PANEL_CONF_LCD_ENABLE_ILI9341       (0)
#define ESP_PANEL_CONF_LCD_ENABLE_ILI9881C      (0)
#define ESP_PANEL_CONF_LCD_ENABLE_JD9165        (0)
#define ESP_PANEL_CONF_LCD_ENABLE_JD9365        (0)
#define ESP_PANEL_CONF_LCD_ENABLE_NV3022B       (0)
#define ESP_PANEL_CONF_LCD_ENABLE_SH8601        (0)
#define ESP_PANEL_CONF_LCD_ENABLE_SPD2010       (0)
#define ESP_PANEL_CONF_LCD_ENABLE_ST7262        (0)
#define ESP_PANEL_CONF_LCD_ENABLE_ST7701        (0)
#define ESP_PANEL_CONF_LCD_ENABLE_ST7703        (0)
#define ESP_PANEL_CONF_LCD_ENABLE_ST7789        (0)
#define ESP_PANEL_CONF_LCD_ENABLE_ST7796        (0)
#define ESP_PANEL_CONF_LCD_ENABLE_ST77903       (0)
#define ESP_PANEL_CONF_LCD_ENABLE_ST77916       (0)
#define ESP_PANEL_CONF_LCD_ENABLE_ST77922       (0)

/**
 * If set to 0, the code of disabled LCD drivers will be shielded to speed up compilation.
 *
 * Take ILI9341 as an example, there are four situations with `ESP_PANEL_CONF_LCD_ENABLE_ILI9341` (CONF1) and
 * `ESP_PANEL_CONF_LCD_COMPILE_DISABLED` (CONF2):
 *
 *      | CONF1 | CONF2 | Is `esp_panel::LCD_ILI9341` available? | Can `esp_panel::Board` work with ILI9341? |
 *      |-------|-------|---------------------------------------|--------------------------------------------|
 *      |   0   |   0   |                   No                  |                   No                       |
 *      |   1   |   0   |                   Yes                 |                   Yes                      |
 *      |   0   |   1   |                   Yes                 |                   No                       |
 *      |   1   |   1   |                   Yes                 |                   Yes                      |
 *
 */
#define ESP_PANEL_CONF_LCD_COMPILE_DISABLED     (1)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// Driver: Touch Configurations //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Maximum point number */
#define ESP_PANEL_CONF_TOUCH_MAX_POINTS         (5)
/* Maximum button number */
#define ESP_PANEL_CONF_TOUCH_MAX_BUTTONS        (1)

/**
 * The following configurations are used to enable the touch drivers which are used in the board. Default disable them
 * to reduce the code size.
 *
 */
#define ESP_PANEL_CONF_TOUCH_ENABLE_AXS15231B   (0)
#define ESP_PANEL_CONF_TOUCH_ENABLE_CST816S     (0)
#define ESP_PANEL_CONF_TOUCH_ENABLE_FT5x06      (0)
#define ESP_PANEL_CONF_TOUCH_ENABLE_GT911       (0)
#define ESP_PANEL_CONF_TOUCH_ENABLE_GT1151      (0)
#define ESP_PANEL_CONF_TOUCH_ENABLE_SPD2010     (0)
#define ESP_PANEL_CONF_TOUCH_ENABLE_ST1633      (0)
#define ESP_PANEL_CONF_TOUCH_ENABLE_ST7123      (0)
#define ESP_PANEL_CONF_TOUCH_ENABLE_STMPE610    (0)
#define ESP_PANEL_CONF_TOUCH_ENABLE_TT21100     (0)
#define ESP_PANEL_CONF_TOUCH_ENABLE_XPT2046     (0)

/**
 * If set to 0, the code of disabled touch drivers will be shielded to speed up compilation.
 *
 * Take GT911 as an example, there are four situations with `ESP_PANEL_CONF_TOUCH_ENABLE_GT911` (CONF1) and
 * `ESP_PANEL_CONF_TOUCH_COMPILE_DISABLED` (CONF2):
 *
 *      | CONF1 | CONF2 | Is `esp_panel::LCD_GT911` available? | Can `esp_panel::Board` work with GT911? |
 *      |-------|-------|--------------------------------------|-----------------------------------------|
 *      |   0   |   0   |                  No                  |                 No                      |
 *      |   1   |   0   |                  Yes                 |                 Yes                     |
 *      |   0   |   1   |                  Yes                 |                 No                      |
 *      |   1   |   1   |                  Yes                 |                 Yes                     |
 *
 */
#define ESP_PANEL_CONF_TOUCH_COMPILE_DISABLED   (1)

#if ESP_PANEL_CONF_TOUCH_ENABLE_XPT2046 || ESP_PANEL_CONF_TOUCH_COMPILE_DISABLED
    /**
     * Minimum Z pressure threshold
     *
     */
    #define ESP_PANEL_CONF_TOUCH_XPT2046_Z_THRESHOLD            (400)

    /**
     * Enable Interrupt (PENIRQ) output, also called Full Power Mode.
     * Enable this to configure the XPT2046 to output low on the PENIRQ output if a touch is detected.
     * This mode uses more power when enabled. Note that this signal goes low normally when a read is active.
     *
     */
    #define ESP_PANEL_CONF_TOUCH_XPT2046_INTERRUPT_MODE         (0)     // 0/1

    /**
     * Keep internal Vref enabled.
     * Enable this to keep the internal Vref enabled between conversions. This uses slightly more power,
     * but requires fewer transactions when reading the battery voltage, aux voltage and temperature.
     *
     */
    #define ESP_PANEL_CONF_TOUCH_XPT2046_VREF_ON_MODE           (0)     // 0/1

    /**
     * Convert touch coordinates to screen coordinates.
     * When this option is enabled the raw ADC values will be converted from 0-4096 to 0-{screen width} or 0-{screen height}.
     * When this option is disabled the process_coordinates method will need to be used to convert the raw ADC values into a
     * screen coordinate.
     *
     */
    #define ESP_PANEL_CONF_TOUCH_XPT2046_CONVERT_ADC_TO_COORDS  (1)     // 0/1

    /**
     * Enable data structure locking.
     * By enabling this option the XPT2046 driver will lock the touch position data structures when reading values from the
     * XPT2046 and when reading position data via API.
     * WARNING: enabling this option may result in unintended crashes.
     *
     */
    #define ESP_PANEL_CONF_TOUCH_XPT2046_ENABLE_LOCKING         (0)     // 0/1
#endif // ESP_PANEL_TOUCH_ENABLE_XPT2046

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// File Version ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Do not change the following versions, they are used to check if the configurations in this file are compatible with
 * the current version of `esp_panel_conf.h` in the library. The detailed rules are as follows:
 *
 *   1. If the major version is not consistent, then the configurations in this file are incompatible with the library
 *      and must be replaced with the file from the library.
 *   2. If the minor version is not consistent, this file might be missing some new configurations, which will be set to
 *      default values. It is recommended to replace it with the file from the library.
 *   3. Even if the patch version is not consistent, it will not affect normal functionality.
 *
 */
#define ESP_PANEL_CONF_FILE_VERSION_MAJOR 1
#define ESP_PANEL_CONF_FILE_VERSION_MINOR 0
#define ESP_PANEL_CONF_FILE_VERSION_PATCH 0

// *INDENT-OFF*
