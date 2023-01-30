#ifndef CREDENTIALS_HPP_
#define CREDENTIALS_HPP_

#include <stdint.h>

// Change to your WiFi credentials
//const char                 *ssid = "your_SSID";
//const char             *password = "your_PASSWORD";

//const char               *apiKey = "your_API_key";                  // See: https://openweathermap.org/
const char               *server = "api.openweathermap.org";
//https://api.openweathermap.org/data/2.5/forecast?q=Regensburg,DE&APPID=your_API_key&mode=json&units=metric&cnt=40
//https://api.openweathermap.org/data/2.5/weather?q=Regensburg,DE&APPID=your_API_key&mode=json&units=metric&cnt=1
//Set your location according to OWM locations
//const char                 *city = "your_City";                     // Your home city, see: http://bulk.openweathermap.org/sample/
//const char              *country = "Home";                          // see: https://en.wikipedia.org/wiki/List_of_ISO_3166_country_codes
//const char             *language = "EN";
//const HemisphereEnum  hemisphere = NORTH;
//const UnitsEnum            units = METRIC;
//const char             *timezone = "";                              // see: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char            *ntpServer = "pool.ntp.org";

#endif