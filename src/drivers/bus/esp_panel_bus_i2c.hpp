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
#include "esp_panel_bus.hpp"

namespace esp_panel::drivers {

/**
 * @brief The I2C bus class
 *
 * This class is derived from `Bus` class and provides I2C bus implementation for ESP Panel
 */
class BusI2C: public Bus {
public:
    /**
     * @brief Default values for I2C bus configuration
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
        /**
         * @brief Host configuration types
         */
        using HostFullConfig = i2c_config_t;

        /**
         * @brief Control panel configuration types
         */
        using ControlPanelFullConfig = esp_lcd_panel_io_i2c_config_t;

        /**
         * @brief Partial host configuration structure
         */
        struct HostPartialConfig {
            int sda_io_num = -1;         ///< GPIO number for SDA signal
            int scl_io_num = -1;         ///< GPIO number for SCL signal
            bool sda_pullup_en = GPIO_PULLUP_ENABLE;  ///< Enable internal pullup for SDA
            bool scl_pullup_en = GPIO_PULLUP_ENABLE;  ///< Enable internal pullup for SCL
            int clk_speed = I2C_CLK_SPEED_DEFAULT;    ///< I2C clock frequency in Hz
        };

        /**
         * @brief Convert partial configurations to full configurations
         */
        void convertPartialToFull();

        /**
         * @brief Print general configuration for debugging
         */
        void printGeneralConfig() const;

        /**
         * @brief Print host configuration for debugging
         */
        void printHostConfig() const;

        /**
         * @brief Print control panel configuration for debugging
         */
        void printControlPanelConfig() const;

        /**
         * @brief Get the full host configuration if available
         *
         * @return Pointer to full host configuration, `nullptr` if using partial configuration
         */
        const HostFullConfig *getHostFullConfig() const;

        int host_id = I2C_HOST_ID_DEFAULT;    ///< I2C host ID
        bool skip_init_host = false;          ///< Skip host initialization if true
        std::variant<HostFullConfig, HostPartialConfig> host = {};  ///< Host configuration
        ControlPanelFullConfig control_panel = {};  ///< Control panel configuration
    };

// *INDENT-OFF*
    /**
     * @brief Construct a new I2C bus instance with individual parameters
     *
     * Uses default values for most configurations. Call `config*()` functions to modify the default settings
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
     * @brief Construct a new I2C bus instance with pre-initialized host
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
     * @brief Construct a new I2C bus instance with complete configuration
     *
     * @param[in] config Complete I2C bus configuration
     */
    BusI2C(const Config &config):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config(config)
    {
    }
// *INDENT-ON*

    /**
     * @brief Destroy the I2C bus instance
     */
    ~BusI2C() override;

    /**
     * @brief Configure I2C internal pullup
     *
     * @param[in] sda_pullup_en Enable internal pullup for SDA
     * @param[in] scl_pullup_en Enable internal pullup for SCL
     * @note This function should be called before `init()`
     */
    void configI2C_PullupEnable(bool sda_pullup_en, bool scl_pullup_en);

    /**
     * @brief Configure I2C clock frequency
     *
     * @param[in] hz Clock frequency in Hz
     * @note This function should be called before `init()`
     */
    void configI2C_FreqHz(uint32_t hz);

    /**
     * @brief Configure I2C device address
     *
     * @param[in] address 7-bit I2C device address
     * @note This function should be called before `init()`
     */
    void configI2C_Address(uint32_t address);

    /**
     * @brief Configure number of bytes in control phase
     *
     * @param[in] num Number of bytes in control phase
     * @note This function should be called before `init()`
     */
    void configI2C_CtrlPhaseBytes(uint32_t num);

    /**
     * @brief Configure DC bit offset in control phase
     *
     * @param[in] num DC bit offset
     * @note This function should be called before `init()`
     */
    void configI2C_DC_BitOffset(uint32_t num);

    /**
     * @brief Configure number of bits for I2C commands
     *
     * @param[in] num Number of bits for commands
     * @note This function should be called before `init()`
     */
    void configI2C_CommandBits(uint32_t num);

    /**
     * @brief Configure number of bits for I2C parameters
     *
     * @param[in] num Number of bits for parameters
     * @note This function should be called before `init()`
     */
    void configI2C_ParamBits(uint32_t num);

    /**
     * @brief Configure I2C flags
     *
     * @param[in] dc_low_on_data Set DC low on data phase
     * @param[in] disable_control_phase Disable control phase
     * @note This function should be called before `init()`
     */
    void configI2C_Flags(bool dc_low_on_data, bool disable_control_phase);

    /**
     * @brief Initialize the I2C bus
     *
     * @return `true` if initialization succeeds, `false` otherwise
     */
    bool init() override;

