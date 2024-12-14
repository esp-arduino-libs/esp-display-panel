/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "soc/soc_caps.h"
#if SOC_LCD_RGB_SUPPORTED
#include <memory>
#include "esp_lcd_panel_rgb.h"
#include "esp_io_expander.hpp"
#include "esp_panel_types.h"
#include "port/esp_lcd_panel_io_additions.h"
#include "esp_panel_bus_base.hpp"

/**
 * For the ESP32-S3, the RGB peripheral only supports 16-bit RGB565.and 8-bit RGB888 color formats. For more details,
 * please refer to the part `Display > LCD Screen > RGB LCD Introduction` in ESP-IoT-Solution Programming Guide
 * (https://docs.espressif.com/projects/esp-iot-solution/en/latest/display/lcd/rgb_lcd.html#color-formats).
 *
 */

namespace esp_panel::drivers {

/**
 * @brief RGB bus object class
 *
 * @note  This class is a derived class of `Bus`, user can use it directly
 */
class BusRGB: public Bus {
public:
    constexpr static Attributes ATTRIBUTES_DEFAULT = {
        .type = ESP_PANEL_BUS_TYPE_RGB,
        .name = "RGB",
    };

    struct Config {
        esp_lcd_panel_io_3wire_spi_config_t getIO_Config(void) const
        {
            return {
                .line_config = {
                    .cs_io_type = static_cast<panel_io_type_t>(cs_io_type),
                    .cs_gpio_num = cs_gpio_num,
                    .scl_io_type = static_cast<panel_io_type_t>(scl_io_type),
                    .scl_gpio_num = scl_gpio_num,
                    .sda_io_type = static_cast<panel_io_type_t>(sda_io_type),
                    .sda_gpio_num = sda_gpio_num,
                },
                .expect_clk_speed = PANEL_IO_SPI_CLK_MAX,
                .spi_mode = static_cast<uint32_t>(flags_scl_active_falling_edge ? 1 : 0),
                .lcd_cmd_bytes = 1,
                .lcd_param_bytes = 1,
                .flags = {
                    .use_dc_bit = 1,
                    .dc_zero_on_data = 0,
                    .lsb_first = 0,
                    .cs_high_active = 0,
                    .del_keep_cs_inactive = 1,
                },
            };
        }

        esp_lcd_rgb_panel_config_t getPanelConfig(void) const
        {
            return {
                .clk_src = LCD_CLK_SRC_DEFAULT,
                .timings = {
                    .pclk_hz = static_cast<uint32_t>(pclk_hz),
                    .h_res = static_cast<uint32_t>(h_res),
                    .v_res = static_cast<uint32_t>(v_res),
                    .hsync_pulse_width = static_cast<uint32_t>(hsync_pulse_width),
                    .hsync_back_porch = static_cast<uint32_t>(hsync_back_porch),
                    .hsync_front_porch = static_cast<uint32_t>(hsync_front_porch),
                    .vsync_pulse_width = static_cast<uint32_t>(vsync_pulse_width),
                    .vsync_back_porch = static_cast<uint32_t>(vsync_back_porch),
                    .vsync_front_porch = static_cast<uint32_t>(vsync_front_porch),
                    .flags = {
                        .hsync_idle_low = 0,
                        .vsync_idle_low = 0,
                        .de_idle_high = 0,
                        .pclk_active_neg = flags_pclk_active_neg,
                        .pclk_idle_high = 0,
                    },
                },
                .data_width = static_cast<size_t>(data_width),
                .bits_per_pixel = static_cast<size_t>(bits_per_pixel),
                .num_fbs = 1,
                .bounce_buffer_size_px = static_cast<size_t>(bounce_buffer_size_px),
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 4, 0)
                .dma_burst_size = 64,
#else
                .sram_trans_align = 4,
                .psram_trans_align = 64,
#endif /* ESP_IDF_VERSION */
                .hsync_gpio_num = hsync_gpio_num,
                .vsync_gpio_num = vsync_gpio_num,
                .de_gpio_num = de_gpio_num,
                .pclk_gpio_num = pclk_gpio_num,
                .disp_gpio_num = disp_gpio_num,
                .data_gpio_nums = {
                    data_gpio_nums[0], data_gpio_nums[1], data_gpio_nums[2], data_gpio_nums[3],
                    data_gpio_nums[4], data_gpio_nums[5], data_gpio_nums[6], data_gpio_nums[7],
                    data_gpio_nums[8], data_gpio_nums[9], data_gpio_nums[10], data_gpio_nums[11],
                    data_gpio_nums[12], data_gpio_nums[13], data_gpio_nums[14], data_gpio_nums[15],
                },
                .flags = {
                    .disp_active_low = 0,
                    .refresh_on_demand = 0,
                    .fb_in_psram = 1,
                    .double_fb = 0,
                    .no_fb = 0,
                    .bb_invalidate_cache = 0,
                },
            };
        }

