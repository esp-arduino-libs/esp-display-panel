/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "drivers/bus/esp_panel_bus_factory.hpp"
#include "drivers/lcd/esp_panel_lcd_factory.hpp"
#include "drivers/touch/esp_panel_touch_factory.hpp"
#include "drivers/backlight/esp_panel_backlight_factory.hpp"
#include "drivers/io_expander/esp_panel_io_expander_factory.hpp"

namespace esp_panel {

/**
 * @brief Configuration structure for ESP Panel board
 *
 * Contains all configurations needed for initializing an ESP Panel board
 */
struct BoardConfig {
    /**
     * @brief Function pointer type for board callback functions
     *
     * @param[in] args User arguments passed to the callback function, usually the pointer of the board itself
     * @return `true` if successful, `false` otherwise
     */
    using FunctionCallback = bool (*)(void *args);

    /* General */
    int width = 0;              /*!< Panel width in pixels */
    int height = 0;             /*!< Panel height in pixels */

    /**
     * @brief LCD related configuration
     */
    struct LCD_Config {
        drivers::BusFactory::Config bus_config;       /*!< LCD bus configuration */
        std::string device_name;                      /*!< LCD device name */
        drivers::LCD::Config device_config;           /*!< LCD device configuration */
        struct {
            int invert_color: 1;                      /*!< Invert color if set to 1 */
            int swap_xy: 1;                           /*!< Swap X and Y coordinates if set to 1 */
            int mirror_x: 1;                          /*!< Mirror X coordinate if set to 1 */
            int mirror_y: 1;                          /*!< Mirror Y coordinate if set to 1 */
        } pre_process;                                /*!< LCD pre-process flags */
    } lcd = {};

    /**
     * @brief Touch related configuration
     */
    struct TouchConfig {
        drivers::BusFactory::Config bus_config;       /*!< Touch bus configuration */
        std::string device_name;                      /*!< Touch device name */
        drivers::Touch::Config device_config;         /*!< Touch device configuration */
        struct {
            int swap_xy: 1;                          /*!< Swap X and Y coordinates if set to 1 */
            int mirror_x: 1;                         /*!< Mirror X coordinate if set to 1 */
            int mirror_y: 1;                         /*!< Mirror Y coordinate if set to 1 */
        } pre_process;                               /*!< Touch pre-process flags */
    } touch = {};

    /**
     * @brief Backlight related configuration
     */
    struct BacklightConfig {
        drivers::BacklightFactory::Config config;     /*!< Backlight device configuration */
        struct {
            int idle_off: 1;                         /*!< Turn off backlight in idle mode if set to 1 */
        } pre_process;                               /*!< Backlight pre-process flags */
    } backlight = {};

    /**
     * @brief IO expander related configuration
     */
    struct IO_ExpanderConfig {
        std::string name;                            /*!< IO expander device name */
        drivers::IO_Expander::Config config;         /*!< IO expander device configuration */
    } io_expander = {};

    /**
     * @brief Stage callback functions
     */
    struct Callbacks {
        FunctionCallback pre_board_begin;            /*!< Called before board initialization */
        FunctionCallback post_board_begin;           /*!< Called after board initialization */
        FunctionCallback pre_expander_begin;         /*!< Called before IO expander initialization */
        FunctionCallback post_expander_begin;        /*!< Called after IO expander initialization */
        FunctionCallback pre_lcd_begin;              /*!< Called before LCD initialization */
        FunctionCallback post_lcd_begin;             /*!< Called after LCD initialization */
        FunctionCallback pre_touch_begin;            /*!< Called before touch initialization */
        FunctionCallback post_touch_begin;           /*!< Called after touch initialization */
        FunctionCallback pre_backlight_begin;        /*!< Called before backlight initialization */
        FunctionCallback post_backlight_begin;       /*!< Called after backlight initialization */
    } callbacks = {};

    /**
     * @brief Board feature flags
     */
    struct Flags {
        int use_lcd: 1;                             /*!< Enable LCD if set to 1 */
        int use_touch: 1;                           /*!< Enable touch if set to 1 */
        int use_backlight: 1;                       /*!< Enable backlight if set to 1 */
        int use_io_expander: 1;                     /*!< Enable IO expander if set to 1 */
    } flags = {};
};

} // namespace esp_panel
