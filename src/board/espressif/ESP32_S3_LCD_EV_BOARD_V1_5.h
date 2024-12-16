/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// *INDENT-OFF*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the LCD panel /////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using an LCD panel */
#define ESP_PANEL_BOARD_DEFAULT_USE_LCD           (1)     // 0/1

#if ESP_PANEL_BOARD_DEFAULT_USE_LCD
/**
 * LCD Controller Name.
 */
#define ESP_PANEL_BOARD_LCD_CONTROLLER          GC9503

/* LCD resolution in pixels */
#define ESP_PANEL_BOARD_WIDTH         (480)
#define ESP_PANEL_BOARD_HEIGHT        (480)

/* LCD Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST    (0)     // 0/1
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

    #define ESP_PANEL_BOARD_LCD_RGB_CLK_HZ            (16 * 1000 * 1000)
    #define ESP_PANEL_BOARD_LCD_RGB_HPW               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_HBP               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_HFP               (20)
    #define ESP_PANEL_BOARD_LCD_RGB_VPW               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_VBP               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_VFP               (10)
    #define ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG   (0)     // 0: rising edge, 1: falling edge
    #define ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH        (16)    //  8 | 16
    #define ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS        (16)    // 24 | 16

    #define ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE   (ESP_PANEL_BOARD_WIDTH * 10)
                                                        // Bounce buffer size in bytes. This function is used to avoid screen drift.
                                                        // To enable the bounce buffer, set it to a non-zero value.
                                                        // Typically set to `ESP_PANEL_BOARD_WIDTH * 10`
    #define ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC          (46)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC          (3)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DE             (17)    // -1 if not used
    #define ESP_PANEL_BOARD_LCD_RGB_IO_PCLK           (9)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DISP           (-1)    // -1 if not used
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA0          (10)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA1          (11)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA2          (12)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA3          (13)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA4          (14)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA5          (21)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA6          (8)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA7          (18)
#if ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH > 8
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA8          (45)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA9          (38)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA10         (39)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA11         (40)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA12         (41)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA13         (42)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA14         (2)
    #define ESP_PANEL_BOARD_LCD_RGB_IO_DATA15         (1)
#endif
#if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_LCD_SPI_IO_CS               (1)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SCK              (2)
    #define ESP_PANEL_BOARD_LCD_SPI_IO_SDA              (3)
    #define ESP_PANEL_BOARD_LCD_SPI_CS_USE_EXPNADER     (1)     // 0/1
    #define ESP_PANEL_BOARD_LCD_SPI_SCL_USE_EXPNADER    (1)     // 0/1
    #define ESP_PANEL_BOARD_LCD_SPI_SDA_USE_EXPNADER    (1)     // 0/1
    #define ESP_PANEL_BOARD_LCD_SPI_SCL_ACTIVE_EDGE     (0)     // 0: rising edge, 1: falling edge
    #define ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX       (1)     // Delete the control panel instance automatically if set to 1.
                                                                // If the 3-wire SPI pins are sharing other pins of the RGB
                                                                // interface to save GPIOs, Please set it to 1 to release
                                                                // the control panel and its pins (except CS signal).
    #define ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD           (!ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX)
                                                                // The `mirror()` function will be implemented by LCD
                                                                // command if set to 1.
#endif

#endif /* ESP_PANEL_BOARD_LCD_BUS_TYPE */

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
/*
#define ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD()                                        \
    {                                                                          \
        {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},              \
        {0xC0, (uint8_t []){0x3B, 0x00}, 2, 0},                                \
        {0xC1, (uint8_t []){0x0D, 0x02}, 2, 0},                                \
        {0x29, (uint8_t []){0x00}, 0, 120},                                    \
        or                                                                     \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}), \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x3B, 0x00}),                   \
        ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x0D, 0x02}),                   \
        ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x29),                               \
    }
*/

/* LCD Color Settings */
/* LCD color depth in bits */
#define ESP_PANEL_BOARD_LCD_COLOR_BITS    (18)        // 8/16/18/24
/*
 * LCD RGB Element Order. Choose one of the following:
 *      - 0: RGB
 *      - 1: BGR
 */
#define ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER     (0)         // 0/1
#define ESP_PANEL_BOARD_LCD_INEVRT_COLOR  (0)         // 0/1

/* LCD Transformation Flags */
#define ESP_PANEL_BOARD_LCD_SWAP_XY       (0)         // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_X      (0)         // 0/1
#define ESP_PANEL_BOARD_LCD_MIRROR_Y      (0)         // 0/1

/* LCD Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_BOARD_LCD_RST_IO        (-1)
#define ESP_PANEL_BOARD_LCD_RST_LEVEL     (0)         // 0: low level, 1: high level

#endif /* ESP_PANEL_BOARD_DEFAULT_USE_LCD */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Please update the following macros to configure the touch panel ///////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 1 when using an touch panel */
#define ESP_PANEL_BOARD_DEFAULT_USE_TOUCH         (1)         // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_TOUCH
/**
 * Touch controller name.
 */
#define ESP_PANEL_BOARD_TOUCH_CONTROLLER        FT5x06

/* Touch resolution in pixels */
#define ESP_PANEL_TOUCH_H_RES       (480)   // Typically set to the same value as the width of LCD
#define ESP_PANEL_TOUCH_V_RES       (480)  // Typically set to the same value as the height of LCD

/* Touch Panel Bus Settings */
/**
 * If set to 1, the bus will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST      (0)     // 0/1
/**
 * Touch panel bus type.
 */
