#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

void convertReadings2Imperial() {
  wxConditions[0].pressure = hPa_to_inHg(wxConditions[0].pressure);
  wxForecast[1].rainfall   = mm_to_inches(wxForecast[1].rainfall);
  wxForecast[1].snowfall   = mm_to_inches(wxForecast[1].snowfall);
}

//#########################################################################################
bool decodeWeather(WiFiClient& json, String type) {
    Serial.print(F("\nCreating object...and "));
  // allocate the JsonDocument
  DynamicJsonDocument doc(35 * 1024);
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }
  // convert it to a JsonObject
  JsonObject root = doc.as<JsonObject>();
  Serial.println(" Decoding " + type + " data");
  if (type == "weather") {
    // All Serial.println statements are for diagnostic purposes and not required, remove if not needed
    wxConditions[0].lon         = root["coord"]["lon"].as<float>();                    Serial.println(" Lon: "+String(wxConditions[0].lon));
    wxConditions[0].lat         = root["coord"]["lat"].as<float>();                    Serial.println(" Lat: "+String(wxConditions[0].lat));
    wxConditions[0].main0       = root["weather"][0]["main"].as<const char*>();        Serial.println("Main: "+String(wxConditions[0].main0));
    wxConditions[0].forecast0   = root["weather"][0]["description"].as<const char*>(); Serial.println("For0: "+String(wxConditions[0].forecast0));
    wxConditions[0].forecast1   = root["weather"][1]["description"].as<const char*>(); Serial.println("For1: "+String(wxConditions[0].forecast1));
    wxConditions[0].forecast2   = root["weather"][2]["description"].as<const char*>(); Serial.println("For2: "+String(wxConditions[0].forecast2));
    wxConditions[0].icon        = root["weather"][0]["icon"].as<const char*>();        Serial.println("Icon: "+String(wxConditions[0].icon));
    wxConditions[0].temperature = root["main"]["temp"].as<float>();                    Serial.println("Temp: "+String(wxConditions[0].temperature));
    wxConditions[0].pressure    = root["main"]["pressure"].as<float>();                Serial.println("Pres: "+String(wxConditions[0].pressure));
    wxConditions[0].humidity    = root["main"]["humidity"].as<float>();                Serial.println("Humi: "+String(wxConditions[0].humidity));
    wxConditions[0].low         = root["main"]["temp_min"].as<float>();                Serial.println("TLow: "+String(wxConditions[0].low));
    wxConditions[0].high        = root["main"]["temp_max"].as<float>();                Serial.println("THig: "+String(wxConditions[0].high));
    wxConditions[0].windspeed   = root["wind"]["speed"].as<float>();                   Serial.println("WSpd: "+String(wxConditions[0].windspeed));
    wxConditions[0].winddir     = root["wind"]["deg"].as<float>();                     Serial.println("WDir: "+String(wxConditions[0].winddir));
    wxConditions[0].cloudcover  = root["clouds"]["all"].as<int>();                     Serial.println("CCov: "+String(wxConditions[0].cloudcover)); // in % of cloud cover
    wxConditions[0].visibility  = root["visibility"].as<int>();                        Serial.println("Visi: "+String(wxConditions[0].visibility)); // in metres
    wxConditions[0].rainfall    = root["rain"]["1h"].as<float>();                      Serial.println("Rain: "+String(wxConditions[0].rainfall));
    wxConditions[0].snowfall    = root["snow"]["1h"].as<float>();                      Serial.println("Snow: "+String(wxConditions[0].snowfall));
    wxConditions[0].country     = root["sys"]["country"].as<const char*>();            Serial.println("Ctry: "+String(wxConditions[0].country));
    wxConditions[0].sunrise     = root["sys"]["sunrise"].as<int>();                    Serial.println("SRis: "+String(wxConditions[0].sunrise));
    wxConditions[0].sunset      = root["sys"]["sunset"].as<int>();                     Serial.println("SSet: "+String(wxConditions[0].sunset));
    wxConditions[0].timezone    = root["timezone"].as<int>();                          Serial.println("TZon: "+String(wxConditions[0].timezone));
  }
  if (type == "forecast") {
    //Serial.println(json);
    Serial.print(F("\nReceiving Forecast period - ")); //------------------------------------------------
    JsonArray list                    = root["list"];
    for (byte r = 0; r < max_readings; r++) {
      Serial.println("\nPeriod-" + String(r) + "--------------");
      wxForecast[r].dt                = list[r]["dt"].as<int>();                                Serial.println("DTim: "+String(wxForecast[r].dt));
      wxForecast[r].temperature       = list[r]["main"]["temp"].as<float>();                    Serial.println("Temp: "+String(wxForecast[r].temperature));
      wxForecast[r].low               = list[r]["main"]["temp_min"].as<float>();                Serial.println("TLow: "+String(wxForecast[r].low));
      wxForecast[r].high              = list[r]["main"]["temp_max"].as<float>();                Serial.println("THig: "+String(wxForecast[r].high));
      wxForecast[r].pressure          = list[r]["main"]["pressure"].as<float>();                Serial.println("Pres: "+String(wxForecast[r].pressure));
      wxForecast[r].humidity          = list[r]["main"]["humidity"].as<float>();                Serial.println("Humi: "+String(wxForecast[r].humidity));
      wxForecast[r].forecast0         = list[r]["weather"][0]["main"].as<const char*>();        Serial.println("For0: "+String(wxForecast[r].forecast0));
      wxForecast[r].forecast1         = list[r]["weather"][1]["main"].as<const char*>();        Serial.println("For1: "+String(wxForecast[r].forecast1));
      wxForecast[r].forecast2         = list[r]["weather"][2]["main"].as<const char*>();        Serial.println("For2: "+String(wxForecast[r].forecast2));
      wxForecast[r].icon              = list[r]["weather"][0]["icon"].as<const char*>();        Serial.println("Icon: "+String(wxForecast[r].icon));
      wxForecast[r].description       = list[r]["weather"][0]["description"].as<const char*>(); Serial.println("Desc: "+String(wxForecast[r].description));
      wxForecast[r].cloudcover        = list[r]["clouds"]["all"].as<int>();                     Serial.println("CCov: "+String(wxForecast[r].cloudcover)); // in % of cloud cover
      wxForecast[r].windspeed         = list[r]["wind"]["speed"].as<float>();                   Serial.println("WSpd: "+String(wxForecast[r].windspeed));
      wxForecast[r].winddir           = list[r]["wind"]["deg"].as<float>();                     Serial.println("WDir: "+String(wxForecast[r].winddir));
      wxForecast[r].rainfall          = list[r]["rain"]["3h"].as<float>();                      Serial.println("Rain: "+String(wxForecast[r].rainfall));
      wxForecast[r].snowfall          = list[r]["snow"]["3h"].as<float>();                      Serial.println("Snow: "+String(wxForecast[r].snowfall));
      wxForecast[r].pop               = list[r]["pop"].as<float>();                             Serial.println("Pop:  "+String(wxForecast[r].pop));
      wxForecast[r].period            = list[r]["dt_txt"].as<const char*>();                    Serial.println("Peri: "+String(wxForecast[r].period));
    }
    //------------------------------------------
    float pressureTrend = wxForecast[2].pressure - wxForecast[0].pressure; // Measure pressure slope between ~now and later
    pressureTrend = ((int)(pressureTrend * 10)) / 10.0; // Remove any small variations less than 0.1
    wxConditions[0].trend = "0";
    if (pressureTrend > 0)  wxConditions[0].trend = "+";
    if (pressureTrend < 0)  wxConditions[0].trend = "-";
    if (pressureTrend == 0) wxConditions[0].trend = "0";

    if (units == IMPERIAL) convertReadings2Imperial();
  }
  return true;
}