        static Config create(
            /* 3-wire SPI IOs */
            int cs_io, int sck_io, int sda_io,
            /* 16-bit RGB IOs */
            int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
            int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
            int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io,
            /* RGB timmings */
            int clk_mhz, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp
        )
        {
            return Config{
                // 3-wire SPI
                .cs_gpio_num = cs_io,
                .scl_gpio_num = sck_io,
                .sda_gpio_num = sda_io,
                // RGB
                .pclk_hz = clk_mhz * 1000000,
                .h_res = h_res,
                .v_res = v_res,
                .hsync_pulse_width = hpw,
                .hsync_back_porch = hbp,
                .hsync_front_porch = hfp,
                .vsync_pulse_width = vpw,
                .vsync_back_porch = vbp,
                .vsync_front_porch = vfp,
                .data_width = 16,
                .bits_per_pixel = 16,
                .hsync_gpio_num = hsync_io,
                .vsync_gpio_num = vsync_io,
                .de_gpio_num = de_io,
                .pclk_gpio_num = pclk_io,
                .disp_gpio_num = disp_io,
                .data_gpio_nums = {
                    d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
                    d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io,
                },
                // Extra
                .use_spi_interface = true,
            };
        }

        static Config create(
            /* 16-bit RGB IOs */
            int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
            int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
            int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io,
            /* RGB timmings */
            int clk_mhz, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp
        )
        {
            return Config{
                // RGB
                .pclk_hz = clk_mhz * 1000000,
                .h_res = h_res,
                .v_res = v_res,
                .hsync_pulse_width = hpw,
                .hsync_back_porch = hbp,
                .hsync_front_porch = hfp,
                .vsync_pulse_width = vpw,
                .vsync_back_porch = vbp,
                .vsync_front_porch = vfp,
                .data_width = 16,
                .bits_per_pixel = 16,
                .hsync_gpio_num = hsync_io,
                .vsync_gpio_num = vsync_io,
                .de_gpio_num = de_io,
                .pclk_gpio_num = pclk_io,
                .disp_gpio_num = disp_io,
                .data_gpio_nums = {
                    d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
                    d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io,
                },
                // Extra
                .use_spi_interface = false,
            };
        }

        static Config create(
            /* 3-wire SPI IOs */
            int cs_io, int sck_io, int sda_io,
            /* 8-bit RGB IOs */
            int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
            int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io,
            /* RGB timmings */
            int clk_mhz, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp
        )
        {
            return Config{
                // 3-wire SPI
                .cs_gpio_num = cs_io,
                .scl_gpio_num = sck_io,
                .sda_gpio_num = sda_io,
                // RGB
                .pclk_hz = clk_mhz * 1000000,
                .h_res = h_res,
                .v_res = v_res,
                .hsync_pulse_width = hpw,
                .hsync_back_porch = hbp,
                .hsync_front_porch = hfp,
                .vsync_pulse_width = vpw,
                .vsync_back_porch = vbp,
                .vsync_front_porch = vfp,
                .data_width = 8,
                .bits_per_pixel = 24,
                .hsync_gpio_num = hsync_io,
                .vsync_gpio_num = vsync_io,
                .de_gpio_num = de_io,
                .pclk_gpio_num = pclk_io,
                .disp_gpio_num = disp_io,
                .data_gpio_nums = {
                    d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io
                },
                // Extra
                .use_spi_interface = true,
            };
        }

