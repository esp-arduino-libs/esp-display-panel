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
 * @brief SPI bus object class
 *
 * @note  This class is a derived class of `Bus`, user can use it directly
 */
class Bus_SPI: public Bus {
public:
    constexpr static int HOST_ID_DEFAULT = static_cast<int>(SPI2_HOST);
    constexpr static Attributes ATTRIBUTES_DEFAULT = {
        .type = ESP_PANEL_BUS_TYPE_SPI,
        .name = "SPI",
    };

    struct Config {
        spi_host_device_t getHostId(void) const
        {
            return static_cast<spi_host_device_t>(host_id);
        }

        spi_bus_config_t getHostConfig(void) const
        {
            return {
                .mosi_io_num = mosi_io_num,
                .miso_io_num = miso_io_num,
                .sclk_io_num = sclk_io_num,
                .quadwp_io_num = -1,
                .quadhd_io_num = -1,
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
            if (use_complete_io_config) {
                return io_config;
            }

            return {
                .cs_gpio_num = partial_io_config.cs_gpio_num,
                .dc_gpio_num = partial_io_config.dc_gpio_num,
                .spi_mode = partial_io_config.spi_mode,
                .pclk_hz = static_cast<unsigned int>(partial_io_config.pclk_hz),
                .trans_queue_depth = 10,
                .on_color_trans_done = nullptr,
                .user_ctx = nullptr,
                .lcd_cmd_bits = partial_io_config.lcd_cmd_bits,
                .lcd_param_bits = partial_io_config.lcd_param_bits,
                .flags = {
                    .dc_low_on_data = 0,
                    .octal_mode = 0,
                    .quad_mode = 0,
                    .sio_mode = 0,
                    .lsb_first = 0,
                    .cs_high_active = 0,
                },
            };
        }

        static Config create(int cs_io, int dc_io, int sck_io, int sda_io, int sdo_io)
        {
            return Config{
                // Host
                .mosi_io_num = sda_io,
                .miso_io_num = sdo_io,
                .sclk_io_num = sck_io,
                // Panel IO
                .partial_io_config = {
                    .cs_gpio_num = cs_io,
                    .dc_gpio_num = dc_io,
                },
                // Extra
                .skip_init_host = false,
                .use_complete_io_config = false,
            };
        }

        static Config create(int cs_io, int dc_io)
        {
            return Config{
                // Panel IO
                .partial_io_config = {
                    .cs_gpio_num = cs_io,
                    .dc_gpio_num = dc_io,
                },
                // Flags
                .skip_init_host = true,
                .use_complete_io_config = false,
            };
        }

        static Config create(int sck_io, int sda_io, int sdo_io, const esp_lcd_panel_io_spi_config_t &io_config)
        {
            return Config{
                // Host
                .mosi_io_num = sda_io,
                .miso_io_num = sdo_io,
                .sclk_io_num = sck_io,
                // Panel IO
                .io_config = io_config,
                // Extra
                .skip_init_host = false,
                .use_complete_io_config = true,
            };
        }

        static Config create(const esp_lcd_panel_io_spi_config_t &io_config)
        {
            return Config{
                // Panel IO
                .io_config = io_config,
                // Flags
                .skip_init_host = true,
                .use_complete_io_config = true,
            };
        }

        // Host
        int host_id = HOST_ID_DEFAULT;
        int mosi_io_num = -1;
        int miso_io_num = -1;
        int sclk_io_num = -1;
        // Panel IO
        union {
            struct {
                int cs_gpio_num = -1;
                int dc_gpio_num = -1;
                int spi_mode = 0;
                int pclk_hz = SPI_MASTER_FREQ_40M;
                int lcd_cmd_bits = 8;
                int lcd_param_bits = 8;
            } partial_io_config;
            esp_lcd_panel_io_spi_config_t io_config = {};
        } ;
        // Extra
        bool skip_init_host = false;
        bool use_complete_io_config = false;
    };

// *INDENT-OFF*
    /**
     * @brief Construct a SPI bus object, the host will be initialized by the driver
     *
     * @note  This function uses some default values (ESP_PANEL_HOST_SPI_CONFIG_DEFAULT && ESP_PANEL_IO_SPI_CONFIG_DEFAULT)
     *        to config the bus object, use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param cs_io  SPI CS pin
     * @param dc_io  SPI DC pin
     * @param sck_io SPI SCK pin
     * @param sda_io SPI MOSI pin
     * @param sdo_io SPI MISO pin, default is `-1`
     */
    Bus_SPI(int cs_io, int dc_io, int sck_io, int sda_io, int sdo_io = -1):
        Bus(ATTRIBUTES_DEFAULT)
    {
        auto config = Config::create(cs_io, dc_io, sck_io, sda_io, sdo_io);
        _flags.skip_init_host = config.skip_init_host;
        _host_id = config.host_id;
        _host_config = config.getHostConfig();
        _io_config = config.getIO_Config();
    }

