/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>
#include <variant>
#include "driver/spi_master.h"
#include "esp_panel_types.h"
#include "drivers/host/esp_panel_host_spi.hpp"
#include "esp_panel_bus_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief The QSPI bus class
 *
 * @note  The class is a derived class of `Bus`, users can use it to construct the QSPI bus
 */
class BusQSPI: public Bus {
public:
    /**
     * Here are some default values for QSPI bus
     */
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {
        .type = ESP_PANEL_BUS_TYPE_QSPI,
        .name = "QSPI",
    };
    static constexpr int QSPI_HOST_ID_DEFAULT = static_cast<int>(SPI2_HOST);
    static constexpr int QSPI_PCLK_HZ_DEFAULT = SPI_MASTER_FREQ_40M;

    /**
     * @brief The QSPI bus configuration structure
     */
    struct Config {
        using HostFullConfig = spi_bus_config_t;
        using ControlPanelFullConfig = esp_lcd_panel_io_spi_config_t;

        struct HostPartialConfig {
            int sclk_io_num = -1;
            int data0_io_num = -1;
            int data1_io_num = -1;
            int data2_io_num = -1;
            int data3_io_num = -1;
        };
        struct ControlPanelPartialConfig {
            int cs_gpio_num = -1;
            int spi_mode = 0;
            int pclk_hz = QSPI_PCLK_HZ_DEFAULT;
            int lcd_cmd_bits = 8;
            int lcd_param_bits = 8;
        };

        void convertPartialToFull();
        void printHostConfig() const;
        void printControlPanelConfig() const;

        const HostFullConfig *getHostFullConfig() const
        {
            if (std::holds_alternative<HostPartialConfig>(host)) {
                return nullptr;
            }
            return &std::get<HostFullConfig>(host);
        }

        const ControlPanelFullConfig *getControlPanelFullConfig() const
        {
            if (std::holds_alternative<ControlPanelPartialConfig>(control_panel)) {
                return nullptr;
            }
            return &std::get<ControlPanelFullConfig>(control_panel);
        }

        // General
        int host_id = QSPI_HOST_ID_DEFAULT;
        // Host
        std::variant<HostFullConfig, HostPartialConfig> host = {};
        // Control Panel
        std::variant<ControlPanelFullConfig, ControlPanelPartialConfig> control_panel = {};
        // Extra
        bool skip_init_host = false;
    };

// *INDENT-OFF*
    /**
     * @brief Construct the QSPI bus with separate parameters, the host will be initialized by the driver
     *
     * @note  This function uses some default values to config the bus object, use `config*()` functions to change them
     *
     * @param[in] cs_io   QSPI CS pin
     * @param[in] sck_io  QSPI SCK pin
     * @param[in] d0_io   QSPI D0 pin
     * @param[in] d1_io   QSPI D1 pin
     * @param[in] d2_io   QSPI D2 pin
     * @param[in] d3_io   QSPI D3 pin
     */
    BusQSPI(int cs_io, int sck_io, int d0_io, int d1_io, int d2_io, int d3_io):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            // Host
            .host = Config::HostPartialConfig{
                .sclk_io_num = sck_io,
                .data0_io_num = d0_io,
                .data1_io_num = d1_io,
                .data2_io_num = d2_io,
                .data3_io_num = d3_io,
            },
            // Control Panel
            .control_panel = Config::ControlPanelPartialConfig{
                .cs_gpio_num = cs_io,
            },
            // Flags
            .skip_init_host = false,
        }
    {
    }

    /**
     * @brief Construct the QSPI bus with separate parameters, the host should be initialized by the user
     *
     * @note  This function uses some default values to config the bus object, use `config*()` functions to change them
     *
     * @param[in] host_id QSPI host ID
     * @param[in] cs_io   QSPI CS pin
     */
    BusQSPI(int host_id, int cs_io):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            // General
            .host_id = host_id,
            // Control Panel
            .control_panel = Config::ControlPanelPartialConfig{
                .cs_gpio_num = cs_io,
            },
            // Flags
            .skip_init_host = true,
        }
    {
    }

    /**
     * @brief Construct the QSPI bus with configuration
     *
     * @param[in] config QSPI bus configuration
     */
    BusQSPI(const Config &config):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config(config)
    {
    }
// *INDENT-OFF*

    /**
     * @brief Destroy the QSPI bus
     */
    ~BusQSPI() override;

    /**
     * Here are some functions to configure the bus. These functions should be called before `init()`
     */
    void configQSPI_Mode(uint8_t mode);
    void configQSPI_FreqHz(uint32_t hz);
    void configQSPI_TransQueueDepth(uint8_t depth);

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

    // TODO: Remove in the next major version
    [[deprecated("Deprecated. Please use `configQSPI_Mode()` instead")]]
    void configQspiMode(uint8_t mode)
    {
        configQSPI_Mode(mode);
    }
    [[deprecated("Deprecated. Please use `configQSPI_FreqHz()` instead")]]
    void configQspiFreqHz(uint32_t hz)
    {
        configQSPI_FreqHz(hz);
    }
    [[deprecated("Deprecated. Please use \
    `configQSPI_TransQueueDepth()` instead")]]
    void configQspiTransQueueDepth(uint8_t depth)
    {
        configQSPI_TransQueueDepth(depth);
    }

private:
    Config::ControlPanelFullConfig &getControlPanelFullConfig()
    {
        if (std::holds_alternative<Config::ControlPanelPartialConfig>(_config.control_panel)) {
            _config.convertPartialToFull();
        }
        return std::get<Config::ControlPanelFullConfig>(_config.control_panel);
    }

    Config::HostFullConfig &getHostFullConfig()
    {
        if (std::holds_alternative<Config::HostPartialConfig>(_config.host)) {
            _config.convertPartialToFull();
        }
        return std::get<Config::HostFullConfig>(_config.host);
    }

    Config _config = {};
    std::shared_ptr<HostSPI> _host = nullptr;
};

} // namespace esp_panel::drivers

/**
 * @deprecated Deprecated. Please use `esp_panel::drivers::BusQSPI`
 *             instead.
 *
 * @TODO: Remove in the next major version
 */
typedef esp_panel::drivers::BusQSPI ESP_PanelBusQSPI __attribute__((deprecated("Deprecated and will be removed in the \
next major version. Please use `esp_panel::drivers::BusQSPI` instead.")));