bool obtain_wx_data(WiFiClient& client, const String& requestType) {
  const String owmUnits = (units == METRIC ? "metric" : "imperial");
  client.stop(); // close connection before sending a new request
  HTTPClient http;
  const String owmCity = preferences.getString("OWM_CITY");
  const String owmCountry = preferences.getString("OWM_COUNTRY");
  const String owmAPIKey = preferences.getString("OWM_APIKEY");
  const String owmLang = preferences.getString("OWM_LANG");
  String uri = "/data/2.5/" + requestType + "?q=" + owmCity + "," + owmCountry + "&APPID=" + owmAPIKey + "&mode=json&units=" + owmUnits + "&lang=" + owmLang;
  if(requestType != "weather") {
    uri += "&cnt=" + String(max_readings);
  }
  //http.begin(uri,test_root_ca); //HTTPS example connection
  http.begin(client, server, 80, uri);
  int httpCode = http.GET();
  if(httpCode == HTTP_CODE_OK) {
    if (!decodeWeather(http.getStream(), requestType)) return false;
    client.stop();
    http.end();
    return true;
  }
  else
  {
    Serial.printf("connection failed, error: %s", http.errorToString(httpCode).c_str());
    client.stop();
    http.end();
    return false;
  }
  http.end();
  return true;
}


#endif /* ifndef COMMON_H_ */
