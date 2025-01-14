/**
 * | Supported ESP SoCs | ESP32-P4 |
 * | ------------------ | -------- |
 *
 * | Supported LCD Controllers | EK79007 | ILI9881C | JD9365 |
 * | ------------------------- | ------- | -------- | ------ |
 *
 * # MIPI-DSI LCD Example
 *
 * The example demonstrates how to develop different model LCDs with MIPI-DSI interface using standalone drivers and test them by displaying color bars.
 *
 * ## How to use
 *
 * 1. [Configure drivers](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#configuring-drivers) if needed.
 * 2. Modify the macros in the example to match the parameters according to your hardware.
 * 3. Navigate to the `Tools` menu in the Arduino IDE to choose a ESP board and configure its parameters, please refter to [Configuring Supported Development Boards](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#configuring-supported-development-boards)
 * 4. Verify and upload the example to your ESP board.
 *
 * ## Serial Output
 *
 * ```
 * ...
 * MIPI-DSI LCD example start
 * Initialize backlight control pin and turn it on
 * Create MIPI-DSI LCD bus
 * Create LCD device
 * Show MIPI-DSI patterns
 * Draw color bar from top left to bottom right, the order is B - G - R
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * Draw bitmap finish callback
 * MIPI-DSI LCD example end
 * MIPI-DSI refresh rate: 0
 * MIPI-DSI refresh rate: 69
 * MIPI-DSI refresh rate: 69
 * MIPI-DSI refresh rate: 69
 * ...
 * ```
 *
 * ## Troubleshooting
 *
 * Please check the [FAQ](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/FAQ.md) first to see if the same question exists. If not, please create a [Github issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues). We will get back to you as soon as possible.
 */

#include <Arduino.h>
#include <esp_display_panel.hpp>

/* The following default configurations are for the board 'Espressif: ESP32-P4-Function-EV-Board, EK79007' */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Currently, the library supports the following MIPI-DSI LCDs:
 *      - EK79007
 *      - ILI9881C
 *      - JD9365
 */
#define EXAMPLE_LCD_NAME                EK79007
#define EXAMPLE_LCD_WIDTH               (1024)
#define EXAMPLE_LCD_HEIGHT              (600)
#define EXAMPLE_LCD_COLOR_BITS          (ESP_PANEL_LCD_COLOR_BITS_RGB888)
                                                // or `ESP_PANEL_LCD_COLOR_BITS_RGB565`
#define EXAMPLE_LCD_DSI_PHY_LDO_ID      (3)     // -1 if not used
#define EXAMPLE_LCD_DSI_LANE_NUM        (2)     // ESP32-P4 supports 1 or 2 lanes
#define EXAMPLE_LCD_DSI_LANE_RATE_MBPS  (1000)  /* Single lane bit rate, should consult the LCD supplier or check the
                                                 * LCD drive IC datasheet for the supported lane rate.
                                                 * ESP32-P4 supports max 1500Mbps
                                                 */
#define EXAMPLE_LCD_DPI_CLK_MHZ         (52)
#define EXAMPLE_LCD_DPI_COLOR_BITS      (EXAMPLE_LCD_COLOR_BITS)
#define EXAMPLE_LCD_DPI_HPW             (10)
#define EXAMPLE_LCD_DPI_HBP             (160)
#define EXAMPLE_LCD_DPI_HFP             (160)
#define EXAMPLE_LCD_DPI_VPW             (1)
#define EXAMPLE_LCD_DPI_VBP             (23)
#define EXAMPLE_LCD_DPI_VFP             (12)
#define EXAMPLE_LCD_USE_EXTERNAL_CMD    (0)
#if EXAMPLE_LCD_USE_EXTERNAL_CMD
/**
 * LCD initialization commands.
 *
 * Vendor specific initialization can be different between manufacturers, should consult the LCD supplier for
 * initialization sequence code.
 *
 * Please uncomment and change the following macro definitions, then use `configVendorCommands()` to pass them in the
 * same format if needed. Otherwise, the LCD driver will use the default initialization sequence code.
 *
 * There are two formats for the sequence code:
 *   1. Raw data: {command, (uint8_t []){ data0, data1, ... }, data_size, delay_ms}
 *   2. Formatter: ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(delay_ms, command, { data0, data1, ... }) and
 *                ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(delay_ms, command)
 */
const esp_panel_lcd_vendor_init_cmd_t lcd_init_cmd[] = {
    // {0xFF, (uint8_t []){0x77, 0x01, 0x00, 0x00, 0x10}, 5, 0},
    // {0xC0, (uint8_t []){0x3B, 0x00}, 2, 0},
    // {0xC1, (uint8_t []){0x0D, 0x02}, 2, 0},
    // {0x29, (uint8_t []){0x00}, 0, 120},
    // // or
    // ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xFF, {0x77, 0x01, 0x00, 0x00, 0x10}),
    // ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC0, {0x3B, 0x00}),
    // ESP_PANEL_LCD_CMD_WITH_8BIT_PARAM(0, 0xC1, {0x0D, 0x02}),
    // ESP_PANEL_LCD_CMD_WITH_NONE_PARAM(120, 0x29),
};
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your board spec ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_PIN_NUM_RST         (27)    // Set to -1 if not used
#define EXAMPLE_LCD_PIN_NUM_BK_LIGHT    (26)    // Set to -1 if not used
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL   (1)
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL  !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL

