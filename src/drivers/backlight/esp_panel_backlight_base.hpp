/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <string>

namespace esp_panel::drivers {

/**
 * @brief The base backlight device class
 *
 * @note  This class is a base class for all devices. Due to it is a virtual class, users cannot construct it directly
 *
 */
class Backlight {
public:
    /**
     * @brief The attributes of base backlight device
     *
     */
    struct Attributes {
        int type = -1;                  /*<! The backlight device type. Default is `-1` */
        const char *name = nullptr;     /*<! The backlight device name. Default is `nullptr` */
    };

    /**
     * @brief Construct a new device
     *
     * @param[in] attr The backlight attributes
     *
     */
    Backlight(const Attributes &attr): _attributes(attr) {}

    /**
     * @brief Destroy the device
     *
     */
    virtual ~Backlight() = default;

    /**
     * @brief Startup the device
     *
     * @return true if success, otherwise false
     *
     */
    virtual bool begin(void) = 0;

    /**
     * @brief Delete the device, release the resources
     *
     * @note  After calling this function, users should call `begin()` to re-init the device
     *
     * @return true if success, otherwise false
     *
     */
    virtual bool del(void) = 0;

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
    virtual bool setBrightness(uint8_t percent) = 0;

    /**
     * @brief Turn on the backlight
     *
     * @note  This function should be called after `begin()`
     * @note  This function is same as `setBrightness(100)`
     *
     * @return true if success, otherwise false
     *
     */
    bool on(void);

    /**
     * @brief Turn off the backlight
     *
     * @note  This function should be called after `begin()`
     * @note  This function is same as `setBrightness(0)`
     *
     * @return true if success, otherwise false
     *
     */
    bool off(void);

    /**
     * @brief Get the device attributes
     *
     * @return The backlight attributes
     *
     */
    const Attributes &getAttributes(void)
    {
        return _attributes;
    }

protected:
    /**
     * @brief Check if the device is already begun
     *
     * @return true if the device is already begun, otherwise false
     *
     */
    bool checkIsBegun(void)
    {
        return flags.is_begun;
    }

    struct {
        uint8_t is_begun: 1;
    } flags = {};

private:
    Attributes _attributes = {};
};

} // namespace esp_panel::drivers

/**
 * @deprecated Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Backlight` instead.
 *
 * @TODO: Remove in the next major version
 *
 */
typedef esp_panel::drivers::Backlight ESP_PanelBacklight __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Backlight` instead.")));
