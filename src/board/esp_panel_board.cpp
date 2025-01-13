/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <memory>
#include "esp_panel_utils.h"
#include "esp_panel_board.hpp"
#include "esp_panel_board_internal.h"
#include "esp_panel_board_config_default.hpp"

#define _LCD_NAME_CLASS(name) drivers::LCD_##name
#define LCD_NAME_CLASS(name) _LCD_NAME_CLASS(name)

#define _TOUCH_NAME_CLASS(name) drivers::Touch ## name
#define TOUCH_NAME_CLASS(name) _TOUCH_NAME_CLASS(name)

namespace esp_panel {

#if ESP_PANEL_BOARD_USE_DEFAULT
Board::Board():
    Board(BOARD_DEFAULT_CONFIG)
{
    _use_default_config = true;
}
#else
Board::Board()
{
    static_assert(false, "No default board is provided. There are three ways to provide a board: 1. Use the \
    constructor with the configuration parameter; 2. Use the `esp_panel_board_supported.h` file to enable a supported \
    board; 3. Use the `esp_panel_board_custom.h` file to define a custom board");
}
#endif // ESP_PANEL_BOARD_USE_DEFAULT

bool Board::init()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Already initialized");

    // Create LCD device if it is used
    std::shared_ptr<drivers::Bus> lcd_bus = nullptr;
    std::shared_ptr<drivers::LCD> lcd_device = nullptr;
    if (_config.flags.use_lcd) {
        ESP_UTILS_LOGI("Create LCD");

        lcd_bus = drivers::BusFactory::create(_config.lcd.bus_config);
        ESP_UTILS_CHECK_NULL_RETURN(lcd_bus, false, "Create LCD bus failed");

#if ESP_PANEL_BOARD_USE_DEFAULT && defined(ESP_PANEL_BOARD_LCD_CONTROLLER)
        // If the LCD is configured by default, it will be created by the default constructor
        if (_use_default_config) {
            ESP_UTILS_CHECK_EXCEPTION_RETURN(
                (lcd_device = utils::make_shared<LCD_NAME_CLASS(ESP_PANEL_BOARD_LCD_CONTROLLER)>(
                                  lcd_bus.get(), _config.lcd.device_config
                              )), false, "Create LCD device failed"
            );
        }
#endif // ESP_PANEL_BOARD_LCD_CONTROLLER
        // If the LCD is not configured by default, it will be created by the factory function
        if (!_use_default_config) {
            lcd_device =
                drivers::LCD_Factory::create(_config.lcd.device_name, lcd_bus.get(), _config.lcd.device_config);
            ESP_UTILS_CHECK_NULL_RETURN(lcd_device, false, "Create LCD device failed");
        }
    }

    // Create touch device if it is used
    std::shared_ptr<drivers::Bus> touch_bus = nullptr;
    std::shared_ptr<drivers::Touch> touch_device = nullptr;
    if (_config.flags.use_touch) {
        ESP_UTILS_LOGI("Create touch");

        touch_bus = drivers::BusFactory::create(_config.touch.bus_config);
        ESP_UTILS_CHECK_NULL_RETURN(touch_bus, false, "Create touch bus failed");

#if ESP_PANEL_BOARD_USE_DEFAULT && defined(ESP_PANEL_BOARD_TOUCH_CONTROLLER)
        // If the touch is configured by default, it will be created by the default constructor
        if (_use_default_config) {
            ESP_UTILS_CHECK_EXCEPTION_RETURN(
                (touch_device = utils::make_shared<TOUCH_NAME_CLASS(ESP_PANEL_BOARD_TOUCH_CONTROLLER)>(
                                    touch_bus.get(), _config.touch.device_config
                                )), false, "Create touch device failed"
            );
        }
#endif // ESP_PANEL_BOARD_TOUCH_CONTROLLER
        // If the touch is not configured by default, it will be created by the factory function
        if (!_use_default_config) {
            touch_device = drivers::TouchFactory::create(
                               _config.touch.device_name, touch_bus.get(), _config.touch.device_config
                           );
            ESP_UTILS_CHECK_NULL_RETURN(touch_device, false, "Create touch device failed");
        }
    }

    // Create backlight device if it is used
    std::shared_ptr<drivers::Backlight> backlight = nullptr;
    if (_config.flags.use_backlight) {
        ESP_UTILS_LOGI("Create backlight");

        // If the backlight is a custom backlight, the user data should be set to the board instance `this`
        if (drivers::BacklightFactory::getConfigType(_config.backlight.config) == ESP_PANEL_BACKLIGHT_TYPE_CUSTOM) {
            using BacklightConfig = drivers::BacklightCustom::Config;
            ESP_UTILS_CHECK_FALSE_RETURN(
                std::holds_alternative<BacklightConfig>(_config.backlight.config), false,
                "Backlight config is not a custom backlight config"
            );

            auto &config = std::get<BacklightConfig>(_config.backlight.config);
            config.user_data = this;
        }

        backlight = drivers::BacklightFactory::create(_config.backlight.config);
        ESP_UTILS_CHECK_NULL_RETURN(backlight, false, "Create backlight device failed");
    }

