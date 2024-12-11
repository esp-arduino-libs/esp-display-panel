/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <memory>
#include "esp_panel_utils.h"
#include "esp_panel_board.hpp"
#include "esp_panel_board_internal.h"

#define _LCD_NAME_CLASS(name) drivers::LCD_##name
#define LCD_NAME_CLASS(name) _LCD_NAME_CLASS(name)

#define _TOUCH_NAME_CLASS(name) drivers::Touch ## name
#define TOUCH_NAME_CLASS(name) _TOUCH_NAME_CLASS(name)

namespace esp_panel {

#if ESP_PANEL_BOARD_USE_DEFAULT
Board::Board():
    Board(BOARD_DEFAULT_CONFIG)
{
    _flags.use_default_config = 1;
#if ESP_PANEL_BOARD_BACKLIGHT_TYPE == ESP_PANEL_BACKLIGHT_TYPE_CUSTOM
    using BacklightConfig = drivers::BacklightCustom::Config;
    BacklightConfig &config = std::get<BacklightConfig>(_config.backlight.config);
    config.user_data = this;
#endif // ESP_PANEL_BOARD_BACKLIGHT_TYPE
}
#else
Board::Board()
{
    static_assert(false, "No default board is provided. Please use the constructor with the configuration parameter");
}
#endif // ESP_PANEL_BOARD_USE_DEFAULT

bool Board::init(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsInit(), false, "Already initialized");

    std::shared_ptr<drivers::LCD> lcd_device = nullptr;
    if (_config.flags.use_lcd) {
        ESP_UTILS_LOGI("Initialize LCD");
        std::shared_ptr<drivers::Bus> lcd_bus = nullptr;
        ESP_UTILS_CHECK_EXCEPTION_RETURN(
            (lcd_bus = drivers::BusFactory::create(_config.lcd.bus_type, &_config.lcd.bus_config)),
            false, "Create LCD bus failed"
        );
        ESP_UTILS_CHECK_FALSE_RETURN(lcd_bus->init(), false, "LCD bus init failed");
// #if ESP_PANEL_BOARD_USE_DEFAULT && defined(ESP_PANEL_BOARD_LCD_CONTROLLER)
//         if (_flags.use_default_config) {
//             ESP_UTILS_CHECK_EXCEPTION_RETURN(
//                 (lcd_device = esp_utils::make_shared<LCD_NAME_CLASS(ESP_PANEL_BOARD_LCD_CONTROLLER)>(
//                                   lcd_bus, _config.lcd.device_config
//                               )), false, "Create LCD device failed"
//             );
//         }
// #endif
//         if (!_flags.use_default_config) {
        lcd_device = drivers::LCD_Factory::create(_config.lcd.controller, lcd_bus, _config.lcd.device_config);
        ESP_UTILS_CHECK_NULL_RETURN(lcd_device, false, "Create LCD device failed");
        // }
    }

    std::shared_ptr<drivers::Touch> touch_device = nullptr;
    if (_config.flags.use_touch) {
        ESP_UTILS_LOGI("Initialize touch");
        std::shared_ptr<drivers::Bus> touch_bus = nullptr;
        ESP_UTILS_CHECK_EXCEPTION_RETURN(
            (touch_bus = drivers::BusFactory::create(
                             _config.touch.bus_type, &_config.touch.bus_config
                         )), false, "Create touch bus failed"
        );
        ESP_UTILS_CHECK_FALSE_RETURN(touch_bus->init(), false, "Touch bus init failed");
// #if ESP_PANEL_BOARD_USE_DEFAULT && defined(ESP_PANEL_BOARD_TOUCH_CONTROLLER)
//         if (_flags.use_default_config) {
//             ESP_UTILS_CHECK_EXCEPTION_RETURN(
//                 (touch_device = esp_utils::make_shared<TOUCH_NAME_CLASS(ESP_PANEL_BOARD_TOUCH_CONTROLLER)>(
//                                     touch_bus, _config.touch.device_config
//                                 )), false, "Create LCD device failed"
//             );
//         }
// #endif
//         if (!_flags.use_default_config) {
        ESP_UTILS_CHECK_EXCEPTION_RETURN(
            (touch_device = drivers::TouchFactory::create(
                                _config.touch.controller, touch_bus, _config.touch.device_config
                            )), false, "Create touch device failed"
        );
        // }
    }