        static Config create(
            /* 8-bit RGB IOs */
            int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
            int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io,
            /* RGB timmings */
            int clk_mhz, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp
        )
        {
            return Config{
                // RGB
                .pclk_hz = clk_mhz * 1000000,
                .h_res = h_res,
                .v_res = v_res,
                .hsync_pulse_width = hpw,
                .hsync_back_porch = hbp,
                .hsync_front_porch = hfp,
                .vsync_pulse_width = vpw,
                .vsync_back_porch = vbp,
                .vsync_front_porch = vfp,
                .data_width = 8,
                .bits_per_pixel = 24,
                .hsync_gpio_num = hsync_io,
                .vsync_gpio_num = vsync_io,
                .de_gpio_num = de_io,
                .pclk_gpio_num = pclk_io,
                .disp_gpio_num = disp_io,
                .data_gpio_nums = {
                    d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io
                },
                // Extra
                .use_spi_interface = false,
            };
        }

        // 3-wire SPI
        int cs_io_type = static_cast<int>(IO_TYPE_GPIO);
        int scl_io_type = static_cast<int>(IO_TYPE_GPIO);
        int sda_io_type = static_cast<int>(IO_TYPE_GPIO);
        int cs_gpio_num = -1;
        int scl_gpio_num = -1;
        int sda_gpio_num = -1;
        bool flags_scl_active_falling_edge = false;
        // RGB
        int pclk_hz = 16000000;
        int h_res = 0;
        int v_res = 0;
        int hsync_pulse_width = 10;
        int hsync_back_porch = 10;
        int hsync_front_porch = 20;
        int vsync_pulse_width = 10;
        int vsync_back_porch = 10;
        int vsync_front_porch = 10;
        int data_width = 16;
        int bits_per_pixel = 16;
        int bounce_buffer_size_px = 0;
        int hsync_gpio_num = -1;
        int vsync_gpio_num = -1;
        int de_gpio_num = -1;
        int pclk_gpio_num = -1;
        int disp_gpio_num = -1;
#ifdef SOC_LCDCAM_RGB_DATA_WIDTH
        int data_gpio_nums[SOC_LCDCAM_RGB_DATA_WIDTH] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
#else
        int data_gpio_nums[SOC_LCD_RGB_DATA_WIDTH] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
#endif
        bool flags_pclk_active_neg = false;
        // Extra
        bool use_spi_interface = false;
    };

// *INDENT-OFF*
    /**
     * @brief Construct a "3-wire SPI + 16-bit RGB" bus object
     *
     * @note  This function uses some default values to config the bus object, use `config*()` functions to change them
     *
     * @param[in] cs_io    3-wire SPI CS pin
     * @param[in] sck_io   3-wire SPI SCK pin
     * @param[in] sda_io   3-wire SPI SDA pin
     * @param[in] d[N]_io  RGB data pins, N is [0, 15]
     * @param[in] hsync_io RGB HSYNC pin
     * @param[in] vsync_io RGB VSYNC pin
     * @param[in] pclk_io  RGB PCLK pin
     * @param[in] de_io    RGB DE pin, set to -1 if not used
     * @param[in] disp_io  RGB DISP pin, default is -1
     * @param[in] clk_mhz  The clock frequency of the RGB bus
     * @param[in] h_res    The width of the panel (horizontal resolution)
     * @param[in] v_res    The height of the panel (vertical resolution)
     * @param[in] hpw      The HSYNC pulse width
     * @param[in] hbp      The HSYNC back porch
     * @param[in] hfp      The HSYNC front porch
     * @param[in] vpw      The VSYNC pulse width
     * @param[in] vbp      The VSYNC back porch
     * @param[in] vfp      The VSYNC front porch
     */
    BusRGB(
        /* 3-wire SPI IOs */
        int cs_io, int sck_io, int sda_io,
        /* 16-bit RGB IOs */
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io,
        /* RGB timmings */
        int clk_mhz, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp
    ):
        Bus(ATTRIBUTES_DEFAULT),
        _flags({true})
    {
        auto config = Config::create(
            /* 3-wire SPI IOs */
            cs_io, sck_io, sda_io,
            /* 16-bit RGB IOs */
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
            d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io,
            hsync_io, vsync_io, pclk_io, de_io, disp_io,
            /* RGB timmings */
            clk_mhz, h_res, v_res, hpw, hbp, hfp, vpw, vbp, vfp
        );
        _io_config = config.getIO_Config();
        _panel_config = config.getPanelConfig();
    }

