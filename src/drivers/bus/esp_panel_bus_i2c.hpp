/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>
#include <variant>
#include "driver/i2c.h"
#include "esp_panel_types.h"
#include "drivers/host/esp_panel_host_i2c.hpp"
#include "esp_panel_bus_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief The I2C bus class
 *
 * @note  The class is a derived class of `Bus`, users can use it to construct the I2C bus
 */
class BusI2C: public Bus {
public:
    /**
     * Here are some default values for I2C bus
     */
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {
        .type = ESP_PANEL_BUS_TYPE_I2C,
        .name = "I2C",
    };
    static constexpr int I2C_HOST_ID_DEFAULT = static_cast<int>(I2C_NUM_0);
    static constexpr int I2C_CLK_SPEED_DEFAULT = 400 * 1000;

    /**
     * @brief The I2C bus configuration structure
     */
    struct Config {
        using HostFullConfig = i2c_config_t;
        using ControlPanelFullConfig = esp_lcd_panel_io_i2c_config_t;

        struct HostPartialConfig {
            int sda_io_num = -1;
            int scl_io_num = -1;
            bool sda_pullup_en = GPIO_PULLUP_ENABLE;
            bool scl_pullup_en = GPIO_PULLUP_ENABLE;
            int clk_speed = I2C_CLK_SPEED_DEFAULT;
        };

        void convertPartialToFull();
        void printGeneralConfig() const;
        void printHostConfig() const;
        void printControlPanelConfig() const;

        const HostFullConfig *getHostFullConfig() const
        {
            if (std::holds_alternative<HostPartialConfig>(host)) {
                return nullptr;
            }
            return &std::get<HostFullConfig>(host);
        }

        // General
        int host_id = I2C_HOST_ID_DEFAULT;
        bool skip_init_host = false;
        // Host
        std::variant<HostFullConfig, HostPartialConfig> host = {};
        // Control Panel
        ControlPanelFullConfig control_panel = {};
    };

// *INDENT-OFF*
    /**
     * @brief Construct the I2C bus with separate parameters, the host will be initialized by the driver
     *
     * @note  This function uses some default values to config the bus, use `config*()` functions to change them
     *
     * @param[in] scl_io I2C SCL pin
     * @param[in] sda_io I2C SDA pin
     * @param[in] control_panel_config I2C control panel configuration
     */
    BusI2C(int scl_io, int sda_io, const Config::ControlPanelFullConfig &control_panel_config):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            // General
            .skip_init_host = false,
            // Host
            .host = Config::HostPartialConfig{
                .sda_io_num = sda_io,
                .scl_io_num = scl_io,
            },
            // Control Panel
            .control_panel = control_panel_config,
        }
    {
    }

    /**
     * @brief Construct the I2C bus with separate parameters, the host should be initialized by the user
     *
     * @param[in] host_id I2C host ID
     * @param[in] control_panel_config I2C control panel configuration
     */
    BusI2C(int host_id, const Config::ControlPanelFullConfig &control_panel_config):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            // General
            .host_id = host_id,
            .skip_init_host = true,
            // Control Panel
            .control_panel = control_panel_config,
        }
    {
    }

    /**
     * @brief Construct the I2C bus with configuration
     *
     * @param[in] config The I2C bus configuration
     */
    BusI2C(const Config &config):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config(config)
    {
    }
// *INDENT-OFF*

    /**
     * @brief Destroy the I2C bus
     */
    ~BusI2C() override;

    /**
     * Here are some functions to configure the bus. These functions should be called before `init()`
     */
    void configI2C_PullupEnable(bool sda_pullup_en, bool scl_pullup_en);
    void configI2C_FreqHz(uint32_t hz);
    void configI2C_Address(uint32_t address);
    void configI2C_CtrlPhaseBytes(uint32_t num);
    void configI2C_DC_BitOffset(uint32_t num);
    void configI2C_CommandBits(uint32_t num);
    void configI2C_ParamBits(uint32_t num);
    void configI2C_Flags(bool dc_low_on_data, bool disable_control_phase);

    /**
     * @brief Initialize the bus
     *
     * @return true if success, otherwise false
     */
    bool init() override;

    /**
     * @brief Startup the bus
     *
     * @return true if success, otherwise false
     */
    bool begin() override;

    /**
     * @brief Delete the bus, release the resources
     *
     * @return true if success, otherwise false
     */
    bool del() override;

    /**
     * @brief Get the bus configuration
     *
     * @return Bus configuration
     */
    const Config &getConfig() const
    {
        return _config;
    }

    /**
     * @brief Get the I2C address
     *
     * @return 7-bit I2C address
     */
    uint8_t getI2cAddress() const
    {
        return static_cast<uint8_t>(getConfig().control_panel.dev_addr);
    }

    // TODO: Remove in the next major version
    [[deprecated("Deprecated. Please use `configI2C_PullupEnable()` \
    instead")]]
    void configI2cPullupEnable(bool sda_pullup_en, bool scl_pullup_en)
    {
        configI2C_PullupEnable(sda_pullup_en, scl_pullup_en);
    }
    [[deprecated("Deprecated. Please use `configI2C_FreqHz()` instead")]]
    void configI2cFreqHz(uint32_t hz)
    {
        configI2C_FreqHz(hz);
    }
    [[deprecated("Deprecated. Please use `configI2C_Address()` instead")]]
    void configI2cAddress(uint32_t address)
    {
        configI2C_Address(address);
    }
    [[deprecated("Deprecated. Please use `configI2C_CtrlPhaseBytes()` \
    instead")]]
    void configI2cCtrlPhaseBytes(uint32_t num)
    {
        configI2C_CtrlPhaseBytes(num);
    }
    [[deprecated("Deprecated. Please use `configI2C_DC_BitOffset()` \
    instead")]]
    void configI2cDcBitOffset(uint32_t num)
    {
        configI2C_DC_BitOffset(num);
    }
    [[deprecated("Deprecated. Please use `configI2C_CommandBits()` \
    instead")]]
    void configI2cCommandBits(uint32_t num)
    {
        configI2C_CommandBits(num);
    }
    [[deprecated("Deprecated. Please use `configI2C_ParamBits()` \
    instead")]]
    void configI2cParamBits(uint32_t num)
    {
        configI2C_ParamBits(num);
    }
    [[deprecated("Deprecated. Please use `configI2C_Flags()` \
    instead")]]
    void configI2cFlags(bool dc_low_on_data, bool disable_control_phase)
    {
        configI2C_Flags(dc_low_on_data, disable_control_phase);
    }

private:
    Config::HostFullConfig &getHostFullConfig()
    {
        if (std::holds_alternative<Config::HostPartialConfig>(_config.host)) {
            _config.convertPartialToFull();
        }
        return std::get<Config::HostFullConfig>(_config.host);
    }

    Config::ControlPanelFullConfig &getControlPanelFullConfig()
    {
        return _config.control_panel;
    }

    Config _config = {};
    std::shared_ptr<HostI2C> _host = nullptr;
};

} // namespace esp_panel::drivers

/**
 * @deprecated Deprecated. Please use `esp_panel::drivers::BusI2C`
 *             instead.
 *
 * @TODO: Remove in the next major version
 */
typedef esp_panel::drivers::BusI2C ESP_PanelBusI2C __attribute__((deprecated("Deprecated and will be removed in the \
next major version. Please use `esp_panel::drivers::BusI2C` instead.")));