/* Enable or disable pattern test */
#define EXAMPLE_ENABLE_PATTERN_EXAMPLE  (1)
/* Enable or disable the attachment of a callback function that is called after each bitmap drawing is completed */
#define EXAMPLE_ENABLE_ATTACH_CALLBACK  (1)
/* Enable or disable the attachment of a callback function that is called after each bitmap drawing is completed */
#define EXAMPLE_ENABLE_PRINT_LCD_FPS    (1)

#define _EXAMPLE_LCD_CLASS(name, ...)   ESP_PanelLcd_##name(__VA_ARGS__)
#define EXAMPLE_LCD_CLASS(name, ...)    _EXAMPLE_LCD_CLASS(name, ##__VA_ARGS__)

#if EXAMPLE_ENABLE_ATTACH_CALLBACK
IRAM_ATTR bool onDrawBitmapFinishCallback(void *user_data)
{
    esp_rom_printf("Draw bitmap finish callback\n");

    return false;
}
#endif

#if EXAMPLE_ENABLE_PRINT_LCD_FPS
#define EXAMPLE_LCD_FPS_COUNT_MAX  (100)

DRAM_ATTR int frame_count = 0;
DRAM_ATTR int fps = 0;
DRAM_ATTR long start_time = 0;

IRAM_ATTR bool onVsyncEndCallback(void *user_data)
{
    long frame_start_time = *(long *)user_data;
    if (frame_start_time == 0) {
        (*(long *)user_data) = millis();

        return false;
    }

    frame_count++;
    if (frame_count >= EXAMPLE_LCD_FPS_COUNT_MAX) {
        fps = EXAMPLE_LCD_FPS_COUNT_MAX * 1000 / (millis() - frame_start_time);
        frame_count = 0;
        (*(long *)user_data) = millis();
    }

    return false;
}
#endif

void setup()
{
    Serial.begin(115200);
    Serial.println("MIPI-DSI LCD example start");

#if EXAMPLE_LCD_PIN_NUM_BK_LIGHT >= 0
    Serial.println("Initialize backlight control pin and turn it on");
    ESP_PanelBacklight *backlight = new ESP_PanelBacklight(
        EXAMPLE_LCD_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL, true
    );
    backlight->begin();
    backlight->on();
#endif

    Serial.println("Create MIPI-DSI LCD bus");
    ESP_PanelBusDSI *panel_bus = new ESP_PanelBusDSI(
        EXAMPLE_LCD_DSI_LANE_NUM, EXAMPLE_LCD_DSI_LANE_RATE_MBPS,
        EXAMPLE_LCD_DPI_CLK_MHZ, EXAMPLE_LCD_DPI_COLOR_BITS, EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT,
        EXAMPLE_LCD_DPI_HPW, EXAMPLE_LCD_DPI_HBP, EXAMPLE_LCD_DPI_HFP,
        EXAMPLE_LCD_DPI_VPW, EXAMPLE_LCD_DPI_VBP, EXAMPLE_LCD_DPI_VFP,
        EXAMPLE_LCD_DSI_PHY_LDO_ID
    );
    panel_bus->begin();

    Serial.println("Create LCD device");
    ESP_PanelLcd *lcd = new EXAMPLE_LCD_CLASS(EXAMPLE_LCD_NAME, panel_bus, EXAMPLE_LCD_COLOR_BITS, EXAMPLE_LCD_PIN_NUM_RST);
#if EXAMPLE_LCD_USE_EXTERNAL_CMD
    // Configure external initialization commands, should called before `init()`
    lcd->configVendorCommands(lcd_init_cmd, sizeof(lcd_init_cmd)/sizeof(lcd_init_cmd[0]));
#endif
    lcd->init();
    lcd->reset();
    lcd->begin();
    lcd->displayOn();

#if EXAMPLE_ENABLE_PATTERN_EXAMPLE
    Serial.println("Show MIPI-DSI patterns");
    lcd->showDsiPattern(ESP_PanelLcd::DsiPatternType::BAR_HORIZONTAL);
    delay(1000);
    lcd->showDsiPattern(ESP_PanelLcd::DsiPatternType::BAR_VERTICAL);
    delay(1000);
    lcd->showDsiPattern(ESP_PanelLcd::DsiPatternType::BER_VERTICAL);
    delay(1000);
    lcd->showDsiPattern(ESP_PanelLcd::DsiPatternType::NONE);
#endif
#if EXAMPLE_ENABLE_ATTACH_CALLBACK
    /* Attach a callback function which will be called when every bitmap drawing is completed */
    lcd->attachDrawBitmapFinishCallback(onDrawBitmapFinishCallback, NULL);
#endif
#if EXAMPLE_ENABLE_PRINT_LCD_FPS
    /* Attach a callback function which will be called when the Vsync signal is detected */
    lcd->attachRefreshFinishCallback(onVsyncEndCallback, (void *)&start_time);
#endif

    Serial.println("Draw color bar from top left to bottom right, the order is B - G - R");
    /* Users can refer to the implementation within `colorBardTest()` to draw patterns on the LCD */
    lcd->colorBarTest(EXAMPLE_LCD_WIDTH, EXAMPLE_LCD_HEIGHT);

    Serial.println("MIPI-DSI LCD example end");
}

void loop()
{
    delay(1000);
#if EXAMPLE_ENABLE_PRINT_LCD_FPS
    Serial.println("MIPI-DSI refresh rate: " + String(fps));
#else
    Serial.println("IDLE loop");
#endif
}
