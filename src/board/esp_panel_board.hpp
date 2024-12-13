/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
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
     *
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
     *
     */
    bool init(void);

    /**
     * @brief Startup the panel device. This function should be called after `init()`
     *
     * @note This function typically initializes the LCD, Touch, Backlight, and other devices based on the configuration.
     *
     * @return true
     */
    bool begin(void);

    /**
     * @brief Delete the panel device, release the resources. This function should be called after `init()`
     *
     * @return true if success, otherwise false
     *
     */
    bool del(void);

    // /**
    //  * @brief Here are the functions to get the pointer of each device
    //  *
    //  */
    drivers::LCD *getLcd(void)
    {
        return _lcd_device.get();
    }

    drivers::Touch *getTouch(void)
    {
        return _touch_device.get();
    }

    drivers::Backlight *getBacklight(void)
    {
        return _backlight.get();
    }

    esp_expander::Base *getIO_Expander(void)
    {
        return _io_expander->getAdaptee();
    }

    /**
     * @brief Here are the functions to get the some parameters of the devices
     *
     */
    uint16_t getLcdWidth(void)
    {
        return _config.width;
    }
    uint16_t getLcdHeight(void)
    {
        return _config.height;
    }

protected:
    bool checkIsInit(void)
    {
        return _flags.is_init;
    }

    /**
     * @brief Check if the device is already begun
     *
     * @return true if the device is already begun, otherwise false
     *
     */
    bool checkIsBegun(void)
    {
        return _flags.is_begun;
    }

private:
    struct {
        uint8_t is_init: 1;
        uint8_t is_begun: 1;
        uint8_t use_default_config: 1;
    } _flags = {};
    BoardConfig _config = {};
    std::shared_ptr<drivers::LCD> _lcd_device = nullptr;
    std::shared_ptr<drivers::Backlight> _backlight = nullptr;
    std::shared_ptr<drivers::Touch> _touch_device = nullptr;
    std::shared_ptr<drivers::IO_Expander> _io_expander = nullptr;
};

/**
 * @brief Internal board configurations
 *
 */
extern const BoardConfig BOARD_DEFAULT_CONFIG;  // Declare on here to avoid undefined reference
extern const BoardConfig BOARD_ESP32_S3_LCD_EV_BOARD_V1_5_CONFIG;

} // namespace esp_panel

/**
 * @deprecated Deprecated and will be removed in the next major version. Please use `esp_panel::Board` instead.
 *
 */
typedef esp_panel::Board ESP_Panel __attribute((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::Board` instead.")));