    /**
     * @brief Construct a single "16-bit RGB" bus object
     *
     * @note  This function uses some default values to config the bus object, use `config*()` functions to change them
     *
     * @param[in] d[N]_io  RGB data pins, N is [0, 15]
     * @param[in] hsync_io RGB HSYNC pin
     * @param[in] vsync_io RGB VSYNC pin
     * @param[in] pclk_io  RGB PCLK pin
     * @param[in] de_io    RGB DE pin, set to -1 if not used
     * @param[in] disp_io  RGB DISP pin, default is -1
     * @param[in] clk_mhz  The clock frequency of the RGB bus
     * @param[in] h_res    The width of the panel (horizontal resolution)
     * @param[in] v_res    The height of the panel (vertical resolution)
     * @param[in] hpw      The HSYNC pulse width
     * @param[in] hbp      The HSYNC back porch
     * @param[in] hfp      The HSYNC front porch
     * @param[in] vpw      The VSYNC pulse width
     * @param[in] vbp      The VSYNC back porch
     * @param[in] vfp      The VSYNC front porch
     */
    BusRGB(
        /* 16-bit RGB IOs */
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io,
        /* RGB timmings */
        int clk_mhz, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp
    ):
        Bus(ATTRIBUTES_DEFAULT),
        _flags({false})
    {
        auto config = Config::create(
            /* 16-bit RGB IOs */
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
            d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io,
            hsync_io, vsync_io, pclk_io, de_io, disp_io,
            /* RGB timmings */
            clk_mhz, h_res, v_res, hpw, hbp, hfp, vpw, vbp, vfp
        );
        _panel_config = config.getPanelConfig();
    }

    /**
     * @brief Construct a "3-wire SPI + 8-bit RGB" bus object
     *
     * @note  This function uses some default values to config the bus object, use `config*()` functions to change them
     *
     * @param[in] cs_io    3-wire SPI CS pin
     * @param[in] sck_io   3-wire SPI SCK pin
     * @param[in] sda_io   3-wire SPI SDA pin
     * @param[in] d[N]_io  RGB data pins, N is [0, 7]
     * @param[in] hsync_io RGB HSYNC pin
     * @param[in] vsync_io RGB VSYNC pin
     * @param[in] pclk_io  RGB PCLK pin
     * @param[in] de_io    RGB DE pin, set to -1 if not used
     * @param[in] disp_io  RGB DISP pin, default is -1
     * @param[in] clk_mhz  The clock frequency of the RGB bus
     * @param[in] h_res    The width of the panel (horizontal resolution)
     * @param[in] v_res    The height of the panel (vertical resolution)
     * @param[in] hpw      The HSYNC pulse width
     * @param[in] hbp      The HSYNC back porch
     * @param[in] hfp      The HSYNC front porch
     * @param[in] vpw      The VSYNC pulse width
     * @param[in] vbp      The VSYNC back porch
     * @param[in] vfp      The VSYNC front porch
     */
    BusRGB(
        /* 3-wire SPI IOs */
        int cs_io, int sck_io, int sda_io,
        /* 8-bit RGB IOs */
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io,
        /* RGB timmings */
        int clk_mhz, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp
    ):
        Bus(ATTRIBUTES_DEFAULT),
        _flags({true})
    {
        auto config = Config::create(
            /* 3-wire SPI IOs */
            cs_io, sck_io, sda_io,
            /* 8-bit RGB IOs */
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
            hsync_io, vsync_io, pclk_io, de_io, disp_io,
            /* RGB timmings */
            clk_mhz, h_res, v_res, hpw, hbp, hfp, vpw, vbp, vfp
        );
        _io_config = config.getIO_Config();
        _panel_config = config.getPanelConfig();
    }