    /**
     * @brief Start the I2C bus operation
     *
     * @return `true` if startup succeeds, `false` otherwise
     */
    bool begin() override;

    /**
     * @brief Delete the I2C bus instance and release resources
     *
     * @return `true` if deletion succeeds, `false` otherwise
     */
    bool del() override;

    /**
     * @brief Get the current bus configuration
     *
     * @return Reference to the current bus configuration
     */
    const Config &getConfig() const
    {
        return _config;
    }

    /**
     * @brief Get the I2C device address
     *
     * @return 7-bit I2C device address
     */
    uint8_t getI2cAddress() const
    {
        return static_cast<uint8_t>(getConfig().control_panel.dev_addr);
    }

    /**
     * @brief Configure I2C internal pullup
     *
     * @param[in] sda_pullup_en Enable internal pullup for SDA
     * @param[in] scl_pullup_en Enable internal pullup for SCL
     * @deprecated Use `configI2C_PullupEnable()` instead
     */
    [[deprecated("Use `configI2C_PullupEnable()` instead")]]
    void configI2cPullupEnable(bool sda_pullup_en, bool scl_pullup_en)
    {
        configI2C_PullupEnable(sda_pullup_en, scl_pullup_en);
    }

    /**
     * @brief Configure I2C clock frequency
     *
     * @param[in] hz Clock frequency in Hz
     * @deprecated Use `configI2C_FreqHz()` instead
     */
    [[deprecated("Use `configI2C_FreqHz()` instead")]]
    void configI2cFreqHz(uint32_t hz)
    {
        configI2C_FreqHz(hz);
    }

    /**
     * @brief Configure I2C device address
     *
     * @param[in] address 7-bit I2C device address
     * @deprecated Use `configI2C_Address()` instead
     */
    [[deprecated("Use `configI2C_Address()` instead")]]
    void configI2cAddress(uint32_t address)
    {
        configI2C_Address(address);
    }

    /**
     * @brief Configure number of bytes in control phase
     *
     * @param[in] num Number of bytes in control phase
     * @deprecated Use `configI2C_CtrlPhaseBytes()` instead
     */
    [[deprecated("Use `configI2C_CtrlPhaseBytes()` instead")]]
    void configI2cCtrlPhaseBytes(uint32_t num)
    {
        configI2C_CtrlPhaseBytes(num);
    }

    /**
     * @brief Configure DC bit offset in control phase
     *
     * @param[in] num DC bit offset
     * @deprecated Use `configI2C_DC_BitOffset()` instead
     */
    [[deprecated("Use `configI2C_DC_BitOffset()` instead")]]
    void configI2cDcBitOffset(uint32_t num)
    {
        configI2C_DC_BitOffset(num);
    }

    /**
     * @brief Configure number of bits for I2C commands
     *
     * @param[in] num Number of bits for commands
     * @deprecated Use `configI2C_CommandBits()` instead
     */
    [[deprecated("Use `configI2C_CommandBits()` instead")]]
    void configI2cCommandBits(uint32_t num)
    {
        configI2C_CommandBits(num);
    }

    /**
     * @brief Configure number of bits for I2C parameters
     *
     * @param[in] num Number of bits for parameters
     * @deprecated Use `configI2C_ParamBits()` instead
     */
    [[deprecated("Use `configI2C_ParamBits()` instead")]]
    void configI2cParamBits(uint32_t num)
    {
        configI2C_ParamBits(num);
    }

    /**
     * @brief Configure I2C flags
     *
     * @param[in] dc_low_on_data Set DC low on data phase
     * @param[in] disable_control_phase Disable control phase
     * @deprecated Use `configI2C_Flags()` instead
     */
    [[deprecated("Use `configI2C_Flags()` instead")]]
    void configI2cFlags(bool dc_low_on_data, bool disable_control_phase)
    {
        configI2C_Flags(dc_low_on_data, disable_control_phase);
    }

private:
    /**
     * @brief Get mutable reference to host full configuration
     *
     * Converts partial configuration to full configuration if necessary
     *
     * @return Reference to host full configuration
     */
    Config::HostFullConfig &getHostFullConfig();

    /**
     * @brief Get mutable reference to control panel full configuration
     *
     * @return Reference to control panel full configuration
     */
    Config::ControlPanelFullConfig &getControlPanelFullConfig();

    Config _config = {};                      ///< I2C bus configuration
    std::shared_ptr<HostI2C> _host = nullptr; ///< I2C host instance
};

} // namespace esp_panel::drivers

/**
 * @brief Alias for backward compatibility
 *
 * @deprecated Use `esp_panel::drivers::BusI2C` instead
 */
using ESP_PanelBusI2C [[deprecated("Use `esp_panel::drivers::BusI2C` instead")]] = esp_panel::drivers::BusI2C;
