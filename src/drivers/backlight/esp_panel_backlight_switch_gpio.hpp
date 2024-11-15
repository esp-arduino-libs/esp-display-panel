/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "driver/gpio.h"
#include "esp_panel_backlight_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief The switch(GPIO) backlight device class
 *
 * @note  The class is a derived class of `Backlight`, users can use it to construct a custom backlight device
 *
 */
class BacklightSwitchGPIO: public Backlight {
public:
    /**
     * @brief The default attributes of switch(GPIO) backlight device
     *
     */
    constexpr static Attributes ATTRIBUTES_DEFAULT = {
        .name = "switch(GPIO)",
    };

    /**
     * @brief The configuration for switch(GPIO) backlight device
     *
     */
    struct Config {
        /**
         * @brief Get the GPIO configuration
         *
         * @return The GPIO configuration
         *
         */
        gpio_config_t getGPIO_Config(void) const
        {
            return {
                .pin_bit_mask = BIT64(io_num),
                .mode = GPIO_MODE_OUTPUT,
                .pull_up_en = GPIO_PULLUP_DISABLE,
                .pull_down_en = GPIO_PULLDOWN_DISABLE,
                .intr_type = GPIO_INTR_DISABLE,
            };
        }

        /**
         * @brief Create a new configuration
         *
         * @param[in] io_num   GPIO number
         * @param[in] on_level Level when light up
         *
         * @return The configuration
         *
         */
        static Config create(int io_num, int on_level)
        {
            return {
                .io_num = io_num,
                .on_level = on_level,
            };
        }

        int io_num = -1;    /*<! GPIO number. Default is `-1` */
        int on_level = 1;   /*<! Level when light up. Default is `1` */
    };

// *INDENT-OFF*
    /**
     * @brief Construct a new switch(GPIO) backlight device with simple parameters
     *
     * @param[in] io_num   GPIO number
     * @param[in] on_level Level when light up
     *
     */
    BacklightSwitchGPIO(int io_num, bool on_level):
        Backlight(ATTRIBUTES_DEFAULT),
        _config(Config::create(io_num, on_level)),
        _io_config(_config.getGPIO_Config())
    {
    }

    /**
     * @brief Construct a new custom device with complex parameters
     *
     * @param[in] config The switch(GPIO) backlight configuration
     *
     */
    BacklightSwitchGPIO(const Config &config):
        Backlight(ATTRIBUTES_DEFAULT),
        _config(config),
        _io_config(config.getGPIO_Config())
    {
    }
// *INDENT-OFF*

    /**
     * @brief Destroy the device
     *
     */
    ~BacklightSwitchGPIO() override;

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
    Config _config = {};
    gpio_config_t _io_config = {};
};

} // namespace esp_panel::drivers
