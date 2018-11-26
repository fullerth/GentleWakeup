#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "esp_log.h"

#include "lwip/err.h"

RTC_DATA_ATTR static time_t alarm_time;

void alarm_create_new(time_t alert) {
    alarm_time = alert;
}

bool is_alarm_active(time_t now) {
    struct tm timeinfo, alarminfo;
    localtime_r(&now, &timeinfo);
    localtime_r(&alarm_time, &alarminfo);
    bool alarm_active = false;

    if(alarminfo.tm_hour == timeinfo.tm_hour && alarminfo.tm_min == timeinfo.tm_min && alarminfo.tm_sec < timeinfo.tm_sec) {
        alarm_active=true;
    }

    return alarm_active;
}
