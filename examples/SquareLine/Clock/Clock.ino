/**
* This example demonstrates a clock demo made with Squareline.
* 
* ## How to Use
* 
* To use this example, besides installing the ESP32_Display_Panel (and its dependencies) and the lvgl (v8.3.x) library, 
* you also need to install the NTPClient library and the ArduinoJson library (open Arduino IDE -> top navigation bar -> Tools -> Manage Libraries and search directly). 
* Place the UI folder generated by Squareline in the correct location, and then configure it according to the provided steps.
* 
* ### Configure ESP32_Display_Panel
* 
* ESP32_Display_Panel has its own configuration file called `ESP_Panel_Conf.h`. After installing ESP32_Display_Panel, follow these configuration steps:
* 
* 1. Navigate to the directory where Arduino libraries are installed.
* 
* 2. Navigate to the `ESP32_Display_Panel` folder, copy `ESP_Panel_Conf_Template.h` and place the copy outside the `ESP32_Display_Panel` folder at the same directory level. Then rename the copied file as `ESP_Panel_Conf.h`.
* 
* 3. Finally, the layout of the Arduino Libraries folder with `ESP_Panel_Conf.h` appear as follows:
* 
*    ```
*    Arduino
*        |-libraries
*            |-ESP32_Display_Panel
*            |-other_lib_1
*            |-other_lib_2
*            |-ESP_Panel_Conf.h
*    ```
* 
* 4. Please refer to [Supported Boards List](https://github.com/esp-arduino-libs/ESP32_Display_Panel#supported-boards--drivers) to check if the current board is compatible. If it is compatible, please navigate to the "For Supported Boards" section; Otherwise, navigate to the "For Unsupported Boards" section.
* 
* #### For Supported Board
* 
* 1. Open `ESP_Panel_Conf.h` file. First, set the macro `ESP_PANEL_USE_SUPPORTED_BOARD` to `1` (default is `1`). Then, according to the name of your target development board, uncomment the macro definitions in the format `ESP_PANEL_BOARD_<NAME>` below,
* 
* 2. The following code takes *ESP32_S3_BOX* development board as an example:
* 
*    ```c
*    ...
*    // #define ESP_PANEL_BOARD_ESP32_C3_LCDKIT
*    #define ESP_PANEL_BOARD_ESP32_S3_BOX
*    // #define ESP_PANEL_BOARD_ESP32_S3_BOX_LITE
*    ...
*    ```
* 
* #### For Unsupported Board
* 
* Please refer to the [document](https://github.com/esp-arduino-libs/ESP32_Display_Panel#for-unsupported-board) for the details.
* 
* ### Configure LVGL
* 
* 1. Navigate to the directory where Arduino libraries are installed.
* 
* 2. Navigate to the `lvgl` folder, copy `lv_conf_template.h` and place the copy outside the `lvgl` folder at the same directory level. Then rename the copied file as `lv_conf.h`.
* 
* 3. Finally, the layout of the Arduino Libraries folder with `lv_conf.h` appear as follows:
* 
*    ```
*    Arduino
*        |-libraries
*            |-lv_conf.h
*            |-lvgl
*            |-other_lib_1
*            |-other_lib_2
*    ```
* 
* 4. Open `lv_conf.h` and change the first `#if 0` to `#if 1` to enable the content of the file.
* 
* 5. Set the following configurations:
* 
*    ```c
*    #define LV_COLOR_DEPTH          16
*    #define LV_COLOR_16_SWAP        1   // This configuration is not for RGB LCD.
*                                        // Don't set it if using ESP32-S3-LCD-Ev-Board or ESP32-S3-LCD-Ev-Board-2
*    #define LV_MEM_CUSTOM           1
*    #define LV_MEMCPY_MEMSET_STD    1
*    #define LV_TICK_CUSTOM          1
*    #define LV_FONT_MONTSERRAT_12   1
*    #define LV_FONT_MONTSERRAT_16   1
*    #define LV_USE_DEMO_WIDGETS     1
*    #define LV_USE_DEMO_BENCHMARK   1
*    #define LV_USE_DEMO_STRESS      1
*    #define LV_USE_DEMO_MUSIC       1
*    ```
* 
* 6. For more information, please refer to [LVGL document](https://docs.lvgl.io/8.3/get-started/platforms/arduino.html).
* 
* ### Port the UI folder generated by Squareline
* 
* 1. Navigate to the directory where Arduino libraries are installed.
* 
* 2. Navigate to the ESP32_Display_Panel folder, follow the path `examples ` ->  `Squareline ` ->  `Clock ` ->  `libraries ` ->  `ui ` to find the ui folder. Copy the ui folder and place the copy outside, at the same directory level as the ESP32_Display_Panel folder.
* 
* 3. Finally, the layout of the Arduino Libraries folder with `ui` folder appear as follows:
* 
*    note: If you want to use the ui folder generated by your own Squareline, the placement location is the same.
* 
*    ```
*    Arduino
*        |-libraries
*            |-ESP32_Display_Panel
*            |-ui
*            |-other_lib
*            |-ESP_Panel_Conf.h
*    ```
* 
* ### Configure Board
* 
* Below are recommended configurations for developing GUI applications on various development boards. These settings can be adjusted based on specific requirements.
* 
* Go to the `Tools` in Arduino IDE to configure the following settings:
* 
* |    Supported Boards     |  Selected Board:   |  PSRAM:  | Flash Mode: | Flash Size: | USB CDC On Boot: |    Partition Scheme:    | Core Debug Level: |
* | :---------------------: | :----------------: | :------: | :---------: | :---------: | :--------------: | :---------------------: | :---------------: |
* |     ESP32-C3-LCDkit     | ESP32C3 Dev Module | Disabled |     QIO     | 4MB (32Mb)  |     Enabled      | Default 4MB with spiffs |       Info        |
* |      ESP32-S3-Box       |    ESP32-S3-Box    |    -     |      -      |      -      |        -         |     16M Flash (3MB)     |       Info        |
* |     ESP32-S3-Box-3      | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |    16MB     |  **See Note 1**  |     16M Flash (3MB)     |       Info        |
* |    ESP32-S3-Box-Lite    |    ESP32-S3-Box    |    -     |      -      |      -      |        -         |     16M Flash (3MB)     |       Info        |
* |      ESP32-S3-EYE       | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |     8MB     |     Enabled      |     8M with spiffs      |       Info        |
* |    ESP32-S3-Korvo-2     | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |    16MB     |     Disabled     |     16M Flash (3MB)     |       Info        |
* |  ESP32-S3-LCD-EV-Board  | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |    16MB     |  **See Note 1**  |     16M Flash (3MB)     |       Info        |
* | ESP32-S3-LCD-EV-Board-2 | ESP32S3 Dev Module |   OPI    |  QIO 80MHz  |    16MB     |  **See Note 1**  |     16M Flash (3MB)     |       Info        |
* |    ESP32-S3-USB-OTG     |  ESP32-S3-USB-OTG  |    -     |      -      |      -      |        -         |     8M with spiffs      |       Info        |
* 
* **Note:**
* 
*       1. "USB CDC On Boot" should be enabled according to the using port:
*          * Disable it if using **UART port**, enable it if using **USB port**.
*          * If it is different in the previous flashing, should enable `Erase All Flash Before Sketch Upload` first, then disable it after flashing.
* 
* ## Example Output
* 
* ```bash
* ...
* Squareline Clock init begin!
* Setup done
* ...
* ```
*
*/

