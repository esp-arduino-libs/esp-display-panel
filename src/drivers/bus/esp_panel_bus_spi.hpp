/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <variant>
#include <memory>
#include "driver/spi_master.h"
#include "esp_panel_types.h"
#include "drivers/host/esp_panel_host_spi.hpp"
#include "esp_panel_bus_base.hpp"

namespace esp_panel::drivers {

/**
 * @brief The SPI bus class
 *
 * @note  The class is a derived class of `Bus`, users can use it to construct the SPI bus
 */
class BusSPI: public Bus {
public:
    /**
     * Here are some default values for SPI bus
     */
    static constexpr BasicAttributes BASIC_ATTRIBUTES_DEFAULT = {
        .type = ESP_PANEL_BUS_TYPE_SPI,
        .name = "SPI",
    };
    static constexpr int SPI_HOST_ID_DEFAULT = static_cast<int>(SPI2_HOST);
    static constexpr int SPI_PCLK_HZ_DEFAULT = SPI_MASTER_FREQ_40M;

    /**
     * @brief The SPI bus configuration structure
     */
    struct Config {
        using HostFullConfig = spi_bus_config_t;
        using ControlPanelFullConfig = esp_lcd_panel_io_spi_config_t;

        struct HostPartialConfig {
            int mosi_io_num = -1;
            int miso_io_num = -1;
            int sclk_io_num = -1;
        };
        struct ControlPanelPartialConfig {
            int cs_gpio_num = -1;
            int dc_gpio_num = -1;
            int spi_mode = 0;
            int pclk_hz = SPI_PCLK_HZ_DEFAULT;
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
        int host_id = SPI_HOST_ID_DEFAULT;
        // Host
        std::variant<HostFullConfig, HostPartialConfig> host = {};
        // Control Panel
        std::variant<ControlPanelFullConfig, ControlPanelPartialConfig> control_panel = {};
        // Extra
        bool skip_init_host = false;
    };

// *INDENT-OFF*
    /**
     * @brief Construct the SPI bus with separate parameters, the host will be initialized by the driver
     *
     * @note  This function uses some default values to config the bus object, use `config*()` functions to change them
     *
     * @param[in] cs_io  SPI CS pin
     * @param[in] dc_io  SPI DC pin
     * @param[in] sck_io SPI SCK pin
     * @param[in] sda_io SPI SDA (MOSI) pin
     * @param[in] sdo_io SPI SDO (MISO) pin, set to -1 if not used
     */
    BusSPI(int cs_io, int dc_io, int sck_io, int sda_io, int sdo_io = -1):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            // Host
            .host = Config::HostPartialConfig{
                .mosi_io_num = sda_io,
                .miso_io_num = sdo_io,
                .sclk_io_num = sck_io,
            },
            // Control Panel
            .control_panel = Config::ControlPanelPartialConfig{
                .cs_gpio_num = cs_io,
                .dc_gpio_num = dc_io,
            },
            // Extra
            .skip_init_host = false,
        }
    {
    }

    /**
     * @brief Construct the SPI bus with separate parameters, the host should be initialized by the user
     *
     * @note  This function uses some default values to config the bus object, use `config*()` functions to change them
     *
     * @param[in] host_id SPI host ID
     * @param[in] cs_io   SPI CS pin
     * @param[in] dc_io   SPI DC pin
     */
    BusSPI(int host_id, int cs_io, int dc_io):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            // General
            .host_id = host_id,
            // Control Panel
            .control_panel = Config::ControlPanelPartialConfig{
                .cs_gpio_num = cs_io,
                .dc_gpio_num = dc_io,
            },
            // Extra
            .skip_init_host = true,
        }
    {
    }

