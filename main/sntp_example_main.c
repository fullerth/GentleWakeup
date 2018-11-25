/* LwIP SNTP example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"

#include "time_service.h"

#include "lwip/err.h"
#include "apps/sntp/sntp.h"

static const char *TAG = "example";

/* Variable holding number of times ESP32 restarted since first boot.
 * It is placed into RTC memory using RTC_DATA_ATTR and
 * maintains its value when ESP32 wakes from deep sleep.
 */
RTC_DATA_ATTR static int boot_count = 0;

void app_main()
{
    ++boot_count;
    ESP_LOGI(TAG, "Boot count: %d", boot_count);

    time_t now;
    struct tm timeinfo;
    struct tm alarminfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        time_service_get_time();
        ESP_LOGI(TAG, "Time Obtained");
        // update 'now' variable with current time
        time(&now);
    }
    char strftime_buf[64];

    RTC_DATA_ATTR static time_t alarm;
    RTC_DATA_ATTR static uint8_t init_once = 0;
    if(init_once == 0) {
        alarm = now + 30;
        init_once = 1;
    }
    ESP_LOGI(TAG, "Time is: %lu", now);
    ESP_LOGI(TAG, "Alarm is: %lu", alarm);

    // Set timezone to Pacific Standard Time and print local time
    setenv("TZ", "PST8PDT", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    localtime_r(&alarm, &alarminfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time on the West Coast is: %s", strftime_buf);
    ESP_LOGI(TAG, "Current Hour is: %i. Min is: %i. Sec is: %i", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    ESP_LOGI(TAG, "Alarm Hour is: %i. Min is: %i. Sec is: %i", alarminfo.tm_hour, alarminfo.tm_min, alarminfo.tm_sec);

    if(alarminfo.tm_hour == timeinfo.tm_hour && alarminfo.tm_min == timeinfo.tm_min && alarminfo.tm_sec < timeinfo.tm_sec) {
        ESP_LOGI(TAG, "ALARM ACTIVE!");
    }

    const int deep_sleep_sec = 10;
    ESP_LOGI(TAG, "Entering deep sleep for %d seconds", deep_sleep_sec);
    esp_deep_sleep(1000000LL * deep_sleep_sec - 2000);
}