#include <Arduino.h>
#include <lvgl.h>
#include <ESP_Panel_Library.h>
#include <ESP_IOExpander_Library.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ui.h>
#include "esp_flash.h"
#include "nvs.h"
#include "nvs_flash.h"

#define LV_BUF_SIZE     (ESP_PANEL_LCD_H_RES * 20)

#define WEATHER_API_KEY  "91c57a80e449b8837b06b38d2460ee69"    //HeartWeather API key
#define WEATHER_CITY "Shanghai"                            //Current city
#define NVS_PART_NAME         "nvs"
#define NVS_PART_NAMESPACE    "result"
#define WIFI_NAME_LEN_MAX       20
#define WIFI_PASSWORD_LEN_MAX   20
#define ALARM_ROL_LEN           10
#define ALARM_TIME_LEN          30
#define DATE_INFO_LEN           20
#define SERVER_IP             "ntp6.aliyun.com"
#define TIME_OFFSET             28800
#define TIME_UPDATE_INTERVAL    60000
#define TIME_INFO_LEN           10
#define WEEK_INFO_LEN           20
#define TEMPERATURE_LEN         20

ESP_Panel *panel = NULL;
SemaphoreHandle_t lvgl_mux = NULL;

nvs_handle_t nvs_flash_handle;
static bool nvs_init_flag = false;
static int8_t nvs_int = 0;

static bool wifi_connected_flag = false;
static bool wifi_list_switch = false;
static bool password_valid_flag = false;
static bool alarm_flag = false;

static char st_wifi_name[WIFI_NAME_LEN_MAX];
static char st_wifi_password[WIFI_PASSWORD_LEN_MAX];
static const char *selected_wifi_name = NULL;
static const char *wifi_password = NULL;

