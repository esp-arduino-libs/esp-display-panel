/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>
#include "driver/i2c.h"
#include "esp_panel_types.h"
#include "esp_panel_bus_base.hpp"

namespace esp_panel::drivers {

// Forward declaration
class BusHost_I2C;

/**
 * @brief I2C bus object class
 *
 * @note  This class is a derived class of `Bus`, user can use it directly
 */
class Bus_I2C: public Bus {
public:
    constexpr static int HOST_ID_DEFAULT = static_cast<int>(I2C_NUM_0);
    constexpr static Attributes ATTRIBUTES_DEFAULT = {
        .type = ESP_PANEL_BUS_TYPE_I2C,
        .name = "I2C",
    };

    struct Config {
        i2c_port_t getHostId(void) const
        {
            return static_cast<i2c_port_t>(host_id);
        }

        i2c_config_t getHostConfig(void) const
        {
            return {
                .mode = I2C_MODE_MASTER,
                .sda_io_num = sda_io_num,
                .scl_io_num = scl_io_num,
                .sda_pullup_en = sda_pullup_en,
                .scl_pullup_en = scl_pullup_en,
                .master = {
                    .clk_speed = clk_speed,
                },
                .clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL,
            };
        }

        esp_lcd_panel_io_i2c_config_t getIO_Config(void) const
        {
            return io_config;
        }

        static Config create(int scl_io, int sda_io, const esp_lcd_panel_io_i2c_config_t &io_config)
        {
            return Config{
                .sda_io_num = sda_io,
                .scl_io_num = scl_io,
                .io_config = io_config,
            };
        }

        static Config create(const esp_lcd_panel_io_i2c_config_t &io_config)
        {
            return Config{
                .io_config = io_config,
            };
        }

        // Host
        int host_id = HOST_ID_DEFAULT;
        int sda_io_num = -1;
        int scl_io_num = -1;
        bool sda_pullup_en = GPIO_PULLUP_ENABLE;
        bool scl_pullup_en = GPIO_PULLUP_ENABLE;
        uint32_t clk_speed = 400000;
        // Panel IO
        esp_lcd_panel_io_i2c_config_t io_config = {};
        // Extra
        bool skip_init_host = false;
    };

// *INDENT-OFF*
    /**
     * @brief Construct a I2C bus object in a common way, the host will be initialized by the driver
     *
     * @note  This function uses some default values (HOST_CONFIG_DEFAULT) to config the bus object,
     *        use `config*()` functions to change them
     * @note  The `init()` function should be called after this function
     *
     * @param scl_io    I2C SCL pin
     * @param sda_io    I2C SDA pin
     * @param io_config I2C panel IO configuration
     */
    Bus_I2C(int scl_io, int sda_io, const esp_lcd_panel_io_i2c_config_t &io_config):
        Bus(ATTRIBUTES_DEFAULT)
    {
        auto config = Config::create(scl_io, sda_io, io_config);
        _flags.skip_init_host = config.skip_init_host;
        _host_id = config.host_id;
        _host_config = config.getHostConfig();
        _io_config = config.getIO_Config();
    }
// *INDENT-OFF*

    /**
     * @brief Construct a I2C bus object in a complex way, the host needs to be initialized by the user
     *
     * @note  The `init()` function should be called after this function
     *
     * @param io_config I2C panel IO configuration
     * @param host_id   I2C host ID, default is `HOST_ID_DEFAULT`
     */
    Bus_I2C(const esp_lcd_panel_io_i2c_config_t &io_config):
        Bus(ATTRIBUTES_DEFAULT)
    {
        auto config = Config::create(io_config);
        _flags.skip_init_host = config.skip_init_host;
        _host_id = config.host_id;
        _io_config = config.getIO_Config();
    }

    Bus_I2C(const Config &config):
        Bus(ATTRIBUTES_DEFAULT)
    {
        _flags.skip_init_host = config.skip_init_host;
        _host_id = config.host_id;
        _host_config = config.getHostConfig();
        _io_config = config.getIO_Config();
    }

    /**
     * @brief Destroy the I2C bus object
     *
     */
    ~Bus_I2C() override;

    /**
     * @brief Here are some functions to configure the I2C bus object. These functions should be called before `begin()`
     *
     */
    void configI2cPullupEnable(bool sda_pullup_en, bool scl_pullup_en);
    void configI2cFreqHz(uint32_t hz);
    void configI2cAddress(uint32_t address);
    void configI2cCtrlPhaseBytes(uint32_t num);
    void configI2cDcBitOffset(uint32_t num);
    void configI2cCommandBits(uint32_t num);
    void configI2cParamBits(uint32_t num);
    void configI2cFlags(bool dc_low_on_data, bool disable_control_phase);

    bool init(void) override;

    /**
     * @brief Startup the bus
     *
     * @note  This function should be called after `init()`
     *
     * @return true if success, otherwise false
     */
    bool begin(void) override;

    /**
     * @brief Delete the bus object, release the resources
     *
     * @return true if success, otherwise false
     */
    bool del(void) override;

    bool checkIsInit(void)
    {
        return _flags.is_init;
    }

    uint8_t getI2cAddress(void)
    {
        return static_cast<uint8_t>(_io_config.dev_addr);
    }

private:
    struct {
        uint8_t is_init: 1;
        uint8_t skip_init_host: 1;
    } _flags = {};
    int _host_id = HOST_ID_DEFAULT;
    i2c_config_t _host_config = {};
    std::shared_ptr<BusHost_I2C> _host = nullptr;
    esp_lcd_panel_io_i2c_config_t _io_config = {};
};

} // namespace esp_panel::drivers

/**
 * @deprecated This type is deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Bus_I2C` instead.
 *
 * Example of migration:
 *
 * Old:
 *  ESP_PanelBus_I2C bus;
 *
 * New:
 *  esp_panel::drivers::Bus_I2C bus;
 */
typedef esp_panel::drivers::Bus_I2C ESP_PanelBus_I2C __attribute__((deprecated("Deprecated and will be removed in the next major version. Please use `esp_panel::drivers::Bus_I2C` instead.")));