    /**
     * @brief Construct a SPI bus object in a common way, the host needs to be initialized by the user
     *
     * @note  This function uses some default values (ESP_PANEL_IO_SPI_CONFIG_DEFAULT) to config the bus object,
     *        use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param cs_io SPI CS pin
     * @param dc_io SPI DC pin
     */
    Bus_SPI(int cs_io, int dc_io):
        Bus(ATTRIBUTES_DEFAULT)
    {
        auto config = Config::create(cs_io, dc_io);
        _flags.skip_init_host = config.skip_init_host;
        _host_id = config.host_id;
        _io_config = config.getIO_Config();
    }

    Bus_SPI(int sck_io, int sda_io, int sdo_io, const esp_lcd_panel_io_spi_config_t &io_config):
        Bus(ATTRIBUTES_DEFAULT)
    {
        auto config = Config::create(sck_io, sda_io, sdo_io, io_config);
        _flags.skip_init_host = config.skip_init_host;
        _host_id = config.host_id;
        _host_config = config.getHostConfig();
        _io_config = config.getIO_Config();
    }

    Bus_SPI(const esp_lcd_panel_io_spi_config_t &io_config):
        Bus(ATTRIBUTES_DEFAULT)
    {
        auto config = Config::create(io_config);
        _flags.skip_init_host = config.skip_init_host;
        _host_id = config.host_id;
        _io_config = config.getIO_Config();
    }

    /**
     * @brief Construct a SPI bus object in a complex way, the host will be initialized by the driver
     *
     * @note  The `init()` function should be called after this function
     *
     * @param config SPI bus configuration
     */
    Bus_SPI(const Config &config):
        Bus(ATTRIBUTES_DEFAULT)
    {
        _flags.skip_init_host = config.skip_init_host;
        _host_id = config.host_id;
        _host_config = config.getHostConfig();
        _io_config = config.getIO_Config();
    }
// *INDENT-OFF*

    /**
     * @brief Destroy the SPI bus object
     *
     */
    ~Bus_SPI() override;

    /**
     * @brief Here are some functions to configure the SPI bus object. These functions should be called before `begin()`
     *
     */
    void configSpiMode(uint8_t mode);
    void configSpiFreqHz(uint32_t hz);
    void configSpiCommandBits(uint32_t num);
    void configSpiParamBits(uint32_t num);
    void configSpiTransQueueDepth(uint8_t depth);

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
    esp_lcd_panel_io_spi_config_t _io_config = {};
    std::shared_ptr<BusHost_SPI> _host = nullptr;
};

} // namespace esp_panel::drivers

/**
 * @deprecated This type is deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Bus_SPI` instead.
 *
 * Example of migration:
 *
 * Old:
 *  ESP_PanelBus_SPI bus;
 *
 * New:
 *  esp_panel::drivers::Bus_SPI bus;
 */
typedef esp_panel::drivers::Bus_SPI ESP_PanelBus_SPI __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Bus_SPI` instead.")));
