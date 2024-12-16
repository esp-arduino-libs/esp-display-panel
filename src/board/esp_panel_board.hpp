/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <memory>
#include <variant>
#include "esp_panel_types.h"
#include "esp_panel_board_config.hpp"

namespace esp_panel {

/**
 * @brief Panel device class
 *
 * @note This class is mainly aimed at development boards, which integrates other independent drivers such as LCD, Touch, and Backlight.
 * @note For supported development boards, the drivers internally automatically use preset parameters to configure each independent driver.
 * @note For custom development boards, users need to adjust the parameters according to the actual hardware through the `esp_panel_board_custom.h` file.
 */
class Board {
public:
    Board();

    Board(const BoardConfig &config): _config(config) {}

    /**
     * @brief Destroy the panel device
     */
    ~Board() = default;

    // /**
    //  * @brief Configure the IO expander from the outside. This function should be called before `init()`
    //  *
    //  * @param expander Pointer of IO expander
    //  *
    //  */
    // void configExpander(esp_expander::Base *expander);

    /**
     * @brief Initialize the panel device, the `begin()` function should be called after this function
     *
     * @note This function typically creates objects for the LCD, Touch, Backlight, and other devices based on the configuration. It then initializes the bus host used by each device.
     *
     * @return true if success, otherwise false
     */
    bool init();

    /**
     * @brief Startup the panel device. This function should be called after `init()`
     *
     * @note This function typically initializes the LCD, Touch, Backlight, and other devices based on the configuration.
     *
     * @return true
     */
    bool begin();

    /**
     * @brief Delete the panel device, release the resources. This function should be called after `init()`
     *
     * @return true if success, otherwise false
     */
    bool del();

    // /**
    //  * Here are the functions to get the pointer of each device
    //  *
    //  */
    drivers::LCD *getLcd()
    {
        return _lcd_device.get();
    }

    drivers::Touch *getTouch()
    {
        return _touch_device.get();
    }

    drivers::Backlight *getBacklight()
    {
        return _backlight.get();
    }

    esp_expander::Base *getIO_Expander()
    {
        return _io_expander->getBase();
    }

    /**
     * Here are the functions to get the some parameters of the devices
     */
    uint16_t getLcdWidth()
    {
        return _config.width;
    }
    uint16_t getLcdHeight()
    {
        return _config.height;
    }

private:
    enum class State : uint8_t {
        DEINIT = 0,
        INIT,
        BEGIN,
    };

    bool isOverState(State state)
    {
        return (this->_state >= state);
    }

    void setState(State state)
    {
        this->_state = state;
    }

    struct {
        uint8_t use_default_config: 1;
    } _flags = {};
    State _state = State::DEINIT;
    BoardConfig _config = {};
    std::shared_ptr<drivers::Bus> _lcd_bus = nullptr;
    std::shared_ptr<drivers::LCD> _lcd_device = nullptr;
    std::shared_ptr<drivers::Backlight> _backlight = nullptr;
    std::shared_ptr<drivers::Bus> _touch_bus = nullptr;
    std::shared_ptr<drivers::Touch> _touch_device = nullptr;
    std::shared_ptr<drivers::IO_Expander> _io_expander = nullptr;
};

/**
 * @brief Internal board configurations
 */
extern const BoardConfig BOARD_DEFAULT_CONFIG;  // Declare on here to avoid undefined reference
extern const BoardConfig BOARD_ESP32_S3_LCD_EV_BOARD_V1_5_CONFIG;

} // namespace esp_panel

/**
 * @deprecated Deprecated. Please use `esp_panel::Board` instead.
 */
typedef esp_panel::Board ESP_Panel __attribute((deprecated("Deprecated. Please use `esp_panel::Board` instead.")));