#define ESP_PANEL_BOARD_TOUCH_BUS_TYPE            (ESP_PANEL_BUS_TYPE_I2C)
/* Touch panel bus parameters */
#if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C

    #define ESP_PANEL_BOARD_TOUCH_BUS_HOST_ID     (0)     // Typically set to 0
    #define ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS     (0)     // Typically set to 0 to use default address
#if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ      (400 * 1000)
                                                    // Typically set to 400K
    #define ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP  (0)     // 0/1
    #define ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP  (0)     // 0/1
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL      (48)
    #define ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA      (47)
#endif

#endif

/* Touch Transformation Flags */
#define ESP_PANEL_BOARD_TOUCH_SWAP_XY         (0)         // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_X        (0)         // 0/1
#define ESP_PANEL_BOARD_TOUCH_MIRROR_Y        (0)         // 0/1

/* Touch Other Settings */
/* IO num of RESET pin, set to -1 if not use */
#define ESP_PANEL_BOARD_TOUCH_RST_IO          (-1)
#define ESP_PANEL_BOARD_TOUCH_RST_LEVEL       (0)         // 0: low level, 1: high level
/* IO num of INT pin, set to -1 if not use */
#define ESP_PANEL_BOARD_TOUCH_INT_IO          (-1)
#define ESP_PANEL_BOARD_TOUCH_INT_LEVEL       (0)         // 0: low level, 1: high level

#endif /* ESP_PANEL_BOARD_DEFAULT_USE_TOUCH */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the backlight ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT         (0)         // 0/1
#define ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT         (1)         // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT
    /**
     * Backlight control mode. Choose one of the following:
     *  - ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO: Use GPIO switch to control the backlight, only support on/off
     *  - ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC:    Use LEDC PWM to control the backlight, support brightness adjustment
     *  - ESP_PANEL_BACKLIGHT_TYPE_CUSTOM:      Use custom function to control the backlight
     */
    #define ESP_PANEL_BOARD_BACKLIGHT_TYPE        (ESP_PANEL_BACKLIGHT_TYPE_CUSTOM)

    #if ESP_PANEL_BOARD_BACKLIGHT_TYPE != ESP_PANEL_BACKLIGHT_TYPE_CUSTOM

        /* IO num of backlight pin */
        #define ESP_PANEL_BOARD_BACKLIGHT_IO          (38)
        #define ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL    (1)         // 0: low level, 1: high level

    #else

        /**
         * @brief Custom backlight control function
         *
         * @param[in] percent   Brightness percentage, 0-100
         * @param[in] user_data User data, default is a pointer of `Board`
         *
         * @return true if successful, otherwise false
         *
         */
        #define ESP_PANEL_BOARD_BACKLIGHT_CUSTOM_FUNCTION( percent, user_data )  \
            {  \
                esp_panel::Board *board = static_cast<esp_panel::Board *>(user_data);  \
                ESP_UTILS_LOGI("Run custom backlight control function (%d)", percent);  \
                return true; \
            }

    #endif // ESP_PANEL_BOARD_BACKLIGHT_TYPE

    /* Set to 1 if you want to turn off the backlight after initializing the panel; otherwise, set it to turn on */
    #define ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF    (0)         // 0: on, 1: off
#endif // ESP_PANEL_BOARD_DEFAULT_USE_BACKLIGHT

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please update the following macros to configure the IO expander //////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set to 0 if not using IO Expander */
#define ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER          (1)         // 0/1
#if ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER
/**
 * IO expander name.
 */
#define ESP_PANEL_BOARD_EXPANDER_CHIP         TCA95XX_8BIT

/* IO expander Settings */
/**
 * If set to 1, the driver will skip to initialize the corresponding host. Users need to initialize the host in advance.
 * It is useful if other devices use the same host. Please ensure that the host is initialized only once.
 */
#define ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST       (0)     // 0/1
/* IO expander parameters */
#define ESP_PANEL_BOARD_EXPANDER_I2C_ADDRESS          (0x20)
#if !ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST
    #define ESP_PANEL_BOARD_EXPANDER_HOST_ID          (0)     // Typically set to 0
    #define ESP_PANEL_BOARD_EXPANDER_I2C_CLK_HZ       (400 * 1000)
                                                        // Typically set to 400K
    #define ESP_PANEL_BOARD_EXPANDER_I2C_SCL_PULLUP   (0)     // 0/1
    #define ESP_PANEL_BOARD_EXPANDER_I2C_SDA_PULLUP   (0)     // 0/1
    #define ESP_PANEL_BOARD_EXPANDER_I2C_IO_SCL       (48)
    #define ESP_PANEL_BOARD_EXPANDER_I2C_IO_SDA       (47)
#endif
#endif /* ESP_PANEL_BOARD_DEFAULT_USE_EXPANDER */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// Please utilize the following macros to execute any additional code if required. //////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ESP_PANEL_BOARD_LCD_PRE_BEGIN_FUNCTION( p ) \
    {  \
        ESP_UTILS_LOGD("Run ESP32_S3_LCD_EV_BOARD_V1_5 LCD pre-begin function");  \
        /* For the v1.5 version sub board, need to set `ESP_PANEL_LCD_RGB_IO_VSYNC` to high before initialize LCD */ \
        gpio_set_direction((gpio_num_t)ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC, GPIO_MODE_OUTPUT); \
        gpio_set_level((gpio_num_t)ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC, 0); \
        vTaskDelay(pdMS_TO_TICKS(10)); \
        gpio_set_level((gpio_num_t)ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC, 1); \
        vTaskDelay(pdMS_TO_TICKS(120)); \
        return true;    \
    }

#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MAJOR 1
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_MINOR 0
#define ESP_PANEL_BOARD_CUSTOM_FILE_VERSION_PATCH 0

// *INDENT-OFF*
