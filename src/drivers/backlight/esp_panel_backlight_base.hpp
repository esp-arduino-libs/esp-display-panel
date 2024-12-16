/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <string>

namespace esp_panel::drivers {

/**
 * @brief The backlight base class
 *
 * @note  This class is a base class for all devices. Due to it is a virtual class, users cannot construct it directly
 */
class Backlight {
public:
    /**
     * @brief The backlight basic attributes structure
     */
    struct BasicAttributes {
        int type = -1;                  /*!< The device type. Default is `-1` */
        const char *name = "Unknown";   /*!< The device name. Default is `Unknown` */
    };

    /**
     * @brief The driver state enumeration
     */
    enum class State : uint8_t {
        DEINIT = 0,    /*!< Driver is not initialized */
        BEGIN,         /*!< Driver is initialized and ready */
    };

    /**
     * @brief Construct a new device
     *
     * @param[in] attr The backlight basic attributes
     */
    Backlight(const BasicAttributes &attr): _basic_attributes(attr) {}

    /**
     * @brief Destroy the device
     */
    virtual ~Backlight() = default;

    /**
     * @brief Startup the device
     *
     * @return true if success, otherwise false
     */
    virtual bool begin() = 0;

    /**
     * @brief Delete the device, release the resources
     *
     * @note  After calling this function, users should call `begin()` to re-init the device
     *
     * @return true if success, otherwise false
     */
    virtual bool del() = 0;

    /**
     * @brief Set the brightness by percent
     *
     * @note  This function should be called after `begin()`
     *
     * @param[in] percent The brightness percent (0-100)
     *
     * @return true if success, otherwise false
     */
    virtual bool setBrightness(uint8_t percent) = 0;

    /**
     * @brief Turn on the backlight
     *
     * @note  This function should be called after `begin()`
     * @note  This function is same as `setBrightness(100)`
     *
     * @return true if success, otherwise false
     */
    bool on();

    /**
     * @brief Turn off the backlight
     *
     * @note  This function should be called after `begin()`
     * @note  This function is same as `setBrightness(0)`
     *
     * @return true if success, otherwise false
     */
    bool off();

    /**
     * @brief Check if the driver has reached or passed the specified state
     *
     * @param[in] state The state to check against current state
     *
     * @return true if current state >= given state, otherwise false
     */
    bool isOverState(State state)
    {
        return (_state >= state);
    }

    /**
     * @brief Get the device basic attributes
     *
     * @return The backlight basic attributes
     */
    const BasicAttributes &getBasicAttributes()
    {
        return _basic_attributes;
    }

protected:
    void setState(State state)
    {
        _state = state;
    }

private:
    State _state = State::DEINIT;
    BasicAttributes _basic_attributes = {};
};

} // namespace esp_panel::drivers

/**
 * @deprecated Deprecated. Please use `esp_panel::drivers::Backlight`
 *             instead.
 *
 * @TODO: Remove in the next major version
 */
typedef esp_panel::drivers::Backlight ESP_PanelBacklight __attribute__((deprecated("Deprecated and will be removed \
in the next major version. Please use `esp_panel::drivers::Backlight` instead.")));
