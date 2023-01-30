#ifndef FORECAST_TYPE_H_
#define FORECAST_TYPE_H_

typedef struct {
  int      dt;
  String   period;
  String   icon;
  String   trend;
  String   main0;
  String   forecast0;
  String   forecast1;
  String   forecast2;
  String   description;
  String   time;
  String   country;
  float    lat;
  float    lon;
  float    temperature;
  float    feelslike;
  float    humidity;
  float    high;
  float    low;
  float    winddir;
  float    windspeed;
  float    rainfall;
  float    snowfall;
  float    pop;
  float    pressure;
  int      cloudcover;
  int      visibility;
  int      sunrise;
  int      sunset;
  int      timezone;
} Forecast_t;

#define max_readings 24

Forecast_t wxConditions[1];
Forecast_t wxForecast[max_readings];

float pressure_readings[max_readings]    = {0};
float temperature_readings[max_readings] = {0};
float humidity_readings[max_readings]    = {0};
float rain_readings[max_readings]        = {0};
float snow_readings[max_readings]        = {0};

#endif /* ifndef FORECAST_RECORD_H_ */
