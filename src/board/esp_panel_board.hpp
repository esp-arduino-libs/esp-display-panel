/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <memory>
#include <string>
#include "esp_panel_types.h"
#include "esp_panel_board_conf_internal.h"
#include "esp_panel_board_config.hpp"
#include "esp_panel_board_config_internal.hpp"

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
     * @brief Board state enumeration
     */
    enum class State : uint8_t {
        DEINIT = 0,    /*!< Board is not initialized */
        INIT,          /*!< Board is initialized */
        BEGIN,         /*!< Board is started */
    };

    /**
     * @brief Default constructor, initializes the board with default configuration.
     *
     * There are two ways to provide a default configuration:
     * 1. Use the `esp_panel_board_supported.h` file to enable a supported board
     * 2. Use the `esp_panel_board_custom.h` file to define a custom board
     */
#if ESP_PANEL_BOARD_USE_DEFAULT
    Board();
#else
    Board() = delete;
#endif

    /**
     * @brief Constructor with configuration
     *
     * @param[in] config Board configuration structure
     */
    Board(const BoardConfig &config): _config(config) {}

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
    const BoardConfig &getConfig() const
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

    [[deprecated("Deprecated. Please use `getIO_Expander()` instead")]]
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

    BoardConfig _config = {};
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
