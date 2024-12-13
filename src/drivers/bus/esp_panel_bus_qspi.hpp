/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>
#include "hal/spi_ll.h"
#include "driver/spi_master.h"
#include "esp_panel_types.h"
#include "esp_panel_bus_base.hpp"

namespace esp_panel::drivers {

// Forward declaration
class BusHost_SPI;

/**
 * @brief QSPI bus object class
 *
 * @note  This class is a derived class of `Bus`, user can use it directly
 */
class Bus_QSPI: public Bus {
public:
    constexpr static int HOST_ID_DEFAULT = static_cast<int>(SPI2_HOST);
    constexpr static Attributes ATTRIBUTES_DEFAULT = {
        .type = ESP_PANEL_BUS_TYPE_QSPI,
        .name = "QSPI",
    };

    struct Config {
        spi_host_device_t getHostId(void) const
        {
            return static_cast<spi_host_device_t>(host_id);
        }

        spi_bus_config_t getHostConfig(void) const
        {
            return {
                .data0_io_num = data0_io_num,
                .data1_io_num = data1_io_num,
                .sclk_io_num = sclk_io_num,
                .data2_io_num = data2_io_num,
                .data3_io_num = data3_io_num,
                .data4_io_num = -1,
                .data5_io_num = -1,
                .data6_io_num = -1,
                .data7_io_num = -1,
                .max_transfer_sz = (SPI_LL_DMA_MAX_BIT_LEN >> 3),
                .flags = SPICOMMON_BUSFLAG_MASTER,
                .intr_flags = 0,
            };
        }

        esp_lcd_panel_io_spi_config_t getIO_Config(void) const
        {
            return {
                .cs_gpio_num = cs_gpio_num,
                .dc_gpio_num = -1,
                .spi_mode = spi_mode,
                .pclk_hz = static_cast<unsigned int>(pclk_hz),
                .trans_queue_depth = 10,
                .on_color_trans_done = nullptr,
                .user_ctx = nullptr,
                .lcd_cmd_bits = lcd_cmd_bits,
                .lcd_param_bits = lcd_param_bits,
                .flags = {
                    .dc_low_on_data = 0,
                    .octal_mode = 0,
                    .quad_mode = 1,
                    .sio_mode = 0,
                    .lsb_first = 0,
                    .cs_high_active = 0,
                },
            };
        }

        static Config create(int cs_io, int sck_io, int d0_io, int d1_io, int d2_io, int d3_io)
        {
            return Config{
                // Host
                .data0_io_num = d0_io,
                .data1_io_num = d1_io,
                .sclk_io_num = sck_io,
                .data2_io_num = d2_io,
                .data3_io_num = d3_io,
                // Panel IO
                .cs_gpio_num = cs_io,
                // Flags
                .skip_init_host = false,
            };
        }

        static Config create(int cs_io)
        {
            return Config{
                // Panel IO
                .cs_gpio_num = cs_io,
                // Flags
                .skip_init_host = true,
            };
        }

        // Host
        int host_id = HOST_ID_DEFAULT;
        int data0_io_num = -1;
        int data1_io_num = -1;
        int sclk_io_num = -1;
        int data2_io_num = -1;
        int data3_io_num = -1;
        // Panel IO
        int cs_gpio_num = -1;
        int spi_mode = 0;
        int pclk_hz = SPI_MASTER_FREQ_40M;
        int lcd_cmd_bits = 8;
        int lcd_param_bits = 8;
        // Extra
        bool skip_init_host = false;
    };

// *INDENT-OFF*
    /**
     * @brief Construct a QSPI bus object in a common way, the host will be initialized by the driver
     *
     * @note  This function uses some default values (ESP_PANEL_IO_QSPI_CONFIG_DEFAULT) to config the bus object,
     *        use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param cs_io   QSPI CS pin
     * @param sck_io  QSPI SCK pin
     * @param d0_io   QSPI D0 pin
     * @param d1_io   QSPI D1 pin
     * @param d2_io   QSPI D2 pin
     * @param d3_io   QSPI D3 pin
     */
    Bus_QSPI(int cs_io, int sck_io, int d0_io, int d1_io, int d2_io, int d3_io):
        Bus(ATTRIBUTES_DEFAULT)
    {
        auto config = Config::create(cs_io, sck_io, d0_io, d1_io, d2_io, d3_io);
        _flags.skip_init_host = config.skip_init_host;
        _host_id = config.host_id;
        _host_config = config.getHostConfig();
        _io_config = config.getIO_Config();
    }

    /**
     * @brief Construct a QSPI bus object in a common way, the host needs to be initialized by the user
     *
     * @note  This function uses some default values (ESP_PANEL_IO_QSPI_CONFIG_DEFAULT) to config the bus object,
     *        use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param cs_io QSPI CS pin
     */
    Bus_QSPI(int cs_io):
        Bus(ATTRIBUTES_DEFAULT)
    {
        auto config = Config::create(cs_io);
        _flags.skip_init_host = config.skip_init_host;
        _host_id = config.host_id;
        _host_config = config.getHostConfig();
        _io_config = config.getIO_Config();
    }

    Bus_QSPI(const Config &config):
        Bus({ESP_PANEL_BUS_TYPE_QSPI})
    {
        _flags.skip_init_host = config.skip_init_host;
        _host_id = config.host_id;
        _host_config = config.getHostConfig();
        _io_config = config.getIO_Config();
    }
// *INDENT-OFF*

    /**
     * @brief Destroy the QSPI bus object
     *
     */
    ~Bus_QSPI() override;

    /**
     * @brief Here are some functions to configure the QSPI bus object. These functions should be called before `begin()`
     *
     */
    void configQspiMode(uint8_t mode);
    void configQspiFreqHz(uint32_t hz);
    void configQspiTransQueueDepth(uint8_t depth);

    bool init(void) override;

    /**
     * @brief Startup the bus
     *
     * @return true if success, otherwise false
     *
     */
    bool begin(void) override;

    /**
     * @brief Delete the bus object, release the resources
     *
     * @return true if success, otherwise false
     *
     */
    bool del(void) override;

    bool checkIsInit(void)
    {
        return _flags.is_init;
    }

private:
    struct {
        uint8_t is_init: 1;
        uint8_t skip_init_host: 1;
    } _flags = {};
    int _host_id = HOST_ID_DEFAULT;
    spi_bus_config_t _host_config = {};
    std::shared_ptr<BusHost_SPI> _host = nullptr;
    esp_lcd_panel_io_spi_config_t _io_config = {};
};

} // namespace esp_panel::drivers

/**
 * @deprecated This type is deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Bus_QSPI` instead.
 *
 * Example of migration:
 *
 * Old:
 *  ESP_PanelBus_QSPI bus;
 *
 * New:
 *  esp_panel::drivers::Bus_QSPI bus;
 */
typedef esp_panel::drivers::Bus_QSPI ESP_PanelBus_QSPI __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Bus_QSPI` instead.")));
