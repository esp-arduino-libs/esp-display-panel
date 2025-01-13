/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <memory>
#include <variant>
#include <string>
#include "esp_panel_types.h"
#include "drivers/bus/esp_panel_bus_factory.hpp"
#include "drivers/lcd/esp_panel_lcd_factory.hpp"
#include "drivers/touch/esp_panel_touch_factory.hpp"
#include "drivers/backlight/esp_panel_backlight_factory.hpp"
#include "drivers/io_expander/esp_panel_io_expander_factory.hpp"

namespace esp_panel {

/**
 * @brief Panel device class for ESP development boards
 *
 * This class integrates independent drivers such as LCD, Touch, and Backlight for development boards.
 * For supported development boards, the drivers internally use preset parameters to configure each independent driver.
 * For custom development boards, users need to adjust the parameters according to the actual hardware through the
 * `esp_panel_board_custom.h` file.
 */
class Board {
public:
    /**
     * @brief Configuration structure for ESP Panel board
     *
     * Contains all configurations needed for initializing an ESP Panel board
     */
    struct Config {
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

    /**
     * @brief Board state enumeration
     */
    enum class State : uint8_t {
        DEINIT = 0,    /*!< Board is not initialized */
        INIT,          /*!< Board is initialized */
        BEGIN,         /*!< Board is started */
    };

    /**
     * @brief Default constructor
     *
     * When `ESP_PANEL_BOARD_USE_DEFAULT` is enabled, initializes the board with default configuration.
     * Otherwise, this constructor will trigger a static assertion error.
     */
    Board();

    /**
     * @brief Constructor with configuration
     *
     * @param[in] config Board configuration structure
     */
    Board(const Config &config): _config(config) {}

    /**
     * @brief Configure the IO expander from the outside.
     *
     * @param[in] expander IO expander instance
     * @return `true` if successful, `false` otherwise
     * @note This function should be called before `init()`
     */
    bool configIO_Expander(drivers::IO_Expander *expander);

    /**
     * @brief Initialize the panel device
     *
     * Creates objects for the LCD, Touch, Backlight, and other devices based on the configuration.
     * The initialization sequence is: `LCD -> Touch -> Backlight -> IO Expander`
     *
     * @return `true` if successful, `false` otherwise
     */
    bool init();

    /**
     * @brief Startup the panel device
     *
     * Initializes and configures all enabled devices in the following order: `IO Expander -> LCD -> Touch -> Backlight`
     *
     * @return `true` if successful, `false` otherwise
     * @note Will automatically call `init()` if not already initialized
     */
    bool begin();

    /**
     * @brief Delete the panel device and release resources
     *
     * Releases all device instances in the following order: `Backlight -> LCD -> Touch -> IO Expander`
     *
     * @return `true` if successful, `false` otherwise
     * @note After calling this function, the board returns to uninitialized state
     */
    bool del();

    /**
     * @brief Check if current state is greater than or equal to given state
     *
     * @param[in] state State to compare with
     * @return `true` if current state is greater than or equal to given state
     */
    bool isOverState(State state)
    {
        return (this->_state >= state);
    }

    /**
     * @brief Get the LCD driver instance
     *
     * @return Pointer to the LCD driver instance, or `nullptr` if LCD is not enabled or not initialized
     */
    drivers::LCD *getLCD()
    {
        return _lcd_device.get();
    }

    /**
     * @brief Get the Touch driver instance
     *
     * @return Pointer to the Touch driver instance, or `nullptr` if Touch is not enabled or not initialized
     */
    drivers::Touch *getTouch()
    {
        return _touch_device.get();
    }

    /**
     * @brief Get the Backlight driver instance
     *
     * @return Pointer to the Backlight driver instance, or `nullptr` if Backlight is not enabled or not initialized
     */
    drivers::Backlight *getBacklight()
    {
        return _backlight.get();
    }

    /**
     * @brief Get the IO Expander driver instance
     *
     * @return Pointer to the IO Expander driver instance, or `nullptr` if IO Expander is not enabled or not initialized
     */
    drivers::IO_Expander *getIO_Expander()
    {
        return _io_expander.get();
    }

    /**
     * @brief Get the current board configuration
     *
     * @return Reference to the current board configuration
     */
    const Config &getConfig() const
    {
        return _config;
    }

    [[deprecated("Deprecated. Please use `configIO_Expander()` instead")]]
    bool configExpander(drivers::IO_Expander *expander)
    {
        return configIO_Expander(expander);
    }

    [[deprecated("Deprecated. Please use `getLCD()` instead")]]
    drivers::LCD *getLcd()
    {
        return getLCD();
    }

    [[deprecated("Deprecated. Please use `getIO_Expander()->getBase()` instead")]]
    esp_expander::Base *getExpander()
    {
        return getIO_Expander()->getBase();
    }

    [[deprecated("Deprecated. Please use `getConfig().width` instead")]]
    int getLcdWidth()
    {
        return getConfig().width;
    }

    [[deprecated("Deprecated. Please use `getConfig().height` instead")]]
    int getLcdHeight()
    {
        return getConfig().height;
    }

private:
    /**
     * @brief Set the current board state
     *
     * @param[in] state New state to set
     */
    void setState(State state)
    {
        _state = state;
    }

    Config _config = {};
    bool _use_default_config = false;
    State _state = State::DEINIT;
    std::shared_ptr<drivers::Bus> _lcd_bus = nullptr;
    std::shared_ptr<drivers::LCD> _lcd_device = nullptr;
    std::shared_ptr<drivers::Backlight> _backlight = nullptr;
    std::shared_ptr<drivers::Bus> _touch_bus = nullptr;
    std::shared_ptr<drivers::Touch> _touch_device = nullptr;
    std::shared_ptr<drivers::IO_Expander> _io_expander = nullptr;
};

} // namespace esp_panel

/**
 * @brief Alias for esp_panel::Board for backward compatibility
 * @deprecated Use `esp_panel::Board` instead
 */
using ESP_Panel [[deprecated("Use esp_panel::Board instead")]] = esp_panel::Board;
