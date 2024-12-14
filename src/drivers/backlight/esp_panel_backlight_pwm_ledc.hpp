/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "driver/ledc.h"
#include "esp_idf_version.h"
#include "esp_panel_types.h"
#include "esp_panel_backlight_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief The PWM(LEDC) backlight device class
 *
 * @note  The class is a derived class of `Backlight`, users can use it to construct a custom backlight device
 *
 */
class BacklightPWM_LEDC: public Backlight {
public:
    /**
     * @brief The default attributes of PWM(LEDC) backlight device
     *
     */
    constexpr static Attributes ATTRIBUTES_DEFAULT = {
        .type = ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC,
        .name = "PWM(LEDC)",
    };

    /**
     * @brief The default values for LEDC
     *
     */
    constexpr static ledc_timer_t LEDC_TIMER_NUM = LEDC_TIMER_0;
    constexpr static ledc_mode_t LEDC_SPEED_MODE = LEDC_LOW_SPEED_MODE;

    /**
     * @brief The configuration for PWM(LEDC) backlight device
     *
     */
    struct Config {
        /**
         * @brief Get the LEDC timer configuration
         *
         * @return The LEDC timer configuration
         *
         */
        ledc_timer_config_t getTimerConfig(void) const
        {
            return {
                .speed_mode = LEDC_SPEED_MODE,
                .duty_resolution = LEDC_TIMER_10_BIT,
                .timer_num = LEDC_TIMER_NUM,
                .freq_hz = 5000,
                .clk_cfg = LEDC_AUTO_CLK,
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 3, 0)
                .deconfigure = false,
#endif
            };
        }

        /**
         * @brief Get the LEDC channel configuration
         *
         * @return The LEDC channel configuration
         *
         */
        ledc_channel_config_t getChannelConfig(void) const
        {
            return {
                .gpio_num = io_num,
                .speed_mode = LEDC_SPEED_MODE,
                .channel = LEDC_CHANNEL_0,
                .intr_type = LEDC_INTR_DISABLE,
                .timer_sel = LEDC_TIMER_NUM,
                .duty = 0,
                .hpoint = 0,
                .flags = {
                    .output_invert = !on_level,
                },
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
        static Config create(int io_num, bool on_level)
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
     * @brief Construct a new PWM(LEDC) backlight device with simple parameters
     *
     * @param[in] io_num   GPIO number
     * @param[in] on_level Level when light up
     *
     */
    BacklightPWM_LEDC(int io_num, bool on_level):
        Backlight(ATTRIBUTES_DEFAULT)
    {
        auto config = Config::create(io_num, on_level);
        _timer_config = config.getTimerConfig();
        _channel_config = config.getChannelConfig();
    }

    /**
     * @brief Construct a new custom device with complex parameters
     *
     * @param[in] config The PWM(LEDC) backlight configuration
     *
     */
    BacklightPWM_LEDC(const Config &config):
        Backlight(ATTRIBUTES_DEFAULT)
    {
        _timer_config = config.getTimerConfig();
        _channel_config = config.getChannelConfig();
    }


    /**
     * @deprecated Deprecated and will be removed in the next major version. Please use other constructors instead
     *
     * @TODO: Remove in the next major version
     *
     */
    [[deprecated("Deprecated and will be removed in the next major version. Please use other constructors instead")]]
    BacklightPWM_LEDC(int io_num, bool light_up_level, bool use_pwm): BacklightPWM_LEDC(io_num, light_up_level) {}
// *INDENT-OFF*

    /**
     * @brief Destroy the device
     *
     */
    ~BacklightPWM_LEDC() override;

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
    ledc_timer_config_t _timer_config = {};
    ledc_channel_config_t _channel_config = {};
};

} // namespace esp_panel::drivers
