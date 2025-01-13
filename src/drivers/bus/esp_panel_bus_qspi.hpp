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
#include "esp_panel_bus.hpp"

namespace esp_panel::drivers {

/**
 * @brief The QSPI bus class for ESP Panel
 *
 * This class is derived from `Bus` class and provides QSPI bus implementation for ESP Panel
 */
class BusQSPI: public Bus {
public:
    /**
     * @brief Default values for QSPI bus configuration
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
        /**
         * @brief Host configuration types
         */
        using HostFullConfig = spi_bus_config_t;

        /**
         * @brief Control panel configuration types
         */
        using ControlPanelFullConfig = esp_lcd_panel_io_spi_config_t;

        /**
         * @brief Partial host configuration structure
         */
        struct HostPartialConfig {
            int sclk_io_num = -1;    ///< GPIO number for SCLK signal
            int data0_io_num = -1;   ///< GPIO number for DATA0 signal
            int data1_io_num = -1;   ///< GPIO number for DATA1 signal
            int data2_io_num = -1;   ///< GPIO number for DATA2 signal
            int data3_io_num = -1;   ///< GPIO number for DATA3 signal
        };

        /**
         * @brief Partial control panel configuration structure
         */
        struct ControlPanelPartialConfig {
            int cs_gpio_num = -1;        ///< GPIO number for CS signal
            int spi_mode = 0;            ///< QSPI mode (0-3)
            int pclk_hz = QSPI_PCLK_HZ_DEFAULT;  ///< QSPI clock frequency in Hz
            int lcd_cmd_bits = 8;        ///< Bits for LCD commands
            int lcd_param_bits = 8;      ///< Bits for LCD parameters
        };

        /**
         * @brief Convert partial configurations to full configurations
         */
        void convertPartialToFull();

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

        /**
         * @brief Get the full control panel configuration if available
         *
         * @return Pointer to full control panel configuration, `nullptr` if using partial configuration
         */
        const ControlPanelFullConfig *getControlPanelFullConfig() const;

        int host_id = QSPI_HOST_ID_DEFAULT;    ///< QSPI host ID
        std::variant<HostFullConfig, HostPartialConfig> host = {};  ///< Host configuration
        ///< Control panel configuration
        std::variant<ControlPanelFullConfig, ControlPanelPartialConfig> control_panel = {};
        bool skip_init_host = false;          ///< Skip host initialization if true
    };

// *INDENT-OFF*
    /**
     * @brief Construct a new QSPI bus instance with individual parameters
     *
     * Uses default values for most configurations. Call `config*()` functions to modify the default settings
     *
     * @param[in] cs_io  GPIO number for QSPI CS signal
     * @param[in] sck_io GPIO number for QSPI SCK signal
     * @param[in] d0_io  GPIO number for QSPI D0 signal
     * @param[in] d1_io  GPIO number for QSPI D1 signal
     * @param[in] d2_io  GPIO number for QSPI D2 signal
     * @param[in] d3_io  GPIO number for QSPI D3 signal
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
     * @brief Construct a new QSPI bus instance with pre-initialized host
     *
     * @param[in] host_id QSPI host ID
     * @param[in] cs_io   GPIO number for CS signal
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
     * @brief Construct a new QSPI bus instance with complete configuration
     *
     * @param[in] config Complete QSPI bus configuration
     */
    BusQSPI(const Config &config):
        Bus(BASIC_ATTRIBUTES_DEFAULT),
        _config(config)
    {
    }
// *INDENT-ON*

    /**
     * @brief Destroy the QSPI bus instance
     */
    ~BusQSPI() override;

    /**
     * @brief Configure QSPI mode
     *
     * @param[in] mode QSPI mode (0-3)
     * @note This function should be called before `init()`
     */
    void configQSPI_Mode(uint8_t mode);

    /**
     * @brief Configure QSPI clock frequency
     *
     * @param[in] hz Clock frequency in Hz
     * @note This function should be called before `init()`
     */
    void configQSPI_FreqHz(uint32_t hz);

    /**
     * @brief Configure QSPI transaction queue depth
     *
     * @param[in] depth Queue depth for QSPI transactions
     * @note This function should be called before `init()`
     */
    void configQSPI_TransQueueDepth(uint8_t depth);

    /**
     * @brief Initialize the QSPI bus
     *
     * @return `true` if initialization succeeds, `false` otherwise
     */
    bool init() override;

    /**
     * @brief Start the QSPI bus operation
     *
     * @return `true` if startup succeeds, `false` otherwise
     */
    bool begin() override;

    /**
     * @brief Delete the QSPI bus instance and release resources
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
     * @brief Configure QSPI mode
     *
     * @param[in] mode QSPI mode (0-3)
     * @deprecated Use `configQSPI_Mode()` instead
     */
    [[deprecated("Use `configQSPI_Mode()` instead")]]
    void configQspiMode(uint8_t mode)
    {
        configQSPI_Mode(mode);
    }

    /**
     * @brief Configure QSPI clock frequency
     *
     * @param[in] hz Clock frequency in Hz
     * @deprecated Use `configQSPI_FreqHz()` instead
     */
    [[deprecated("Use `configQSPI_FreqHz()` instead")]]
    void configQspiFreqHz(uint32_t hz)
    {
        configQSPI_FreqHz(hz);
    }

    /**
     * @brief Configure QSPI transaction queue depth
     *
     * @param[in] depth Queue depth for QSPI transactions
     * @deprecated Use `configQSPI_TransQueueDepth()` instead
     */
    [[deprecated("Use `configQSPI_TransQueueDepth()` instead")]]
    void configQspiTransQueueDepth(uint8_t depth)
    {
        configQSPI_TransQueueDepth(depth);
    }

private:
    /**
     * @brief Get mutable reference to control panel full configuration
     *
     * Converts partial configuration to full configuration if necessary
     *
     * @return Reference to control panel full configuration
     */
    Config::ControlPanelFullConfig &getControlPanelFullConfig();

    /**
     * @brief Get mutable reference to host full configuration
     *
     * Converts partial configuration to full configuration if necessary
     *
     * @return Reference to host full configuration
     */
    Config::HostFullConfig &getHostFullConfig();

    Config _config = {};                      ///< QSPI bus configuration
    std::shared_ptr<HostSPI> _host = nullptr; ///< QSPI host instance
};

} // namespace esp_panel::drivers

/**
 * @brief Alias for backward compatibility
 *
 * @deprecated Use `esp_panel::drivers::BusQSPI` instead
 */
using ESP_PanelBusQSPI [[deprecated("Use `esp_panel::drivers::BusQSPI` instead")]] = esp_panel::drivers::BusQSPI;
