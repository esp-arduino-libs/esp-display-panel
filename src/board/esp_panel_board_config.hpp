/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <string>
#include "drivers/bus/esp_panel_bus.hpp"
#include "drivers/lcd/esp_panel_lcd.hpp"
#include "drivers/touch/esp_panel_touch.hpp"
#include "drivers/backlight/esp_panel_backlight.hpp"
#include "drivers/io_expander/esp_panel_io_expander.hpp"

namespace esp_panel {

// *INDENT-OFF*
struct BoardConfig {
    using FunctionCallback = bool (*)(void *);

    /* General */
    int width = 0;
    int height = 0;

    /* LCD */
    struct {
        drivers::BusFactory::Config bus_config;
        std::string device_name;
        drivers::LCD::Config device_config;
        struct {
            int invert_color: 1;
            int swap_xy: 1;
            int mirror_x: 1;
            int mirror_y: 1;
        } pre_process;
    } lcd = {};

    /* Touch */
    struct {
        drivers::BusFactory::Config bus_config;
        std::string device_name;
        drivers::Touch::Config device_config;
        struct {
            int swap_xy: 1;
            int mirror_x: 1;
            int mirror_y: 1;
        } pre_process;
    } touch = {};

    /* Backlight */
    struct {
        drivers::BacklightFactory::Config config;
        struct {
            int idle_off: 1;
        } pre_process;
    } backlight = {};

    /* IO expander */
    struct {
        std::string name;
        drivers::IO_Expander::Config config;
    } io_expander = {};

    /* Others */
    struct {
        FunctionCallback pre_board_begin;
        FunctionCallback post_board_begin;
        FunctionCallback pre_expander_begin;
        FunctionCallback post_expander_begin;
        FunctionCallback pre_lcd_begin;
        FunctionCallback post_lcd_begin;
        FunctionCallback pre_touch_begin;
        FunctionCallback post_touch_begin;
        FunctionCallback pre_backlight_begin;
        FunctionCallback post_backlight_begin;
    } callbacks = {};
    struct {
        int use_lcd: 1;
        int use_touch: 1;
        int use_backlight: 1;
        int use_io_expander: 1;
    } flags = {};
};

} // namespace esp_panel