static char hours_rol[ALARM_ROL_LEN];
static char min_rol[ALARM_ROL_LEN];
static char sec_rol[ALARM_ROL_LEN];
static char init_hours_rol[ALARM_ROL_LEN];
static char init_min_rol[ALARM_ROL_LEN];
static char init_sec_rol[ALARM_ROL_LEN];
static char alarm_time[ALARM_TIME_LEN]; 

lv_obj_t * wifi_list = NULL;
static int num_wifi = 0;

static char date_string[DATE_INFO_LEN];

static int connected_count = 0;
static int nvs_wifi_connect_count = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, SERVER_IP, TIME_OFFSET, TIME_UPDATE_INTERVAL);

HTTPClient http;

static String openweather_api_key = WEATHER_API_KEY;    
static String openweather_city = WEATHER_CITY; 
static String url_openweather = "";
static String url_lat_lon = "";
static String weather_url = "";
static String lat_lon_url = "";
static String Weather = "";
static String prev_weather = "";
static String lat = "";
static String lon = "";             
static int temperature = 999;
static char temperature_buffer[TEMPERATURE_LEN];

time_t now;
struct tm timeinfo;
static char time_str[TIME_INFO_LEN] = "12:59:59";    
static char weekday_str[WEEK_INFO_LEN];

static uint16_t init_hour_roll_set = 0;
static uint16_t init_min_roll_set = 0;
static uint16_t init_sec_roll_set = 0;

#if ESP_PANEL_LCD_BUS_TYPE == ESP_PANEL_BUS_TYPE_RGB
/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    panel->getLcd()->drawBitmap(area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_p);
    lv_disp_flush_ready(disp);
}
#else
/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    panel->getLcd()->drawBitmap(area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_p);
}

bool notify_lvgl_flush_ready(void *user_ctx)
{
    lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
    lv_disp_flush_ready(disp_driver);
    return false;
}
#endif /* ESP_PANEL_LCD_BUS_TYPE */

#if ESP_PANEL_USE_LCD_TOUCH
/* Read the touchpad */
void my_touchpad_read(lv_indev_drv_t * indev, lv_indev_data_t * data)
{
    panel->getLcdTouch()->readData();

    bool touched = panel->getLcdTouch()->getTouchState();
    if(!touched) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        TouchPoint point = panel->getLcdTouch()->getPoint();

        data->state = LV_INDEV_STATE_PR;
        /*Set the coordinates*/
        data->point.x = point.x;
        data->point.y = point.y;

        // Serial.printf("Touch point: x %d, y %d\n", point.x, point.y);
    }
}
#endif

bool lv_port_lock(uint32_t timeout_ms)
{
    const TickType_t timeout_ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTakeRecursive(lvgl_mux, timeout_ticks) == pdTRUE;
}

void lv_port_unlock(void)
{
    xSemaphoreGiveRecursive(lvgl_mux);
}

void lvgl_task(void *pvParameter)
{
    while (1) {
        lv_port_lock(0);
        lv_task_handler();
        lv_port_unlock();
        delay(5);
    }
}

void nvs_write_str(const char* key, const char* value)
{
    nvs_open_from_partition(NVS_PART_NAME, NVS_PART_NAMESPACE, NVS_READWRITE, &nvs_flash_handle);
    
    nvs_set_str(nvs_flash_handle, key, value);
    nvs_commit(nvs_flash_handle);
    nvs_close(nvs_flash_handle);
}

void nvs_read_str(const char* key, char* nvs_str, size_t max_length)
{
    nvs_open_from_partition(NVS_PART_NAME, NVS_PART_NAMESPACE, NVS_READWRITE, &nvs_flash_handle);

    nvs_get_str(nvs_flash_handle, key, nvs_str, &max_length);
    nvs_close(nvs_flash_handle);
}

void nvs_write_int(const char* key, int8_t value)
{
    nvs_open_from_partition(NVS_PART_NAME, NVS_PART_NAMESPACE, NVS_READWRITE, &nvs_flash_handle);
    
    nvs_set_i8(nvs_flash_handle, key, value);
    nvs_commit(nvs_flash_handle);
    nvs_close(nvs_flash_handle);
}

int8_t nvs_read_int(const char* key)
{
    nvs_open_from_partition(NVS_PART_NAME, NVS_PART_NAMESPACE, NVS_READWRITE, &nvs_flash_handle);
    
    nvs_get_i8(nvs_flash_handle, key, &nvs_int);
    nvs_close(nvs_flash_handle);
    return nvs_int;
}

String Getlatlon(String api,String city)
{
    url_lat_lon =  "http://api.openweathermap.org/geo/1.0/direct?q=";
    url_lat_lon += city;
    url_lat_lon += "&limit=1&appid=";
    url_lat_lon += api;
    return url_lat_lon;
}

