/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "esp_panel_types.h"
#include "esp_panel_backlight_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief The custom backlight device class
 *
 * @note This class is a derived class of `Backlight`. Users can implement
 *       custom backlight control through callback functions.
 */
class BacklightCustom: public Backlight {
public:
    /**
     * Here are some default values for custom backlight device
     */
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {
        .type = ESP_PANEL_BACKLIGHT_TYPE_CUSTOM,
        .name = "Custom",
    };

    using FunctionSetBrightnessCallback = bool (*)(uint8_t, void *);

    /**
     * @brief The custom backlight device configuration structure
     */
    struct Config {
        /*!< The callback function to set the brightness. Default is `nullptr` */
        FunctionSetBrightnessCallback callback = nullptr;
        /*!< The user data passed to the callback function. Default is `nullptr` */
        void *user_data = nullptr;
    };

// *INDENT-OFF*
    /**
     * @brief Construct the custom backlight device with separate parameters
     *
     * @param[in] callback  The callback function to set the brightness
     * @param[in] user_data The user data passed to the callback function
     */
    BacklightCustom(FunctionSetBrightnessCallback callback, void *user_data):
        Backlight(BASIC_ATTRIBUTES_DEFAULT),
        _config(Config{callback, user_data})
    {
    }
// *INDENT-OFF*

    /**
     * @brief Construct the custom backlight device with configuration
     *
     * @param[in] config The custom backlight configuration
     */
    BacklightCustom(const Config &config): Backlight(BASIC_ATTRIBUTES_DEFAULT), _config(config) {}

    /**
     * @brief Destroy the device
     */
    ~BacklightCustom() override;

    /**
     * @brief Startup the device
     *
     * @return true if success, otherwise false
     */
    bool begin() override;

    /**
     * @brief Delete the device, release the resources
     *
     * @note  After calling this function, users should call `begin()` to re-init the device
     *
     * @return true if success, otherwise false
     */
    bool del() override;

    /**
     * @brief Set the brightness by percent
     *
     * @note  This function should be called after `begin()`
     *
     * @param[in] percent The brightness percent (0-100)
     *
     * @return true if success, otherwise false
     */
    bool setBrightness(uint8_t percent) override;

private:
    Config _config = {};
};

} // namespace esp_panel::drivers