    /**
     * @brief Construct a single "8-bit RGB" bus object
     *
     * @note  This function uses some default values to config the bus object, use `config*()` functions to change them
     *
     * @param[in] d[N]_io  RGB data pins, N is [0, 7]
     * @param[in] hsync_io RGB HSYNC pin
     * @param[in] vsync_io RGB VSYNC pin
     * @param[in] pclk_io  RGB PCLK pin
     * @param[in] de_io    RGB DE pin, set to -1 if not used
     * @param[in] disp_io  RGB DISP pin, default is -1
     * @param[in] clk_mhz  The clock frequency of the RGB bus
     * @param[in] h_res    The width of the panel (horizontal resolution)
     * @param[in] v_res    The height of the panel (vertical resolution)
     * @param[in] hpw      The HSYNC pulse width
     * @param[in] hbp      The HSYNC back porch
     * @param[in] hfp      The HSYNC front porch
     * @param[in] vpw      The VSYNC pulse width
     * @param[in] vbp      The VSYNC back porch
     * @param[in] vfp      The VSYNC front porch
     */
    BusRGB(
        /* 8-bit RGB IOs */
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io,
        /* RGB timmings */
        int clk_mhz, int h_res, int v_res, int hpw, int hbp, int hfp, int vpw, int vbp, int vfp
    ):
        Bus(ATTRIBUTES_DEFAULT),
        _flags({false})
    {
        auto config = Config::create(
            /* 8-bit RGB IOs */
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
            hsync_io, vsync_io, pclk_io, de_io, disp_io,
            /* RGB timmings */
            clk_mhz, h_res, v_res, hpw, hbp, hfp, vpw, vbp, vfp
        );
        _io_config = config.getIO_Config();
        _panel_config = config.getPanelConfig();
    }

    /**
     * @brief Construct a RGB bus object with a configuration
     *
     * @param[in] config Bus configuration
     */
    BusRGB(const Config &config):
        Bus(ATTRIBUTES_DEFAULT),
        _flags({config.use_spi_interface}),
        _io_config(config.getIO_Config()),
        _panel_config(config.getPanelConfig())
    {
    }
// *INDENT-OFF*

    [[deprecated("Deprecated and will be removed in the next major version. Use other constructors instead.")]]
    BusRGB(
        uint16_t width, uint16_t height,
        int cs_io, int sck_io, int sda_io,
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io = -1
    ):
        BusRGB(
            cs_io, sck_io, sda_io,
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
            d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io,
            hsync_io, vsync_io, pclk_io, de_io, disp_io,
            16, width, height, 10, 10, 20, 10, 10, 10
        )
    {
    }

    [[deprecated("Deprecated and will be removed in the next major version. Use other constructors instead.")]]
    BusRGB(
        uint16_t width, uint16_t height,
        int cs_io, int sck_io, int sda_io,
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io = -1
    ):
        BusRGB(
            cs_io, sck_io, sda_io,
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
            hsync_io, vsync_io, pclk_io, de_io, disp_io,
            8, width, height, 10, 10, 20, 10, 10, 10
        )
    {
    }

