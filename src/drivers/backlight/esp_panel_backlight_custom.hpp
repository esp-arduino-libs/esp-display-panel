/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <functional>
#include "esp_panel_backlight_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief The custom backlight device class
 *
 * @note  The class is a derived class of `Backlight`, users can use it to construct a custom backlight device
 *
 */
class BacklightCustom: public Backlight {
public:
    /**
     * @brief The default attributes of custom backlight device
     *
     */
    constexpr static Attributes ATTRIBUTES_DEFAULT = {
        .name = "Custom",
    };

    /**
     * @brief The callback function to set the brightness
     *
     */
    using SetBrightnessCallback = bool (*)(uint8_t, void *);

    /**
     * @brief The configuration for custom backlight device
     *
     */
    struct Config {
        SetBrightnessCallback callback = nullptr;   /*<! The callback function to set the brightness. Default is `nullptr` */
        void *user_data = nullptr;                  /*<! The user data passed to the callback function. Default is `nullptr` */
    };

// *INDENT-OFF*
    /**
     * @brief Construct a new custom device with simple parameters
     *
     * @param[in] callback  The callback function to set the brightness
     * @param[in] user_data The user data passed to the callback function
     *
     */
    BacklightCustom(SetBrightnessCallback callback, void *user_data):
        Backlight(ATTRIBUTES_DEFAULT),
        _config(Config{callback, user_data})
    {
    }
// *INDENT-OFF*

    /**
     * @brief Construct a new custom device with complex parameters
     *
     * @param[in] config The custom backlight configuration
     *
     */
    BacklightCustom(const Config &config): Backlight(ATTRIBUTES_DEFAULT), _config(config) {}

    /**
     * @brief Destroy the device
     *
     */
    ~BacklightCustom() override;

    /**
     * @brief Startup the device
     *
     * @return true if success, otherwise false
     *
     */
    bool begin(void) override;

    /**
     * @brief Delete the device, release the resources
     *
     * @note  After calling this function, users should call `begin()` to re-init the device
     *
     * @return true if success, otherwise false
     *
     */
    bool del(void) override;

    /**
     * @brief Set the brightness by percent
     *
     * @note  This function should be called after `begin()`
     *
     * @param[in] percent The brightness percent (0-100)
     *
     * @return true if success, otherwise false
     *
     */
    bool setBrightness(uint8_t percent) override;

private:
    Config _config;
};

} // namespace esp_panel::drivers