String GetURL(String api,String lat, String lon)
{
    url_openweather =  "https://api.openweathermap.org/data/2.5/weather?lat=";
    url_openweather += lat;
    url_openweather += "&lon=";
    url_openweather += lon;
    url_openweather += "&appid=";
    url_openweather += api;
    return url_openweather;
}

void ParseWeather(String url)
{  
    DynamicJsonDocument doc(1024); //Allocate memory dynamically

    http.begin(url);

    int httpGet = http.GET();
    if(httpGet > 0) { 
        if(httpGet == HTTP_CODE_OK) {
            String json = http.getString();
            deserializeJson(doc, json);

            Weather = doc["weather"][0]["main"].as<String>();   
            temperature = doc["main"]["temp"].as<int>() - 273.15;

            Serial.printf("Weather: %s\n", Weather);
            Serial.printf("temperature: %d\n", temperature);
        } else {
            Serial.printf("ERROR: HTTP_CODE\n");
        }
    } else {
        Serial.printf("ERROR: httpGet\n");
    }

    http.end();
}

void Parselatlon(String url)
{  
    DynamicJsonDocument doc(4096); //Allocate memory dynamically

    http.begin(url);

    int httpGet = http.GET();
    if(httpGet > 0) { 
        if(httpGet == HTTP_CODE_OK) {
            String json = http.getString();
            deserializeJson(doc, json);

            lat = doc[0]["lat"].as<String>(); 
            lon = doc[0]["lon"].as<String>();

            Serial.printf("lat: %s\n", lat);
            Serial.printf("lon: %s\n", lon);
        } else {
            Serial.printf("ERROR: HTTP_CODE\n");
        }
    } else {
        Serial.printf("ERROR: httpGet\n");
    }

    http.end();
}

void splitTime(char *timeStr, char *init_hour_rol, char *init_min_rol, char *init_sec_rol) {
    char *token = strtok(timeStr, ":");
    if (token != NULL) {
        strcpy(init_hour_rol, token);
        token = strtok(NULL, ":");
        if (token != NULL) {
            strcpy(init_min_rol, token);
            token = strtok(NULL, ":");
            if (token != NULL) {
                strcpy(init_sec_rol, token);
            } else {
                strcpy(init_sec_rol, "00");
            }
        } else {
            strcpy(init_min_rol, "00");
            strcpy(init_sec_rol, "00");
        }
    } else {
        strcpy(init_hour_rol, "00");
        strcpy(init_min_rol, "00");
        strcpy(init_sec_rol, "00");
    }
}

void WifiClock_run_cb(lv_timer_t *timer)
{
    lv_obj_t *ui_LabelTime = (lv_obj_t *) timer->user_data;

    if(wifi_connected_flag) {
        String formattedTime = timeClient.getFormattedTime();
        snprintf(time_str, sizeof(time_str), "%s", formattedTime.c_str());
    }

    lv_label_set_text_fmt(ui_LabelTime, "%s", time_str);
    if(!strcmp(alarm_time, time_str)) {
        alarm_flag = true;
        Serial.println("alarm_flag: true");
    } 

}

void WifiCalendar_run_cb(lv_timer_t *timer)
{
    lv_obj_t *ui_Labelcalendar = (lv_obj_t *) timer->user_data;
    lv_label_set_text_fmt(ui_Labelcalendar, "%s", date_string);
}

void WifiWeek_run_cb(lv_timer_t *timer)
{
    lv_obj_t *ui_LabelWeek = (lv_obj_t *) timer->user_data;
    String week_temp_str;
    if(wifi_connected_flag) {
        if (timeClient.getDay() == 0) {
            week_temp_str = "Sunday";
        } else if (timeClient.getDay() == 1) {
            week_temp_str = "Monday";
        } else if (timeClient.getDay() == 2) {
            week_temp_str = "Tuesday";
        } else if (timeClient.getDay() == 3) {
            week_temp_str = "Wednesday";
        } else if (timeClient.getDay() == 4) {
            week_temp_str = "Thursday";
        } else if (timeClient.getDay() == 5) {
            week_temp_str = "Friday";
        } else if (timeClient.getDay() == 6) {
            week_temp_str = "Saturday";
        }
        lv_label_set_text_fmt(ui_LabelWeek, "%s", week_temp_str);
    } else {
        lv_label_set_text_fmt(ui_LabelWeek, "%s", weekday_str);
    }

    if(week_temp_str == "Wednesday" || strcmp(weekday_str, "Wednesday") == 0) {
        lv_obj_set_x( ui_Labelweek, 80 );
    }
}