    [[deprecated("Deprecated and will be removed in the next major version. Use other constructors instead.")]]
    BusRGB(
        uint16_t width, uint16_t height,
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int d8_io, int d9_io, int d10_io, int d11_io, int d12_io, int d13_io, int d14_io, int d15_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io = -1
    ):
        BusRGB(
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
            d8_io, d9_io, d10_io, d11_io, d12_io, d13_io, d14_io, d15_io,
            hsync_io, vsync_io, pclk_io, de_io, disp_io,
            16, width, height, 10, 10, 20, 10, 10, 10
        )
    {
    }

    [[deprecated("Deprecated and will be removed in the next major version. Use other constructors instead.")]]
    BusRGB(
        uint16_t width, uint16_t height,
        int d0_io, int d1_io, int d2_io, int d3_io, int d4_io, int d5_io, int d6_io, int d7_io,
        int hsync_io, int vsync_io, int pclk_io, int de_io, int disp_io = -1
    ):
        BusRGB(
            d0_io, d1_io, d2_io, d3_io, d4_io, d5_io, d6_io, d7_io,
            hsync_io, vsync_io, pclk_io, de_io, disp_io,
            8, width, height, 10, 10, 20, 10, 10, 10
        )
    {
    }

    /**
     * @brief Destroy the RGB bus object
     *
     */
    ~BusRGB() override;

    /**
     * @brief Here are some functions to configure the RGB bus object
     *
     * @note  These functions should be called before `begin()`
     *
     */
    bool configSPI_IO_Expander(esp_io_expander_t *expander_handle);
    bool configSPI_SCL_ActiveFallingEdge(bool enable);

    bool configRgbFrameBufferNumber(uint8_t num);
    bool configRgbBounceBufferSize(uint32_t size_in_pixel);
    bool configRgbFlagDispActiveLow(void);

    [[deprecated("Deprecated and will be removed in the next major version. Please use `configSPI_IO_Expander()` instead.")]]
    bool configSpiLine(bool cs_use_expaneer, bool sck_use_expander, bool sda_use_expander, esp_expander::Base *io_expander);
    [[deprecated("Deprecated and will be removed in the next major version.")]]
    bool configRgbTimingFreqHz(uint32_t hz);
    [[deprecated("Deprecated and will be removed in the next major version.")]]
    bool configRgbTimingPorch(uint16_t hpw, uint16_t hbp, uint16_t hfp, uint16_t vpw, uint16_t vbp, uint16_t vfp);
    bool configRgbTimingFlags(
        bool hsync_idle_low, bool vsync_idle_low, bool de_idle_high, bool pclk_active_neg, bool pclk_idle_high
    );

    /**
     * @brief Startup the bus.
     *
     * @return true if success, otherwise false
     *
     */
    bool begin(void) override;

    /**
     * @brief Delete the bus object, release the resources
     *
     * @return true if success, otherwise false
     *
     */
    bool del(void) override;

    bool checkUseSPI_Interface(void) const
    {
        return _flags.use_spi_interface;
    }

    const esp_lcd_rgb_panel_config_t *getPanelConfig()
    {
        return &_panel_config;
    }

    [[deprecated("Deprecated and will be removed in the next major version. Use `getPanelConfig()` instead.")]]
    const esp_lcd_rgb_panel_config_t *getRgbConfig()
    {
        return getPanelConfig();
    }

private:
    struct {
        uint8_t use_spi_interface: 1;
    } _flags = {};
    esp_lcd_panel_io_3wire_spi_config_t _io_config = {};
    esp_lcd_rgb_panel_config_t _panel_config = {};
};

} // namespace esp_panel::drivers

/**
 * @deprecated This type is deprecated and will be removed in the next major version. Please use `esp_panel::drivers::BusRGB` instead.
 *
 * Example of migration:
 *
 * Old:
 *  ESP_PanelBusRGB bus;
 *
 * New:
 *  esp_panel::drivers::BusRGB bus;
 */
typedef esp_panel::drivers::BusRGB ESP_PanelBusRGB __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::BusRGB` instead.")));

#endif // SOC_LCD_RGB_SUPPORTED
