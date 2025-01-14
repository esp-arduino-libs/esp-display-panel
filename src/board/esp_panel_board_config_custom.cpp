/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_panel_utils.h"
#include "esp_panel_types.h"
#include "board/esp_panel_board_conf_internal.h"
#include "board/esp_panel_board_config.hpp"
#include "board/esp_panel_board.hpp"
// Replace the following header file if creating a new board configuration
#if ESP_PANEL_BOARD_DEFAULT_USE_CUSTOM
#include "esp_panel_board_config_custom.h"
#endif

// *INDENT-OFF*
#undef _TO_STR
#undef TO_STR
#define _TO_STR(name) #name
#define TO_STR(name) _TO_STR(name)

namespace esp_panel {

#ifdef ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD
static const esp_panel_lcd_vendor_init_cmd_t lcd_vendor_init_cmds[] = ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD();
#endif // ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD

const BoardConfig BOARD_CUSTOM_CONFIG = {

    /* General */
#ifdef ESP_PANEL_BOARD_WIDTH
    .width = ESP_PANEL_BOARD_WIDTH,
#endif
#ifdef ESP_PANEL_BOARD_HEIGHT
    .height = ESP_PANEL_BOARD_HEIGHT,
#endif

    /* LCD */
#if ESP_PANEL_BOARD_USE_LCD
    .lcd = {
    #if ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
        .bus_config = drivers::BusSPI::Config{
            .host_id = ESP_PANEL_BOARD_LCD_SPI_HOST_ID,
            // Host
        #if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
            .host = drivers::BusSPI::Config::HostPartialConfig{
                .mosi_io_num = ESP_PANEL_BOARD_LCD_SPI_IO_MOSI,
                .miso_io_num = ESP_PANEL_BOARD_LCD_SPI_IO_MISO,
                .sclk_io_num = ESP_PANEL_BOARD_LCD_SPI_IO_SCK,
            },
        #endif // ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
            // Control Panel
            .control_panel = drivers::BusSPI::Config::ControlPanelPartialConfig{
                .cs_gpio_num = ESP_PANEL_BOARD_LCD_SPI_IO_CS,
                .dc_gpio_num = ESP_PANEL_BOARD_LCD_SPI_IO_DC,
                .spi_mode = ESP_PANEL_BOARD_LCD_SPI_MODE,
                .pclk_hz = ESP_PANEL_BOARD_LCD_SPI_CLK_HZ,
                .lcd_cmd_bits = ESP_PANEL_BOARD_LCD_SPI_CMD_BITS,
                .lcd_param_bits = ESP_PANEL_BOARD_LCD_SPI_PARAM_BITS,
            },
            // Extra
            .skip_init_host = ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST,
        },
    #elif ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_QSPI
        .bus_config = drivers::BusQSPI::Config{
            .host_id = ESP_PANEL_BOARD_LCD_QSPI_HOST_ID,
        #if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
            // Host
            .host = drivers::BusQSPI::Config::HostPartialConfig{
                .sclk_io_num = ESP_PANEL_BOARD_LCD_QSPI_IO_SCK,
                .data0_io_num = ESP_PANEL_BOARD_LCD_QSPI_IO_DATA0,
                .data1_io_num = ESP_PANEL_BOARD_LCD_QSPI_IO_DATA1,
                .data2_io_num = ESP_PANEL_BOARD_LCD_QSPI_IO_DATA2,
                .data3_io_num = ESP_PANEL_BOARD_LCD_QSPI_IO_DATA3,
            },
        #endif // ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
            // Control Panel
            .control_panel = drivers::BusQSPI::Config::ControlPanelPartialConfig{
                .cs_gpio_num = ESP_PANEL_BOARD_LCD_QSPI_IO_CS,
                .spi_mode = ESP_PANEL_BOARD_LCD_QSPI_MODE,
                .pclk_hz = ESP_PANEL_BOARD_LCD_QSPI_CLK_HZ,
                .lcd_cmd_bits = ESP_PANEL_BOARD_LCD_QSPI_CMD_BITS,
                .lcd_param_bits = ESP_PANEL_BOARD_LCD_QSPI_PARAM_BITS,
            },
            // Extra
            .skip_init_host = ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST,
        },
    #elif (ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB) && SOC_LCD_RGB_SUPPORTED
        .bus_config = drivers::BusRGB::Config{
        #if !ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
            // Control Panel
            .control_panel = drivers::BusRGB::Config::ControlPanelPartialConfig{
                .cs_io_type = ESP_PANEL_BOARD_LCD_RGB_SPI_CS_USE_EXPNADER ? IO_TYPE_EXPANDER : IO_TYPE_GPIO,
                .scl_io_type = ESP_PANEL_BOARD_LCD_RGB_SPI_SCL_USE_EXPNADER ? IO_TYPE_EXPANDER : IO_TYPE_GPIO,
                .sda_io_type = ESP_PANEL_BOARD_LCD_RGB_SPI_SDA_USE_EXPNADER ? IO_TYPE_EXPANDER : IO_TYPE_GPIO,
                .cs_gpio_num = ESP_PANEL_BOARD_LCD_RGB_SPI_CS_USE_EXPNADER ? BIT64(ESP_PANEL_BOARD_LCD_RGB_SPI_IO_CS) :
                                                                        ESP_PANEL_BOARD_LCD_RGB_SPI_IO_CS,
                .scl_gpio_num = ESP_PANEL_BOARD_LCD_RGB_SPI_SCL_USE_EXPNADER ? BIT64(ESP_PANEL_BOARD_LCD_RGB_SPI_IO_SCK) :
                                                                        ESP_PANEL_BOARD_LCD_RGB_SPI_IO_SCK,
                .sda_gpio_num = ESP_PANEL_BOARD_LCD_RGB_SPI_SDA_USE_EXPNADER ? BIT64(ESP_PANEL_BOARD_LCD_RGB_SPI_IO_SDA) :
                                                                        ESP_PANEL_BOARD_LCD_RGB_SPI_IO_SDA,
                .spi_mode = ESP_PANEL_BOARD_LCD_RGB_SPI_MODE,
                .lcd_cmd_bytes = ESP_PANEL_BOARD_LCD_RGB_SPI_CMD_BYTES,
                .lcd_param_bytes = ESP_PANEL_BOARD_LCD_RGB_SPI_PARAM_BYTES,
                .flags_use_dc_bit = ESP_PANEL_BOARD_LCD_RGB_SPI_USE_DC_BIT,
            },
        #endif // ESP_PANEL_BOARD_LCD_BUS_SKIP_INIT_HOST
            // Refresh Panel
            .refresh_panel = drivers::BusRGB::Config::RefreshPanelPartialConfig{
                .pclk_hz = ESP_PANEL_BOARD_LCD_RGB_CLK_HZ,
                .h_res = ESP_PANEL_BOARD_WIDTH,
                .v_res = ESP_PANEL_BOARD_HEIGHT,
                .hsync_pulse_width = ESP_PANEL_BOARD_LCD_RGB_HPW,
                .hsync_back_porch = ESP_PANEL_BOARD_LCD_RGB_HBP,
                .hsync_front_porch = ESP_PANEL_BOARD_LCD_RGB_HFP,
                .vsync_pulse_width = ESP_PANEL_BOARD_LCD_RGB_VPW,
                .vsync_back_porch = ESP_PANEL_BOARD_LCD_RGB_VBP,
                .vsync_front_porch = ESP_PANEL_BOARD_LCD_RGB_VFP,
                .data_width = ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH,
                .bits_per_pixel = ESP_PANEL_BOARD_LCD_RGB_PIXEL_BITS,
                .bounce_buffer_size_px = ESP_PANEL_BOARD_LCD_RGB_BOUNCE_BUF_SIZE,
                .hsync_gpio_num = ESP_PANEL_BOARD_LCD_RGB_IO_HSYNC,
                .vsync_gpio_num = ESP_PANEL_BOARD_LCD_RGB_IO_VSYNC,
                .de_gpio_num = ESP_PANEL_BOARD_LCD_RGB_IO_DE,
                .pclk_gpio_num = ESP_PANEL_BOARD_LCD_RGB_IO_PCLK,
                .disp_gpio_num = ESP_PANEL_BOARD_LCD_RGB_IO_DISP,
                .data_gpio_nums = {
                    ESP_PANEL_BOARD_LCD_RGB_IO_DATA0, ESP_PANEL_BOARD_LCD_RGB_IO_DATA1, ESP_PANEL_BOARD_LCD_RGB_IO_DATA2,
                    ESP_PANEL_BOARD_LCD_RGB_IO_DATA3, ESP_PANEL_BOARD_LCD_RGB_IO_DATA4, ESP_PANEL_BOARD_LCD_RGB_IO_DATA5,
                    ESP_PANEL_BOARD_LCD_RGB_IO_DATA6, ESP_PANEL_BOARD_LCD_RGB_IO_DATA7,
            #if ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH > 8
                    ESP_PANEL_BOARD_LCD_RGB_IO_DATA8, ESP_PANEL_BOARD_LCD_RGB_IO_DATA9, ESP_PANEL_BOARD_LCD_RGB_IO_DATA10,
                    ESP_PANEL_BOARD_LCD_RGB_IO_DATA11, ESP_PANEL_BOARD_LCD_RGB_IO_DATA12, ESP_PANEL_BOARD_LCD_RGB_IO_DATA13,
                    ESP_PANEL_BOARD_LCD_RGB_IO_DATA14, ESP_PANEL_BOARD_LCD_RGB_IO_DATA15,
            #endif // ESP_PANEL_BOARD_LCD_RGB_DATA_WIDTH
                },
                .flags_pclk_active_neg = ESP_PANEL_BOARD_LCD_RGB_PCLK_ACTIVE_NEG,
            },
            // Extra
            .use_control_panel = !ESP_PANEL_BOARD_LCD_RGB_USE_CONTROL_PANEL,
        },
    #elif (ESP_PANEL_BOARD_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_MIPI_DSI) && SOC_MIPI_DSI_SUPPORTED
        .bus_config = drivers::BusDSI::Config{
            // Host
            .host = drivers::BusDSI::Config::HostPartialConfig{
                .num_data_lanes = ESP_PANEL_BOARD_LCD_MIPI_DSI_LANE_NUM,
                .lane_bit_rate_mbps = ESP_PANEL_BOARD_LCD_MIPI_DSI_LANE_RATE_MBPS,
            },
            // Panel
            .refresh_panel = drivers::BusDSI::Config::RefreshPanelPartialConfig{
                .dpi_clock_freq_mhz = ESP_PANEL_BOARD_LCD_MIPI_DPI_CLK_MHZ,
                .bits_per_pixel = ESP_PANEL_BOARD_LCD_MIPI_DPI_PIXEL_BITS,
                .h_size = ESP_PANEL_BOARD_WIDTH,
                .v_size = ESP_PANEL_BOARD_HEIGHT,
                .hsync_pulse_width = ESP_PANEL_BOARD_LCD_MIPI_DPI_HPW,
                .hsync_back_porch = ESP_PANEL_BOARD_LCD_MIPI_DPI_HBP,
                .hsync_front_porch = ESP_PANEL_BOARD_LCD_MIPI_DPI_HFP,
                .vsync_pulse_width = ESP_PANEL_BOARD_LCD_MIPI_DPI_VPW,
                .vsync_back_porch = ESP_PANEL_BOARD_LCD_MIPI_DPI_VBP,
                .vsync_front_porch = ESP_PANEL_BOARD_LCD_MIPI_DPI_VFP,
            },
            // PHY LDO
            .phy_ldo = drivers::BusDSI::Config::PHY_LDO_PartialConfig{
                .chan_id = ESP_PANEL_BOARD_LCD_MIPI_PHY_LDO_ID
            },
        },
    #endif // ESP_PANEL_BOARD_LCD_BUS_TYPE
        .device_name = TO_STR(ESP_PANEL_BOARD_LCD_CONTROLLER),
        .device_config = {
            // Device
            .device = drivers::LCD::Config::DevicePartialConfig{
                .reset_gpio_num = ESP_PANEL_BOARD_LCD_RST_IO,
                .rgb_ele_order = ESP_PANEL_BOARD_LCD_COLOR_BGR_ORDER,
                .bits_per_pixel = ESP_PANEL_BOARD_LCD_COLOR_BITS,
                .flags_reset_active_high = ESP_PANEL_BOARD_LCD_RST_LEVEL,
            },
            // Vendor
            .vendor = drivers::LCD::Config::VendorPartialConfig{
    #ifdef ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD
                .init_cmds = lcd_vendor_init_cmds,
                .init_cmds_size = sizeof(lcd_vendor_init_cmds) / sizeof(lcd_vendor_init_cmds[0]),
    #endif // ESP_PANEL_BOARD_LCD_VENDOR_INIT_CMD
    #ifdef ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD
                .flags_mirror_by_cmd = ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD,
    #endif // ESP_PANEL_BOARD_LCD_FLAGS_MIRROR_BY_CMD
    #ifdef ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX
                .flags_enable_io_multiplex = ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX,
    #endif // ESP_PANEL_BOARD_LCD_FLAGS_ENABLE_IO_MULTIPLEX
            },
        },
        .pre_process = {
            .invert_color = ESP_PANEL_BOARD_LCD_INEVRT_COLOR,
    #ifdef ESP_PANEL_BOARD_LCD_SWAP_XY
            .swap_xy = ESP_PANEL_BOARD_LCD_SWAP_XY,
    #endif // ESP_PANEL_BOARD_LCD_SWAP_XY
    #ifdef ESP_PANEL_BOARD_LCD_MIRROR_X
            .mirror_x = ESP_PANEL_BOARD_LCD_MIRROR_X,
    #endif // ESP_PANEL_BOARD_LCD_MIRROR_X
    #ifdef ESP_PANEL_BOARD_LCD_MIRROR_Y
            .mirror_y = ESP_PANEL_BOARD_LCD_MIRROR_Y,
    #endif // ESP_PANEL_BOARD_LCD_MIRROR_Y
        },
    },
#endif // ESP_PANEL_BOARD_USE_LCD

    /* Touch */
#if ESP_PANEL_BOARD_USE_TOUCH
    .touch = {
    #if ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_I2C
        .bus_config = drivers::BusI2C::Config{
            // General
            .host_id = ESP_PANEL_BOARD_TOUCH_I2C_HOST_ID,
            .skip_init_host = ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST,
            // Host
        #if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
            .host = drivers::BusI2C::Config::HostPartialConfig{
                .sda_io_num = ESP_PANEL_BOARD_TOUCH_I2C_IO_SDA,
                .scl_io_num = ESP_PANEL_BOARD_TOUCH_I2C_IO_SCL,
                .sda_pullup_en = ESP_PANEL_BOARD_TOUCH_I2C_SDA_PULLUP,
                .scl_pullup_en = ESP_PANEL_BOARD_TOUCH_I2C_SCL_PULLUP,
                .clk_speed = ESP_PANEL_BOARD_TOUCH_I2C_CLK_HZ,
            },
        #endif // ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
            // Control Panel
        #if ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS == 0
            .control_panel = drivers::BusI2C::Config::ControlPanelFullConfig
                ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG(ESP_PANEL_BOARD_TOUCH_CONTROLLER),
        #else
            .control_panel = drivers::BusI2C::Config::ControlPanelFullConfig
                ESP_PANEL_TOUCH_I2C_PANEL_IO_CONFIG_WITH_ADDR(
                    ESP_PANEL_BOARD_TOUCH_CONTROLLER, ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS
                ),
        #endif // ESP_PANEL_BOARD_TOUCH_I2C_ADDRESS
        },
    #elif ESP_PANEL_BOARD_TOUCH_BUS_TYPE == ESP_PANEL_BUS_TYPE_SPI
        .bus_config = drivers::BusSPI::Config{
            .host_id = ESP_PANEL_BOARD_TOUCH_SPI_HOST_ID,
            // Host
        #if !ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
            .mosi_io_num = ESP_PANEL_BOARD_TOUCH_SPI_IO_MOSI,
            .miso_io_num = ESP_PANEL_BOARD_TOUCH_SPI_IO_MISO,
            .sclk_io_num = ESP_PANEL_BOARD_TOUCH_SPI_IO_SCK,
        #endif // ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST
            // Control Panel
            .control_panel = ESP_PANEL_TOUCH_SPI_PANEL_IO_CONFIG(
                ESP_PANEL_BOARD_TOUCH_CONTROLLER, ESP_PANEL_BOARD_TOUCH_SPI_IO_CS
            ),
            // Extra
            .skip_init_host = ESP_PANEL_BOARD_TOUCH_BUS_SKIP_INIT_HOST,
            .use_complete_io_config = true,
        },
    #endif // ESP_PANEL_BOARD_TOUCH_BUS_TYPE
        .device_name = TO_STR(ESP_PANEL_BOARD_TOUCH_CONTROLLER),
        .device_config = {
            .device = drivers::Touch::Config::DevicePartialConfig{
                .x_max = ESP_PANEL_BOARD_WIDTH,
                .y_max = ESP_PANEL_BOARD_HEIGHT,
                .rst_gpio_num = ESP_PANEL_BOARD_TOUCH_RST_IO,
                .int_gpio_num = ESP_PANEL_BOARD_TOUCH_INT_IO,
                .levels_reset = ESP_PANEL_BOARD_TOUCH_RST_LEVEL,
                .levels_interrupt = ESP_PANEL_BOARD_TOUCH_INT_LEVEL,
            },
        },
        .pre_process = {
    #ifdef ESP_PANEL_BOARD_TOUCH_SWAP_XY
            .swap_xy = ESP_PANEL_BOARD_TOUCH_SWAP_XY,
    #endif // ESP_PANEL_BOARD_TOUCH_SWAP_XY
    #ifdef ESP_PANEL_BOARD_TOUCH_MIRROR_X
            .mirror_x = ESP_PANEL_BOARD_TOUCH_MIRROR_X,
    #endif // ESP_PANEL_BOARD_TOUCH_MIRROR_X
    #ifdef ESP_PANEL_BOARD_TOUCH_MIRROR_Y
            .mirror_y = ESP_PANEL_BOARD_TOUCH_MIRROR_Y,
    #endif // ESP_PANEL_BOARD_TOUCH_MIRROR_Y
        },
    },
#endif // ESP_PANEL_BOARD_USE_TOUCH

    /* Backlight */
#if ESP_PANEL_BOARD_USE_BACKLIGHT
    .backlight = {
    #if ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_SWITCH_GPIO
        .config = drivers::BacklightSwitchGPIO::Config{
            .io_num = ESP_PANEL_BOARD_BACKLIGHT_IO,
            .on_level = ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL,
        },
    #elif ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_SWITCH_EXPANDER
        .config = drivers::BacklightSwitchExpander::Config{
            .io_num = ESP_PANEL_BOARD_BACKLIGHT_IO,
            .on_level = ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL,
        },
    #elif ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC
        .config = drivers::BacklightPWM_LEDC::Config{
            .ledc_channel = drivers::BacklightPWM_LEDC::Config::LEDC_ChannelPartialConfig{
                .io_num = ESP_PANEL_BOARD_BACKLIGHT_IO,
                .on_level = ESP_PANEL_BOARD_BACKLIGHT_ON_LEVEL,
            },
        },
    #elif ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_CUSTOM
        .config = drivers::BacklightCustom::Config{
            .callback = [](int percent, void *user_data)
                ESP_PANEL_BOARD_BACKLIGHT_CUSTOM_FUNCTION(percent, user_data),
            .user_data = nullptr,
        },
    #endif // ESP_PANEL_BOARD_BACKLIGHT_TYPE
        .pre_process = {
            .idle_off = ESP_PANEL_BOARD_BACKLIGHT_IDLE_OFF,
        },
    },
#endif // ESP_PANEL_BOARD_USE_BACKLIGHT

    /* IO expander */
#if ESP_PANEL_BOARD_USE_EXPANDER
    .io_expander = {
        .name = TO_STR(ESP_PANEL_BOARD_EXPANDER_CHIP),
        .config = {
    #if !ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST
            // Host
            .host_id = ESP_PANEL_BOARD_EXPANDER_I2C_HOST_ID,
            .host = drivers::IO_Expander::Config::HostPartialConfig{
                .sda_io_num = ESP_PANEL_BOARD_EXPANDER_I2C_IO_SDA,
                .scl_io_num = ESP_PANEL_BOARD_EXPANDER_I2C_IO_SCL,
                .sda_pullup_en = ESP_PANEL_BOARD_EXPANDER_I2C_SDA_PULLUP,
                .scl_pullup_en = ESP_PANEL_BOARD_EXPANDER_I2C_SCL_PULLUP,
                .clk_speed = ESP_PANEL_BOARD_EXPANDER_I2C_CLK_HZ,
            },
    #endif // ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST
            // Device
            .device_address = ESP_PANEL_BOARD_EXPANDER_I2C_ADDRESS,
            // Extra
            .skip_init_host = ESP_PANEL_BOARD_EXPANDER_SKIP_INIT_HOST,
        },
    },
#endif // ESP_PANEL_BOARD_USE_EXPANDER

    /* Others */
    .callbacks = {
#ifdef ESP_PANEL_BOARD_PRE_BEGIN_FUNCTION
        .pre_board_begin = [](void *p) ESP_PANEL_BOARD_PRE_BEGIN_FUNCTION(p),
#endif // ESP_PANEL_BOARD_PRE_BEGIN_FUNCTION
#ifdef ESP_PANEL_BOARD_POST_BEGIN_FUNCTION
        .post_board_begin = [](void *p) ESP_PANEL_BOARD_POST_BEGIN_FUNCTION(p),
#endif // ESP_PANEL_BOARD_POST_BEGIN_FUNCTION
#ifdef ESP_PANEL_BOARD_EXPANDER_PRE_BEGIN_FUNCTION
        .pre_expander_begin = [](void *p) ESP_PANEL_BOARD_EXPANDER_PRE_BEGIN_FUNCTION(p),
#endif // ESP_PANEL_BOARD_EXPANDER_PRE_BEGIN_FUNCTION
#ifdef ESP_PANEL_BOARD_EXPANDER_POST_BEGIN_FUNCTION
        .post_expander_begin = [](void *p) ESP_PANEL_BOARD_EXPANDER_POST_BEGIN_FUNCTION(p),
#endif // ESP_PANEL_BOARD_EXPANDER_POST_BEGIN_FUNCTION
#ifdef ESP_PANEL_BOARD_LCD_PRE_BEGIN_FUNCTION
        .pre_lcd_begin = [](void *p) ESP_PANEL_BOARD_LCD_PRE_BEGIN_FUNCTION(p),
#endif // ESP_PANEL_BOARD_LCD_PRE_BEGIN_FUNCTION
#ifdef ESP_PANEL_BOARD_LCD_POST_BEGIN_FUNCTION
        .post_lcd_begin = [](void *p) ESP_PANEL_BOARD_LCD_POST_BEGIN_FUNCTION(p),
#endif // ESP_PANEL_BOARD_LCD_POST_BEGIN_FUNCTION
#ifdef ESP_PANEL_BOARD_TOUCH_PRE_BEGIN_FUNCTION
        .pre_touch_begin = [](void *p) ESP_PANEL_BOARD_TOUCH_PRE_BEGIN_FUNCTION(p),
#endif // ESP_PANEL_BOARD_TOUCH_PRE_BEGIN_FUNCTION
#ifdef ESP_PANEL_BOARD_TOUCH_POST_BEGIN_FUNCTION
        .post_touch_begin = [](void *p) ESP_PANEL_BOARD_TOUCH_POST_BEGIN_FUNCTION(p),
#endif // ESP_PANEL_BOARD_TOUCH_POST_BEGIN_FUNCTION
#ifdef ESP_PANEL_BOARD_BACKLIGHT_PRE_BEGIN_FUNCTION
        .pre_backlight_begin = [](void *p) ESP_PANEL_BOARD_BACKLIGHT_PRE_BEGIN_FUNCTION(p),
#endif // ESP_PANEL_BOARD_BACKLIGHT_PRE_BEGIN_FUNCTION
#ifdef ESP_PANEL_BOARD_BACKLIGHT_POST_BEGIN_FUNCTION
        .post_backlight_begin = [](void *p) ESP_PANEL_BOARD_BACKLIGHT_POST_BEGIN_FUNCTION(p),
#endif // ESP_PANEL_BOARD_BACKLIGHT_POST_BEGIN_FUNCTION
    },
    .flags = {
#ifdef ESP_PANEL_BOARD_USE_LCD
        .use_lcd = ESP_PANEL_BOARD_USE_LCD,
#endif
#ifdef ESP_PANEL_BOARD_USE_TOUCH
        .use_touch = ESP_PANEL_BOARD_USE_TOUCH,
#endif
#ifdef ESP_PANEL_BOARD_USE_BACKLIGHT
        .use_backlight = ESP_PANEL_BOARD_USE_BACKLIGHT,
#endif
#ifdef ESP_PANEL_BOARD_USE_EXPANDER
        .use_io_expander = ESP_PANEL_BOARD_USE_EXPANDER,
#endif
    },
};

} // namespace esp_panel
// *INDENT-ON*
