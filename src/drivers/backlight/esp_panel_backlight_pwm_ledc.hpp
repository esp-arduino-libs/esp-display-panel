/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <variant>
#include "driver/ledc.h"
#include "esp_idf_version.h"
#include "esp_panel_types.h"
#include "esp_panel_backlight_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief The PWM(LEDC) backlight device class
 *
 * @note  The class is a derived class of `Backlight`, users can use it to construct a custom backlight device
 */
class BacklightPWM_LEDC: public Backlight {
public:
    /**
     * Here are some default values for PWM(LEDC) backlight device
     */
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {
        .type = ESP_PANEL_BACKLIGHT_TYPE_PWM_LEDC,
        .name = "PWM(LEDC)",
    };
    static constexpr ledc_timer_t LEDC_TIMER_NUM_DEFAULT = LEDC_TIMER_0;
    static constexpr int LEDC_TIMER_FREQ_DEFAULT = 5000;
    static constexpr ledc_timer_bit_t LEDC_TIMER_BIT_DEFAULT = LEDC_TIMER_10_BIT;
    static constexpr ledc_mode_t LEDC_SPEED_MODE_DEFAULT = LEDC_LOW_SPEED_MODE;

    /**
     * @brief The configuration for PWM(LEDC) backlight device
     */
    struct Config {
        using LEDC_TimerFullConfig = ledc_timer_config_t;
        using LEDC_ChannelFullConfig = ledc_channel_config_t;

        struct LEDC_ChannelPartialConfig {
            int io_num = -1;    /*!< GPIO number. Default is `-1` */
            int on_level = 1;   /*!< Level when light up. Default is `1` */
        };

        void convertPartialToFull();
        void printLEDC_TimerConfig() const;
        void printLEDC_ChannelConfig() const;

        const LEDC_TimerFullConfig *getLEDC_FullTimerConfig() const
        {
            return &ledc_timer;
        }

        const LEDC_ChannelFullConfig *getLEDC_FullChannelConfig() const
        {
            if (std::holds_alternative<LEDC_ChannelPartialConfig>(ledc_channel)) {
                return nullptr;
            }
            return &std::get<LEDC_ChannelFullConfig>(ledc_channel);
        }

        /*!< LEDC timer configuration */
        LEDC_TimerFullConfig ledc_timer = {
            .speed_mode = LEDC_SPEED_MODE_DEFAULT,
            .duty_resolution = LEDC_TIMER_BIT_DEFAULT,
            .timer_num = LEDC_TIMER_NUM_DEFAULT,
            .freq_hz = LEDC_TIMER_FREQ_DEFAULT,
            .clk_cfg = LEDC_AUTO_CLK,
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 3, 0)
            .deconfigure = false,
#endif // ESP_IDF_VERSION
        };
        /*!< LEDC channel configuration */
        std::variant<LEDC_ChannelFullConfig, LEDC_ChannelPartialConfig> ledc_channel = {};
    };

// *INDENT-OFF*
    /**
     * @brief Construct the PWM(LEDC) backlight device with separate parameters
     *
     * @param[in] io_num   GPIO number
     * @param[in] on_level Level when light up
     */
    BacklightPWM_LEDC(int io_num, bool on_level):
        Backlight(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            .ledc_channel = Config::LEDC_ChannelPartialConfig{
                .io_num = io_num,
                .on_level = on_level,
            }
        }
    {
    }

    /**
     * @brief Construct the PWM(LEDC) backlight device with configuration
     *
     * @param[in] config The PWM(LEDC) backlight configuration
     */
    BacklightPWM_LEDC(const Config &config):
        Backlight(BASIC_ATTRIBUTES_DEFAULT),
        _config(config)
    {
    }

    /**
     * @deprecated Deprecated. Please use other constructors instead
     *
     * @TODO: Remove in the next major version
     */
    [[deprecated("Deprecated. Please use other constructors instead")]]
    BacklightPWM_LEDC(int io_num, bool light_up_level, bool use_pwm): BacklightPWM_LEDC(io_num, light_up_level) {}
// *INDENT-OFF*

    /**
     * @brief Destroy the device
     */
    ~BacklightPWM_LEDC() override;

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
    Config::LEDC_TimerFullConfig &getLEDC_TimerConfig()
    {
        return _config.ledc_timer;
    }

    Config::LEDC_ChannelFullConfig &getLEDC_ChannelConfig()
    {
        if (std::holds_alternative<Config::LEDC_ChannelPartialConfig>(_config.ledc_channel)) {
            _config.convertPartialToFull();
        }
        return std::get<Config::LEDC_ChannelFullConfig>(_config.ledc_channel);
    }

    Config _config = {};
};

} // namespace esp_panel::drivers
