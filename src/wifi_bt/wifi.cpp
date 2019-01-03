#include "wifi.h"
#include "clock/clock.h"
#include "serial.h"

#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <WebServer.h>
#endif
#include <time.h>
#include <AutoConnect.h>

#if defined(ARDUINO_ARCH_ESP8266)
ESP8266WebServer Server;
#elif defined(ARDUINO_ARCH_ESP32)
WebServer Server;
#endif
AutoConnect Portal(Server);
AutoConnectConfig Config; // Enable autoReconnect supported on v0.9.4

#define TIMEZONE (3600 * 9)          // Tokyo
#define NTPServer1 "at.pool.ntp.org" // NICT japan.
#define NTPServer2 "europe.pool.ntp.org"

#define SHORTNAME "WI"

void rootPage()
{
    String content =
        "<html>"
        "<head>"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "</head>"
        "<body>"
        "<h2 align=\"center\" style=\"color:blue;margin:20px;\">Hello, world</h2>"
        "<h3 align=\"center\" style=\"color:gray;margin:10px;\">{{DateTime}}</h3>"
        "<p style=\"padding-top:10px;text-align:center\">" AUTOCONNECT_LINK(COG_32) "</p>"
                                                                                    "</body>"
                                                                                    "</html>";

    content.replace("{{DateTime}}", Time_getDateTime());
    Server.send(200, "text/html", content);
}

void WIFI_init()
{
    Server.on("/", rootPage);

    Config.autoReconnect = true;
    Portal.config(Config);

    if (Portal.begin())
    {
        Serial_log(SHORTNAME, "WiFi connected: " + WiFi.localIP().toString());
        configTime(3600, 0, NTPServer1, NTPServer2);
        // setenv("TZ", "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", 1);
    }
}

void WIFI_handleOperations()
{
    Portal.handleClient();
}

char *WIFI_getStatus()
{
    return (WiFi.getMode() == WIFI_OFF) ? (char *)"OFF" : (char *)"ON";
}

void WIFI_ToggleWifi()
{
    if (WiFi.getMode() != WIFI_OFF)
    {
        WiFi.mode(WIFI_OFF);
        Serial_log(SHORTNAME, "Switching WiFI OFF");
    }
    else
    {
        Portal.begin();
        Serial_log(SHORTNAME, "Switching WiFI ON");
    }
}