    // Create IO expander device if it is used
    // If the IO expander is already configured, it will not be created again
    std::shared_ptr<drivers::IO_Expander> io_expander = nullptr;
    if (_config.flags.use_io_expander && getIO_Expander() == nullptr) {
        ESP_UTILS_LOGI("Create IO Expander");

        io_expander = drivers::IO_ExpanderFactory::create(_config.io_expander.name, _config.io_expander.config);
        ESP_UTILS_CHECK_NULL_RETURN(io_expander, false, "Create IO expander device failed");
    }

    _lcd_bus = lcd_bus;
    _lcd_device = lcd_device;
    _touch_bus = touch_bus;
    _touch_device = touch_device;
    _backlight = backlight;
    _io_expander = io_expander;

    setState(State::INIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Board::begin()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::BEGIN), false, "Already begun");

    // Initialize the board if not initialized
    if (!isOverState(State::INIT)) {
        ESP_UTILS_CHECK_FALSE_RETURN(init(), false, "Init failed");
    }

    auto &config = getConfig();

    // Begin the IO expander if it is used
    // If the IO expander is already begun, it will not be begun again
    auto io_expander = getIO_Expander();
    if (io_expander != nullptr && !io_expander->isOverState(esp_expander::Base::State::BEGIN)) {
        ESP_UTILS_LOGI("Beginning IO Expander");

        if (config.callbacks.pre_expander_begin != nullptr) {
            ESP_UTILS_LOGD("IO expander pre-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(
                config.callbacks.pre_expander_begin(this), false, "IO expander pre-begin failed"
            );
        }

        ESP_UTILS_CHECK_FALSE_RETURN(io_expander->begin(), false, "IO expander begin failed");

        if (config.callbacks.post_expander_begin != nullptr) {
            ESP_UTILS_LOGD("IO expander post-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(
                config.callbacks.post_expander_begin(this), false, "IO expander post-begin failed"
            );
        }

        ESP_UTILS_LOGI("IO expander begin success");
    }

    // Begin the LCD if it is used
    auto lcd_device = getLCD();
    if (lcd_device != nullptr) {
        ESP_UTILS_LOGI("Beginning LCD");

        if (config.callbacks.pre_lcd_begin != nullptr) {
            ESP_UTILS_LOGD("LCD pre-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(config.callbacks.pre_lcd_begin(this), false, "LCD pre-begin failed");
        }

        drivers::Bus *lcd_bus = lcd_device->getBus();
#if SOC_LCD_RGB_SUPPORTED
        // When using "3-wire SPI + RGB" LCD, the IO expander should be configured first
        if ((lcd_bus->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_RGB) && (_io_expander != nullptr)) {
            ESP_UTILS_CHECK_FALSE_RETURN(
                static_cast<drivers::BusRGB *>(lcd_bus)->configSPI_IO_Expander(
                    io_expander->getBase()->getDeviceHandle()
                ), false, "\"3-wire SPI + RGB \" LCD bus config IO expander failed"
            );
        }
#endif // SOC_LCD_RGB_SUPPORTED
        ESP_UTILS_CHECK_FALSE_RETURN(lcd_bus->begin(), false, "LCD bus begin failed");
        ESP_UTILS_CHECK_FALSE_RETURN(lcd_device->init(), false, "LCD device init failed");
        ESP_UTILS_CHECK_FALSE_RETURN(lcd_device->reset(), false, "LCD device reset failed");
        ESP_UTILS_CHECK_FALSE_RETURN(lcd_device->begin(), false, "LCD device begin failed");
        if (lcd_device->isFunctionSupported(drivers::LCD::BasicBusSpecification::FUNC_DISPLAY_ON_OFF)) {
            ESP_UTILS_CHECK_FALSE_RETURN(lcd_device->setDisplayOnOff(true), false, "LCD device set display on failed");
        } else {
            ESP_UTILS_LOGD("LCD device doesn't support display on/off function");
        }
        if (lcd_device->isFunctionSupported(drivers::LCD::BasicBusSpecification::FUNC_INVERT_COLOR)) {
            ESP_UTILS_CHECK_FALSE_RETURN(
                lcd_device->invertColor(config.lcd.pre_process.invert_color), false, "LCD device invert color failed"
            );
        } else {
            ESP_UTILS_LOGD("LCD device doesn't support invert color function");
        }
        if (lcd_device->isFunctionSupported(drivers::LCD::BasicBusSpecification::FUNC_SWAP_XY)) {
            ESP_UTILS_CHECK_FALSE_RETURN(
                lcd_device->swapXY(config.lcd.pre_process.swap_xy), false, "LCD device swap XY failed"
            );
        } else {
            ESP_UTILS_LOGD("LCD device doesn't support swap XY function");
        }
        if (lcd_device->isFunctionSupported(drivers::LCD::BasicBusSpecification::FUNC_MIRROR_X)) {
            ESP_UTILS_CHECK_FALSE_RETURN(
                lcd_device->mirrorX(config.lcd.pre_process.mirror_x), false, "LCD device mirror X failed"
            );
        } else {
            ESP_UTILS_LOGD("LCD device doesn't support mirror X function");
        }
        if (lcd_device->isFunctionSupported(drivers::LCD::BasicBusSpecification::FUNC_MIRROR_Y)) {
            ESP_UTILS_CHECK_FALSE_RETURN(
                lcd_device->mirrorY(config.lcd.pre_process.mirror_y), false, "LCD device mirror Y failed"
            );
        } else {
            ESP_UTILS_LOGD("LCD device doesn't support mirror Y function");
        }

        if (config.callbacks.post_lcd_begin != nullptr) {
            ESP_UTILS_LOGD("LCD post-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(config.callbacks.post_lcd_begin(this), false, "LCD post-begin failed");
        }

        ESP_UTILS_LOGI("LCD begin success");
    }

    // Begin the touch if it is used
    auto touch_device = getTouch();
    if (touch_device != nullptr) {
        ESP_UTILS_LOGI("Beginning touch");

        if (config.callbacks.pre_touch_begin != nullptr) {
            ESP_UTILS_LOGD("Touch pre-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(config.callbacks.pre_touch_begin(this), false, "Touch pre-begin failed");
        }

        drivers::Bus *touch_bus = touch_device->getBus();
        ESP_UTILS_CHECK_FALSE_RETURN(touch_bus->begin(), false, "Touch bus begin failed");
        ESP_UTILS_CHECK_FALSE_RETURN(touch_device->init(), false, "Touch device init failed");
        ESP_UTILS_CHECK_FALSE_RETURN(touch_device->begin(), false, "Touch device begin failed");
        ESP_UTILS_CHECK_FALSE_RETURN(
            touch_device->swapXY(config.touch.pre_process.swap_xy), false, "Touch device swap XY failed"
        );
        ESP_UTILS_CHECK_FALSE_RETURN(
            touch_device->mirrorX(config.touch.pre_process.mirror_x), false, "Touch device mirror X failed"
        );
        ESP_UTILS_CHECK_FALSE_RETURN(
            touch_device->mirrorY(config.touch.pre_process.mirror_y), false, "Touch device mirror Y failed"
        );

        if (config.callbacks.post_touch_begin != nullptr) {
            ESP_UTILS_LOGD("Touch post-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(config.callbacks.post_touch_begin(this), false, "Touch post-begin failed");
        }

        ESP_UTILS_LOGI("Touch begin success");
    }

    // Begin the backlight if it is used
    auto backlight = getBacklight();
    if (backlight != nullptr) {
        ESP_UTILS_LOGI("Beginning backlight");

        if (config.callbacks.pre_backlight_begin != nullptr) {
            ESP_UTILS_LOGD("Backlight pre-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(
                config.callbacks.pre_backlight_begin(this), false, "Backlight pre-begin failed"
            );
        }

        ESP_UTILS_CHECK_FALSE_RETURN(backlight->begin(), false, "Backlight begin failed");
        if (config.backlight.pre_process.idle_off) {
            ESP_UTILS_CHECK_FALSE_RETURN(backlight->off(), false, "Backlight off failed");
        } else {
            ESP_UTILS_CHECK_FALSE_RETURN(backlight->on(), false, "Backlight on failed");
        }

        if (config.callbacks.post_backlight_begin != nullptr) {
            ESP_UTILS_LOGD("Backlight post-begin");
            ESP_UTILS_CHECK_FALSE_RETURN(
                config.callbacks.post_backlight_begin(this), false, "Backlight post-begin failed"
            );
        }

        ESP_UTILS_LOGI("Backlight begin success");
    }

    setState(State::BEGIN);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Board::del()
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    _backlight = nullptr;
    _lcd_device = nullptr;
    _touch_device = nullptr;
    _io_expander = nullptr;

    setState(State::DEINIT);

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

bool Board::configIO_Expander(drivers::IO_Expander *expander)
{
    ESP_UTILS_LOG_TRACE_ENTER_WITH_THIS();

    ESP_UTILS_CHECK_FALSE_RETURN(!isOverState(State::INIT), false, "Already initialized");

    _io_expander = std::shared_ptr<drivers::IO_Expander>(expander, [](drivers::IO_Expander * expander) {
        ESP_UTILS_LOGD("Skip delete IO expander");
    });

    ESP_UTILS_LOG_TRACE_EXIT_WITH_THIS();

    return true;
}

} // namespace esp_panel
