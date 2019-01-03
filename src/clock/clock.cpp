
#include <Arduino.h>
#include <time.h>
#include "clock.h"

static const char *wd[7] = {"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat"};

static tm *getLocalTime(void);

String Time_getDate()
{
    struct tm *tm = getLocalTime();
    char dateTime[26];

    sprintf(dateTime, "%04d/%02d/%02d(%s)",
            tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
            wd[tm->tm_wday]);

    return String(dateTime);
}

String Time_getTime()
{
    struct tm *tm = getLocalTime();
    char dateTime[26];

    sprintf(dateTime, "%02d:%02d:%02d",
            tm->tm_hour, tm->tm_min, tm->tm_sec);

    return String(dateTime);
}

String Time_getDateTime()
{
    String time = Time_getTime();
    String date = Time_getDate();

    return date + " " + time;
}

static tm *getLocalTime(void)
{
    time_t t = time(NULL);
    return localtime(&t);
}