    /**
     * @brief Construct the SPI bus with separate parameters, the host will be initialized by the driver
     *
     * @note  This function uses some default values to config the bus object, use `config*()` functions to change them
     *
     * @param[in] sck_io               SPI SCK pin
     * @param[in] sda_io               SPI SDA (MOSI) pin
     * @param[in] sdo_io               SPI SDO (MISO) pin
     * @param[in] control_panel_config SPI control panel configuration
     */
    BusSPI(int sck_io, int sda_io, int sdo_io, const Config::ControlPanelFullConfig &control_panel_config):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            // Host
            .host = Config::HostPartialConfig{
                .mosi_io_num = sda_io,
                .miso_io_num = sdo_io,
                .sclk_io_num = sck_io,
            },
            // Control Panel
            .control_panel = control_panel_config,
            // Extra
            .skip_init_host = false,
        }
    {
    }

    /**
     * @brief Construct the SPI bus with separate parameters, the host should be initialized by the user
     *
     * @note  This function uses some default values to config the bus object, use `config*()` functions to change them
     *
     * @param[in] host_id              SPI host ID
     * @param[in] control_panel_config SPI control panel configuration
     */
    BusSPI(int host_id, const Config::ControlPanelFullConfig &control_panel_config):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config{
            // General
            .host_id = host_id,
            // Control Panel
            .control_panel = control_panel_config,
            // Extra
            .skip_init_host = true,
        }
    {
    }

    /**
     * @brief Construct the SPI bus with configuration
     *
     * @param[in] config SPI bus configuration
     */
    BusSPI(const Config &config):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config(config)
    {
    }
// *INDENT-OFF*

    /**
     * @brief Destroy the SPI bus
     */
    ~BusSPI() override;

    /**
     * Here are some functions to configure the bus. These functions should be called before `begin()`
     */
    void configSPI_Mode(uint8_t mode);
    void configSPI_FreqHz(uint32_t hz);
    void configSPI_CommandBits(uint32_t num);
    void configSPI_ParamBits(uint32_t num);
    void configSPI_TransQueueDepth(uint8_t depth);

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
     * @brief Delete the bus object, release the resources
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
    [[deprecated("Deprecated. Please use `configSPI_Mode()` instead")]]
    void configSpiMode(uint8_t mode)
    {
        configSPI_Mode(mode);
    }
    [[deprecated("Deprecated. Please use `configSPI_FreqHz()` instead")]]
    void configSpiFreqHz(uint32_t hz)
    {
        configSPI_FreqHz(hz);
    }
    [[deprecated("Deprecated. Please use `configSPI_CommandBits()` \
    instead")]]
    void configSpiCommandBits(uint32_t num)
    {
        configSPI_CommandBits(num);
    }
    [[deprecated("Deprecated. Please use `configSPI_ParamBits()` \
    instead")]]
    void configSpiParamBits(uint32_t num)
    {
        configSPI_ParamBits(num);
    }
    [[deprecated("Deprecated. Please  use `configSPI_TransQueueDepth()` \
    instead")]]
    void configSpiTransQueueDepth(uint8_t depth)
    {
        configSPI_TransQueueDepth(depth);
    }

protected:
    Config::HostFullConfig &getHostFullConfig()
    {
        if (std::holds_alternative<Config::HostPartialConfig>(_config.host)) {
            _config.convertPartialToFull();
        }
        return std::get<Config::HostFullConfig>(_config.host);
    }

    Config::ControlPanelFullConfig &getControlPanelFullConfig()
    {
        if (std::holds_alternative<Config::ControlPanelPartialConfig>(_config.control_panel)) {
            _config.convertPartialToFull();
        }
        return std::get<Config::ControlPanelFullConfig>(_config.control_panel);
    }

private:
    Config _config = {};
    std::shared_ptr<HostSPI> _host = nullptr;
};

} // namespace esp_panel::drivers

/**
 * @deprecated Deprecated. Please use `esp_panel::drivers::BusSPI`
 *             instead.
 *
 * @TODO: Remove in the next major version
 */
typedef esp_panel::drivers::BusSPI ESP_PanelBusSPI __attribute__((deprecated("Deprecated and will be removed in the \
next major version. Please use `esp_panel::drivers::BusSPI` instead.")));