void WifiWeather_run_cb(lv_timer_t *timer)
{
    if(wifi_connected_flag) {
        lv_obj_t *ui_Labelweather = (lv_obj_t *) timer->user_data;
        lv_port_lock(0);
        lv_label_set_text_fmt(ui_Labelweather, "%s", Weather);
        if(prev_weather != Weather) {
            if(Weather == "Clear") {
                _ui_flag_modify(ui_ImageCloudy, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageRain, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSnow, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSunny, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
                _ui_flag_modify(ui_ImageThunderstor, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageAtmosphere, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageDrizzle, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            } else if(Weather == "Clouds") {
                _ui_flag_modify(ui_ImageCloudy, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
                _ui_flag_modify(ui_ImageRain, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSnow, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSunny, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageThunderstor, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageAtmosphere, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageDrizzle, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);                
            } else if(Weather == "Rain") {
                _ui_flag_modify(ui_ImageCloudy, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageRain, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
                _ui_flag_modify(ui_ImageSnow, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSunny, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD); 
                _ui_flag_modify(ui_ImageThunderstor, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageAtmosphere, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageDrizzle, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD); 
            } else if(Weather == "Snow") {
                _ui_flag_modify(ui_ImageCloudy, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageRain, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSnow, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
                _ui_flag_modify(ui_ImageSunny, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageThunderstor, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageAtmosphere, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageDrizzle, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD); 
            } else if(Weather == "Drizzle") {
                _ui_flag_modify(ui_ImageCloudy, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageRain, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSnow, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSunny, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageThunderstor, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageAtmosphere, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageDrizzle, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
            } else if(Weather == "Thunderstorm") {
                _ui_flag_modify(ui_ImageCloudy, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageRain, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSnow, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSunny, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageThunderstor, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
                _ui_flag_modify(ui_ImageAtmosphere, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageDrizzle, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            } else if(Weather == "Mist" || Weather == "Smoke" || Weather == "Haze" || Weather == "Dust" || \
                      Weather == "Fog" || Weather == "Sand" || Weather == "Dust") {
                _ui_flag_modify(ui_ImageCloudy, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageRain, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSnow, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSunny, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageThunderstor, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageAtmosphere, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
                _ui_flag_modify(ui_ImageDrizzle, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            } else {
                _ui_flag_modify(ui_ImageCloudy, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageRain, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSnow, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageSunny, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageThunderstor, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageAtmosphere, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
                _ui_flag_modify(ui_ImageDrizzle, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            }
        }
        lv_port_unlock();
        prev_weather = Weather;
    }
}

void WifiTemperature_run_cb(lv_timer_t *timer)
{
    if(wifi_connected_flag) {
        lv_obj_t *ui_Labeltemperature = (lv_obj_t *) timer->user_data;
        if(temperature != 999) {
            sprintf(temperature_buffer, "%d℃", temperature);
        }
        lv_label_set_text_fmt(ui_Labeltemperature, "%s", temperature_buffer);
    }
}

void ui_timer_init()
{
    lv_timer_t *LabelTime_Wifitimer = lv_timer_create(WifiClock_run_cb, 1000, (void *) ui_LabelTime);
    WifiClock_run_cb(LabelTime_Wifitimer);
    lv_timer_t *Labelcalendar_timer = lv_timer_create(WifiCalendar_run_cb, 1000, (void *) ui_Labelcalendar);
    WifiCalendar_run_cb(Labelcalendar_timer);
    lv_timer_t *Labelweek_timer = lv_timer_create(WifiWeek_run_cb, 1000, (void *) ui_Labelweek);
    WifiWeek_run_cb(Labelweek_timer);
    lv_timer_t *Labelweather_timer = lv_timer_create(WifiWeather_run_cb, 1000, (void *) ui_Labelweather);
    WifiWeather_run_cb(Labelweather_timer);
    lv_timer_t *Labeltemperature_timer = lv_timer_create(WifiTemperature_run_cb, 1000, (void *) ui_Labeltemperature);
    WifiTemperature_run_cb(Labeltemperature_timer);
}

void WifiListClicked_cb(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    
    if (event_code == LV_EVENT_CLICKED) {
        wifi_list_switch = false;
        Serial.println("wifi_list_switch: false");

        lv_port_lock(0);
        selected_wifi_name = lv_list_get_btn_text(wifi_list, target);
        if (selected_wifi_name != NULL) {
            Serial.printf("%s\n", selected_wifi_name);
        }
        _ui_screen_change(&ui_ScreenPassord, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenPassord_screen_init);
        lv_port_unlock();
    }
}

void keyboard_event_cb(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if(event_code == LV_EVENT_CLICKED && lv_keyboard_get_selected_btn(target) == 39) {
        wifi_password = lv_textarea_get_text(ui_TextPassword);
        Serial.printf("%s\n", wifi_password);

        WiFi.begin(selected_wifi_name, wifi_password);

        password_valid_flag = true;
        Serial.println("password_valid_flag: true");

        _ui_flag_modify(ui_SpinnerLoadPassword, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        _ui_flag_modify(ui_KeyboardPassword, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    }
}

void Ala_confirm_cb(lv_event_t * e)
{
    lv_roller_get_selected_str(ui_Rollerhour, hours_rol, sizeof(hours_rol));
    lv_roller_get_selected_str(ui_Rollerminute, min_rol, sizeof(hours_rol));
    lv_roller_get_selected_str(ui_Rollersecond, sec_rol, sizeof(min_rol));

    strcpy(alarm_time, hours_rol); 
    strcat(alarm_time, ":"); 
    strcat(alarm_time, min_rol); 
    strcat(alarm_time, ":"); 
    strcat(alarm_time, sec_rol); 
    Serial.printf("alarm_time: %s\n", alarm_time);

    _ui_screen_change(&ui_ScreenClock, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenClock_screen_init);
}

void Factory_cb(lv_event_t * e)
{
    wifi_connected_flag = false;
    Serial.println("wifi_connected_flag: false");
    nvs_write_int("NVS_WifiFg", wifi_connected_flag);

    http.end();

    lv_port_lock(0);
    _ui_flag_modify(ui_ImageWifi, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    lv_port_unlock();
    
    WiFi.disconnect();
}

void ButtonAlarm_cb(lv_event_t * e)
{
    splitTime(time_str, init_hours_rol, init_min_rol, init_sec_rol);

    init_hour_roll_set = atoi(&init_hours_rol[0]);
    init_min_roll_set = atoi(&init_min_rol[0]);
    init_sec_roll_set = atoi(&init_sec_rol[0]);

    lv_roller_set_selected(ui_Rollerhour, init_hour_roll_set, LV_ANIM_OFF);
    lv_roller_set_selected(ui_Rollerminute, init_min_roll_set, LV_ANIM_OFF);
    lv_roller_set_selected(ui_Rollersecond, init_sec_roll_set, LV_ANIM_OFF);
}

void ButtonSetWifi_cb(lv_event_t * e)
{
    wifi_list_switch = true;
}

void ButtonRetSet_cb(lv_event_t * e)
{
    wifi_list_switch = false;
}

void ButtonRetWifi_cb(lv_event_t * e)
{
    wifi_list_switch = true;
}

void setup()
{
    Serial.begin(115200); /* prepare for possible serial debug */ 
    Serial.println("Squareline Clock init begin!");
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        Serial.println("nvs init err");
    }

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    setenv("TZ", "CST-8", 1);
    tzset();
    
    panel = new ESP_Panel();

    /* Initialize LVGL core */
    lv_init();
    
    prev_weather = "Clear";  

    /* Initialize LVGL buffers */
    static lv_disp_draw_buf_t draw_buf;
    /* Using double buffers is more faster than single buffer */
    /* Using internal SRAM is more fast than PSRAM (Note: Memory allocated using `malloc` may be located in PSRAM.) */
    uint8_t *buf = (uint8_t *)heap_caps_calloc(1, LV_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_INTERNAL);
    assert(buf);
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_BUF_SIZE);

    /* Initialize the display device */
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = ESP_PANEL_LCD_H_RES;
    disp_drv.ver_res = ESP_PANEL_LCD_V_RES;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

#if ESP_PANEL_USE_LCD_TOUCH
    /* Initialize the input device */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
#endif

    /* There are some extral initialization for ESP32-S3-LCD-EV-Board */
#ifdef ESP_PANEL_BOARD_ESP32_S3_LCD_EV_BOARD
    /* Initialize IO expander */
    ESP_IOExpander *expander = new ESP_IOExpander_TCA95xx_8bit(ESP_PANEL_LCD_TOUCH_BUS_HOST_ID, ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000, ESP_PANEL_LCD_TOUCH_I2C_IO_SCL, ESP_PANEL_LCD_TOUCH_I2C_IO_SDA);
    expander->init();
    expander->begin();
    /* Add into panel for 3-wire SPI */
    panel->addIOExpander(expander);
    /* For the newest version sub board, need to set `ESP_PANEL_LCD_RGB_IO_VSYNC` to high before initialize LCD */
    pinMode(ESP_PANEL_LCD_RGB_IO_VSYNC, OUTPUT);
    digitalWrite(ESP_PANEL_LCD_RGB_IO_VSYNC, HIGH);
#endif

    /* There are some extral initialization for ESP32-S3-Korvo-2 */
#ifdef ESP_PANEL_BOARD_ESP32_S3_KORVO_2
    /* Initialize IO expander */
    ESP_IOExpander *expander = new ESP_IOExpander_TCA95xx_8bit(ESP_PANEL_LCD_TOUCH_BUS_HOST_ID, ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000, ESP_PANEL_LCD_TOUCH_I2C_IO_SCL, ESP_PANEL_LCD_TOUCH_I2C_IO_SDA);
    expander->init();
    expander->begin();
    /* Reset LCD */
    expander->pinMode(2, OUTPUT);
    expander->digitalWrite(2, LOW);
    usleep(20000);
    expander->digitalWrite(2, LOW);
    usleep(120000);
    expander->digitalWrite(2, HIGH);
    /* Turn on backlight */
    expander->pinMode(1, OUTPUT);
    expander->digitalWrite(1, HIGH);
    /* Keep CS low */
    expander->pinMode(3, OUTPUT);
    expander->digitalWrite(3, LOW);
#endif

#ifdef ESP_PANEL_BOARD_ESP32_S3_BOX_3
    pinMode(ESP_PANEL_LCD_TOUCH_IO_INT, OUTPUT);
    digitalWrite(ESP_PANEL_LCD_TOUCH_IO_INT, LOW);
    usleep(100);
#endif

    /* Initialize bus and device of panel */
    panel->init();
#if ESP_PANEL_LCD_BUS_TYPE != ESP_PANEL_BUS_TYPE_RGB
    /* Register a function to notify LVGL when the panel is ready to flush */
    /* This is useful for refreshing the screen using DMA transfers */
    panel->getLcd()->setCallback(notify_lvgl_flush_ready, &disp_drv);
#endif
    /* Start panel */
    panel->begin();

    /* Create a task to run the LVGL task periodically */
    lvgl_mux = xSemaphoreCreateRecursiveMutex();
    xTaskCreate(lvgl_task, "lvgl", 8192, NULL, 1, NULL);
    
    /**
     * To avoid errors caused by multiple tasks simultaneously accessing LVGL,
     * should acquire a lock before operating on LVGL.
     */

    lv_port_lock(0);

    ui_init();
    ui_timer_init();

    lv_port_unlock();

    Serial.println("Setup done");
}

void loop()
{
    if(!nvs_init_flag) {
        if(nvs_read_int("NVS_WifiFg")) { 
            nvs_read_str("NVS_WifiNa", st_wifi_name, sizeof(st_wifi_name));
            nvs_read_str("NVS_WifiPw", st_wifi_password, sizeof(st_wifi_password));
            selected_wifi_name = st_wifi_name;
            wifi_password = st_wifi_password;
            Serial.printf("NVS: selected_wifi_name: %s\n", selected_wifi_name);
            Serial.printf("NVS: wifi_password: %s\n", wifi_password);
            
            if(wifi_password[0] != '\0' && selected_wifi_name[0] != '\0') {
                WiFi.begin(selected_wifi_name, wifi_password);
            }
    
            while(WiFi.status() != WL_CONNECTED) {
                Serial.println("Wifi connecting...");
                Serial.printf("NVS: selected_wifi_name: %s\n", selected_wifi_name);
                Serial.printf("NVS: wifi_password: %s\n", wifi_password);

                nvs_wifi_connect_count++;

                if(nvs_wifi_connect_count >= 5) {
                    Serial.printf("NVS Wifi Connecting Fail\n");
                    WiFi.disconnect();
                    nvs_wifi_connect_count = 0;
                    break;
                }
                delay(1000);
            }

            if(WiFi.status() == WL_CONNECTED) {
                Serial.printf("NVS Wifi Connecting Success\n");
                lat_lon_url = Getlatlon(openweather_api_key, openweather_city);
                while(lat == NULL && lon == NULL) {
                    Parselatlon(lat_lon_url);
                }
                weather_url = GetURL(openweather_api_key, lat, lon);

                wifi_connected_flag = true;
                Serial.printf("wifi_connected_flag: true\n");
            } 
        } else {
            wifi_connected_flag = false;
            Serial.printf("wifi_connected_flag: false\n");
        }
        nvs_init_flag = true;
        Serial.printf("nvs_init_flag: true\n");
        nvs_wifi_connect_count = 0;
    }

    if(wifi_list_switch) {
        num_wifi = WiFi.scanNetworks();
        Serial.println("Scan done");

        if(num_wifi == 0) {
            Serial.println("no networks found");
        } else if(wifi_list_switch) {
            wifi_list_switch = false;
            Serial.println("wifi_list_switch: false");

            Serial.println("Wifi list show:");

            lv_port_lock(0);

            _ui_flag_modify(ui_SpinnerLoadWifi, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);

            if(wifi_list != NULL) {
                lv_obj_del(wifi_list);
            }
            wifi_list = lv_list_create(lv_scr_act());
            lv_obj_set_size(wifi_list, 300, 180);
            for (int i = 0; i < num_wifi; i++) {
                lv_obj_t *wifi_list_Item = lv_list_add_btn(wifi_list, NULL, WiFi.SSID(i).c_str());
                lv_obj_set_user_data(wifi_list_Item, (void *)WiFi.SSID(i).c_str());
                lv_obj_add_event_cb(wifi_list_Item, WifiListClicked_cb, LV_EVENT_ALL, NULL);
            }
            lv_obj_align(wifi_list, LV_ALIGN_CENTER, 0, 30);

            lv_port_unlock();
        }
        WiFi.scanDelete();
    }

    if(password_valid_flag) {
        Serial.println("Wifi connecting...");
        Serial.printf("%s\n",  selected_wifi_name);
        Serial.printf("%s\n", wifi_password);
        connected_count++;

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("password correct: Wifi connected success");

            lat_lon_url = Getlatlon(openweather_api_key, openweather_city);
            while(lat == NULL && lon == NULL) {
                Parselatlon(lat_lon_url);
            }
            weather_url = GetURL(openweather_api_key, lat, lon);

            nvs_write_str("NVS_WifiNa", selected_wifi_name);
            nvs_write_str("NVS_WifiPw", wifi_password);
            
            wifi_connected_flag = true;
            Serial.println("wifi_connected_flag: true");
            nvs_write_int("NVS_WifiFg", wifi_connected_flag);

            password_valid_flag = false;
            Serial.println("password_valid_flag: false");

            connected_count = 0;

            lv_port_lock(0);
            _ui_flag_modify(ui_SpinnerLoadPassword, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            _ui_flag_modify(ui_KeyboardPassword, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
            _ui_screen_change(&ui_ScreenClock, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_ScreenClock_screen_init);
            lv_textarea_set_text(ui_TextPassword, "");
            lv_port_unlock();
        } else if(connected_count >= 6) {
            Serial.println("password wrong: Wifi connected failed");
            
            wifi_connected_flag = false;
            Serial.println("password_valid_flag: false");
            nvs_write_int("NVS_WifiFg", wifi_connected_flag);

            WiFi.disconnect();

            password_valid_flag = false;
            Serial.println("wifi_connected_flag: false");
            
            connected_count = 0;
            
            lv_port_lock(0);

            _ui_flag_modify(ui_SpinnerLoadPassword, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
            _ui_flag_modify(ui_KeyboardPassword, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
            lv_textarea_set_text(ui_TextPassword, "");

            lv_port_unlock();
        }
    }

    if(wifi_connected_flag) {
        timeClient.update();
        unsigned long epochTime = timeClient.getEpochTime();
        struct tm *ptm = gmtime((time_t *)&epochTime);
        snprintf(date_string, sizeof(date_string), "%04d-%02d-%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday);
        Serial.print("Now time: ");
        Serial.println(timeClient.getFormattedTime());
        Serial.print("Current date: ");
        Serial.println(date_string);
        Serial.print("Current day of the week: ");
        Serial.println(timeClient.getDay());

        ParseWeather(weather_url);

        struct timeval tv = {
            .tv_sec = epochTime - 28800,
            .tv_usec = 0,
        };
        settimeofday(&tv, NULL);

        if(WiFi.status() != WL_CONNECTED) {
            wifi_connected_flag = false;
            Serial.println("wifi_connected_flag: false");
            nvs_write_int("NVS_WifiFg", wifi_connected_flag);
            WiFi.disconnect();
        }
    } else {
        time(&now);
        localtime_r(&now, &timeinfo);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", &timeinfo);         
        strftime(date_string, sizeof(date_string), "%Y-%m-%d", &timeinfo);        
        strftime(weekday_str, sizeof(weekday_str), "%A", &timeinfo);  
    }

    if(alarm_flag) {
        static const char * btns[] ={""};

        lv_port_lock(0);

        lv_obj_t* Alarm_mbox = lv_msgbox_create(NULL, "Alarm activated", "Click the button to turn off the alarm", btns, true);
        lv_obj_center(Alarm_mbox);

        alarm_flag = false;
        Serial.println("alarm_flag: false");

        lv_port_unlock();
    }

    if(wifi_connected_flag) {
        lv_port_lock(0);
        _ui_flag_modify(ui_ImageWifi, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
        lv_port_unlock();
    } else {
        lv_port_lock(0);
        _ui_flag_modify(ui_ImageWifi, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
        lv_port_unlock();
    }
    
    delay(500);
}
