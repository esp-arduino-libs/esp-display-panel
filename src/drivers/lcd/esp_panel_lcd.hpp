/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <memory>
#include <string>
#include <map>
#include "drivers/bus/esp_panel_bus_base.hpp"
#include "esp_panel_lcd_base.hpp"
#include "esp_panel_lcd_axs15231b.hpp"
#include "esp_panel_lcd_ek9716b.hpp"
#include "esp_panel_lcd_ek79007.hpp"
#include "esp_panel_lcd_gc9a01.hpp"
#include "esp_panel_lcd_gc9b71.hpp"
#include "esp_panel_lcd_gc9503.hpp"
#include "esp_panel_lcd_hx8399.hpp"
#include "esp_panel_lcd_ili9881c.hpp"
#include "esp_panel_lcd_ili9341.hpp"
#include "esp_panel_lcd_jd9165.hpp"
#include "esp_panel_lcd_jd9365.hpp"
#include "esp_panel_lcd_nv3022b.hpp"
#include "esp_panel_lcd_sh8601.hpp"
#include "esp_panel_lcd_spd2010.hpp"
#include "esp_panel_lcd_st7262.hpp"
#include "esp_panel_lcd_st7701.hpp"
#include "esp_panel_lcd_st7703.hpp"
#include "esp_panel_lcd_st7789.hpp"
#include "esp_panel_lcd_st7796.hpp"
#include "esp_panel_lcd_st77903.hpp"
#include "esp_panel_lcd_st77916.hpp"
#include "esp_panel_lcd_st77922.hpp"

namespace esp_panel::drivers {

class LCD_Factory {
public:
    enum class Conrtoller {
        AXS15231B,
        EK9716B, EK79007,
        GC9A01, GC9B71, GC9503,
        HX8399,
        ILI9881C, ILI9341,
        JD9165, JD9365,
        NV3022B,
        SH8601,
        SPD2010,
        ST7262, ST7701, ST7703, ST7789, ST7796, ST77903, ST77916, ST77922,
    };

    LCD_Factory() = default;
    ~LCD_Factory() = default;

    static std::shared_ptr<LCD> create(Conrtoller name, std::shared_ptr<Bus> bus, const LCD::Config &config);

    static std::string getControllerNameString(Conrtoller name);

private:
    static const std::map<Conrtoller, std::string> _controller_name_map;
};

} // namespace esp_panel::drivers
