#include <Arduino.h>
#include <Preferences.h>
Preferences preferences;
#include <WiFi.h>
#include <time.h>

enum HemisphereEnum { NORTH  = 0, SOUTH    = 1 };
enum UnitsEnum      { METRIC = 0, IMPERIAL = 1 };
HemisphereEnum hemisphere;
UnitsEnum           units;

#include "Credentials.hpp"
#include "lang_de.hpp"
#include "ForecastType.hpp"
#include "CommonFunctions.hpp"
#include "Common.hpp"
#include "DisplayConfig.hpp"
#include "DrawingRoutines.hpp"


String timeStr, dateStr;
int wifi_signal, currentHour = 0, currentMin = 0, currentSec = 0;
uint64_t startTime = 0;


#define sleepDuration 30
#define wakeupTime     6
#define sleepTime     22
//#define SAVE_CREDENTIALS


void beginSleep() {
  display.powerOff();
  uint64_t sleepTimer = sleepDuration * 60;
  uint64_t offset = (currentMin % sleepDuration) * 60 + currentSec;
  if (offset > sleepDuration/2 * 60) {
    offset -= sleepDuration * 60;
  }
  esp_sleep_enable_timer_wakeup((sleepTimer - offset) * 1000000LL);
#ifdef BUILTIN_LED
  pinMode(BUILTIN_LED, INPUT); // If it's On, turn it off and some boards use GPIO-5 for SPI-SS, which remains low after screen use
  digitalWrite(BUILTIN_LED, HIGH);
#endif
  Serial.println("Entering " + String((unsigned long)sleepTimer) + "-secs of sleep time");
  Serial.println("Awake for : " + String((millis() - startTime) / 1000.0, 3) + "-secs");
  Serial.println("Starting deep-sleep period...");
  preferences.end();
  esp_deep_sleep_start();      // Sleep for e.g. 30 minutes
}

uint8_t startWiFi() {
  const String wifissid = preferences.getString("WIFI_SSID");
  const String wifipass = preferences.getString("WIFI_PASS");
  Serial.print("\r\nConnecting to: "); Serial.println(wifissid);
  IPAddress dns(8, 8, 8, 8); // Google DNS
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(wifissid.c_str(), wifipass.c_str());
  unsigned long start = millis();
  uint8_t connectionStatus;
  bool attemptConnection = true;
  while (attemptConnection) {
    connectionStatus = WiFi.status();
    if (millis() > start + 15000) { // Wait 15-secs maximum
      attemptConnection = false;
    }
    if (connectionStatus == WL_CONNECTED || connectionStatus == WL_CONNECT_FAILED) {
      attemptConnection = false;
    }
    delay(50);
  }
  if (connectionStatus == WL_CONNECTED) {
    wifi_signal = WiFi.RSSI(); // Get Wifi Signal strength now, because the WiFi will be turned off to save power!
    Serial.println("WiFi connected at: " + WiFi.localIP().toString());
  }
  else Serial.println("WiFi connection *** FAILED ***");
  return connectionStatus;
}

void stopWiFi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

boolean setupTime() {
  const String timezone = preferences.getString("TIMEZONE");
  configTzTime(timezone.c_str(), ntpServer);
  delay(1000);
  Serial.print("Waiting for NTP time sync: ");
  time_t nowSecs = time(nullptr);
  while (nowSecs < 3600) { // maximum wait for an hour
    delay(500);
    Serial.print(".");
    yield();
    nowSecs = time(nullptr);
  }
  Serial.println();
  struct tm timeinfo;
  localtime_r(&nowSecs, &timeinfo);
  currentHour = timeinfo.tm_hour;
  currentMin  = timeinfo.tm_min;
  currentSec  = timeinfo.tm_sec;
  char time_output[30], day_output[30], update_time[30];
  if (units == METRIC) {
    sprintf(day_output, dateFormat, weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900);
    strftime(update_time, sizeof(update_time), "%H:%M:%S", &timeinfo);
    sprintf(time_output, "%s", update_time);
  } else {
    strftime(day_output, sizeof(day_output), "%a  %b-%d-%Y", &timeinfo);
    strftime(update_time, sizeof(update_time), "%r", &timeinfo);
    sprintf(time_output, "%s", update_time);
  }
  dateStr = day_output;
  timeStr = time_output;
  return true;
}


void setup() {
  startTime = millis();
  Serial.begin(115200);
  while(!Serial) {}
#ifdef SAVE_CREDENTIALS
  {
    preferences.begin("SkyCast");
    preferences.clear();
    preferences.putString("WIFI_SSID", "Ssid");
    preferences.putString("WIFI_PASS", "Pass");
    preferences.putString("OWM_CITY", "City");
    preferences.putString("OWM_COUNTRY", "Home");
    preferences.putString("OWM_APIKEY", "your API key");
    preferences.putString("OWM_LANG", "EN");
    preferences.putString("TIMEZONE", "");
    preferences.putBool("HEMISPHERE", NORTH);
    preferences.putBool("UNITS", METRIC);
    preferences.end();
    esp_deep_sleep_start();
  }
#endif
  preferences.begin("SkyCast", true);
  hemisphere = (HemisphereEnum)preferences.getBool("HEMISPHERE", 0);
  units = (UnitsEnum)preferences.getBool("UNITS", 0);
  if (startWiFi() == WL_CONNECTED && setupTime() == true) {
    if (currentHour >= wakeupTime && currentHour <= sleepTime) {
      initialiseDisplay(); // Give screen time to initialise by getting weather data!
      byte attempts = 1;
      bool rxWeather = false, rxForecast = false;
      WiFiClient client;   // wifi client object
      while ((rxWeather == false || rxForecast == false) && attempts <= 2) { // Try up-to 2 time for Weather and Forecast data
        if (rxWeather  == false) rxWeather  = obtain_wx_data(client, "weather");
        if (rxForecast == false) rxForecast = obtain_wx_data(client, "forecast");
        attempts++;
      }
      if (rxWeather && rxForecast) { // Only if received both Weather or Forecast proceed
        stopWiFi(); // Reduces power consumption
        Serial.println("Date: " + dateStr + ", time: " + timeStr);
        display.firstPage();
        do {
          displayWeather(dateStr.c_str(), timeStr.c_str());
        } while (display.nextPage());
      }
    }
  }
  beginSleep();
}

void loop() {} // this will never run!