    std::shared_ptr<drivers::Backlight> backlight = nullptr;
    if (_config.flags.use_backlight) {
        ESP_UTILS_LOGI("Initialize Backlight");
        ESP_UTILS_CHECK_EXCEPTION_RETURN(
            (backlight = drivers::BacklightFactory::create(
                             _config.backlight.type, &_config.backlight.config
                         )), false, "Create backlight device failed"
        );
    }

    std::shared_ptr<drivers::IO_Expander> io_expander = nullptr;
    if (_config.flags.use_io_expander) {
        ESP_UTILS_LOGI("Initialize IO Expander");
        ESP_UTILS_CHECK_EXCEPTION_RETURN(
            (io_expander = drivers::IO_ExpanderFactory::create(_config.io_expander.chip, _config.io_expander.base_config)),
            false, "Create IO expander device failed"
        );
        ESP_UTILS_CHECK_FALSE_RETURN(io_expander->init(), false, "IO expander init failed");
    }

    _lcd_device = lcd_device;
    _touch_device = touch_device;
    _backlight = backlight;
    _io_expander = io_expander;
    _flags.is_init = true;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Board::begin(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");
    ESP_UTILS_CHECK_FALSE_RETURN(!checkIsBegun(), false, "Already begun");

    if (_io_expander != nullptr) {
        ESP_UTILS_LOGI("Begin IO Expander");
        if (_config.callbacks.pre_expander_begin != nullptr) {
            ESP_UTILS_LOGD("IO expander pre-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(
                _config.callbacks.pre_expander_begin(this), false, "IO expander pre-begin failed"
            );
        }

        ESP_UTILS_CHECK_FALSE_RETURN(_io_expander->begin(), false, "IO expander begin failed");

        if (_config.callbacks.post_expander_begin != nullptr) {
            ESP_UTILS_LOGD("IO expander post-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(
                _config.callbacks.post_expander_begin(this), false, "IO expander post-begin failed"
            );
        }
    }

    if (_lcd_device != nullptr) {
        ESP_UTILS_LOGI("Begin LCD");
        if (_config.callbacks.pre_lcd_begin != nullptr) {
            ESP_UTILS_LOGD("LCD pre-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(_config.callbacks.pre_lcd_begin(this), false, "LCD pre-begin failed");
        }

        drivers::Bus *lcd_bus = _lcd_device->getBus();
#if SOC_LCD_RGB_SUPPORTED
        // When using "3-wire SPI + RGB" LCD, the IO expander should be configured first
        if ((lcd_bus->getType() == ESP_PANEL_BUS_TYPE_RGB) && (_io_expander != nullptr)) {
            ESP_UTILS_CHECK_FALSE_RETURN(
                static_cast<drivers::Bus_RGB *>(lcd_bus)->configSPI_IO_Expander(getIO_Expander()->getDeviceHandle()),
                false, "\"3-wire SPI + RGB \" LCD bus config IO expander failed"
            );
        }
#endif // SOC_LCD_RGB_SUPPORTED
        ESP_UTILS_CHECK_FALSE_RETURN(lcd_bus->begin(), false, "LCD bus begin failed");
        ESP_UTILS_CHECK_FALSE_RETURN(_lcd_device->init(), false, "LCD device init failed");
        ESP_UTILS_CHECK_FALSE_RETURN(_lcd_device->reset(), false, "LCD device reset failed");
        ESP_UTILS_CHECK_FALSE_RETURN(_lcd_device->begin(), false, "LCD device begin failed");
        ESP_UTILS_CHECK_FALSE_RETURN(_lcd_device->setDisplayOnOff(true), false, "LCD device set display on failed");
        if (_config.lcd.pre_process.invert_color) {
            ESP_UTILS_CHECK_FALSE_RETURN(_lcd_device->invertColor(true), false, "LCD device invert color failed");
        }
        if (_config.lcd.pre_process.swap_xy) {
            ESP_UTILS_CHECK_FALSE_RETURN(_lcd_device->swapXY(true), false, "LCD device swap XY failed");
        }
        if (_config.lcd.pre_process.mirror_x) {
            ESP_UTILS_CHECK_FALSE_RETURN(_lcd_device->mirrorX(true), false, "LCD device mirror X failed");
        }
        if (_config.lcd.pre_process.mirror_y) {
            ESP_UTILS_CHECK_FALSE_RETURN(_lcd_device->mirrorY(true), false, "LCD device mirror Y failed");
        }

        if (_config.callbacks.post_lcd_begin != nullptr) {
            ESP_UTILS_LOGD("LCD post-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(_config.callbacks.post_lcd_begin(this), false, "LCD post-begin failed");
        }
    }

    if (_touch_device != nullptr) {
        ESP_UTILS_LOGI("Begin touch");
        if (_config.callbacks.pre_touch_begin != nullptr) {
            ESP_UTILS_LOGD("Touch pre-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(_config.callbacks.pre_touch_begin(this), false, "Touch pre-begin failed");
        }

        ESP_UTILS_CHECK_FALSE_RETURN(_touch_device->getBus()->begin(), false, "Touch bus begin failed");
        ESP_UTILS_CHECK_FALSE_RETURN(_touch_device->init(), false, "Touch device init failed");
        ESP_UTILS_CHECK_FALSE_RETURN(_touch_device->begin(), false, "Touch device begin failed");
        if (_config.touch.pre_process.swap_xy) {
            ESP_UTILS_CHECK_FALSE_RETURN(_touch_device->swapXY(true), false, "Touch device swap XY failed");
        }
        if (_config.touch.pre_process.mirror_x) {
            ESP_UTILS_CHECK_FALSE_RETURN(_touch_device->mirrorX(true), false, "Touch device mirror X failed");
        }
        if (_config.touch.pre_process.mirror_y) {
            ESP_UTILS_CHECK_FALSE_RETURN(_touch_device->mirrorY(true), false, "Touch device mirror Y failed");
        }

        if (_config.callbacks.post_touch_begin != nullptr) {
            ESP_UTILS_LOGD("Touch post-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(_config.callbacks.post_touch_begin(this), false, "Touch post-begin failed");
        }
    }

    if (_backlight != nullptr) {
        ESP_UTILS_LOGI("Begin Backlight");
        if (_config.callbacks.pre_backlight_begin != nullptr) {
            ESP_UTILS_LOGD("Backlight pre-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(
                _config.callbacks.pre_backlight_begin(this), false, "Backlight pre-begin failed"
            );
        }

        ESP_UTILS_CHECK_FALSE_RETURN(_backlight->begin(), false, "Backlight begin failed");
        if (_config.backlight.pre_process.idle_off) {
            ESP_UTILS_CHECK_FALSE_RETURN(_backlight->off(), false, "Backlight off failed");
        } else {
            ESP_UTILS_CHECK_FALSE_RETURN(_backlight->on(), false, "Backlight on failed");
        }

        if (_config.callbacks.post_backlight_begin != nullptr) {
            ESP_UTILS_LOGD("Backlight post-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(
                _config.callbacks.post_backlight_begin(this), false, "Backlight post-begin failed"
            );
        }
    }

    _flags.is_begun = true;

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Board::del(void)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(checkIsInit(), false, "Not initialized");

    _backlight = nullptr;
    _lcd_device = nullptr;
    _touch_device = nullptr;
    _io_expander = nullptr;
    _flags = {};

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel
