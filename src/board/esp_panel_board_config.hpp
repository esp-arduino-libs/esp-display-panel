/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <string>
#include <variant>
#include "esp_panel_types.h"
#include "drivers/bus/esp_panel_bus.hpp"
#include "drivers/lcd/esp_panel_lcd.hpp"
#include "drivers/touch/esp_panel_touch.hpp"
#include "drivers/backlight/esp_panel_backlight.hpp"
#include "drivers/io_expander/esp_panel_io_expander.hpp"

namespace esp_panel {

// *INDENT-OFF*
struct BoardConfig {
    using CallbackFunction = bool (*)(void *);

    /* General */
    uint16_t width = 0;
    uint16_t height = 0;

    /* LCD */
    struct {
        int bus_type;
        std::variant<
            drivers::Bus_SPI::Config,
            drivers::Bus_QSPI::Config
#if SOC_LCD_RGB_SUPPORTED
            ,drivers::Bus_RGB::Config
#endif // SOC_LCD_RGB_SUPPORTED
#if SOC_MIPI_DSI_SUPPORTED
            ,drivers::Bus_DSI::Config
#endif // SOC_MIPI_DSI_SUPPORTED
        > bus_config;
        std::string device_name;
        drivers::LCD::Config device_config;
        struct {
            uint8_t invert_color: 1;
            uint8_t swap_xy: 1;
            uint8_t mirror_x: 1;
            uint8_t mirror_y: 1;
        } pre_process;
    } lcd = {};

    /* Touch */
    struct {
        int bus_type;
        std::variant<
            drivers::Bus_I2C::Config,
            drivers::Bus_SPI::Config
        > bus_config;
        std::string device_name;
        drivers::Touch::Config device_config;
        struct {
            uint8_t swap_xy: 1;
            uint8_t mirror_x: 1;
            uint8_t mirror_y: 1;
        } pre_process;
    } touch = {};

    /* Backlight */
    struct {
        drivers::BacklightFactory::Type type;
        std::variant<
            drivers::BacklightSwitchGPIO::Config,
            drivers::BacklightPWM_LEDC::Config,
            drivers::BacklightCustom::Config
        > config;
        struct {
            uint8_t idle_off: 1;
        } pre_process;
    } backlight = {};

    /* IO expander */
    struct {
        std::string name;
        esp_expander::Base::Config config;
    } io_expander = {};

    /* Others */
    struct {
        CallbackFunction pre_board_begin;
        CallbackFunction post_board_begin;
        CallbackFunction pre_expander_begin;
        CallbackFunction post_expander_begin;
        CallbackFunction pre_lcd_begin;
        CallbackFunction post_lcd_begin;
        CallbackFunction pre_touch_begin;
        CallbackFunction post_touch_begin;
        CallbackFunction pre_backlight_begin;
        CallbackFunction post_backlight_begin;
    } callbacks = {};
    struct {
        uint8_t use_lcd: 1;
        uint8_t use_touch: 1;
        uint8_t use_backlight: 1;
        uint8_t use_io_expander: 1;
    } flags = {};
};

} // namespace esp_panel
