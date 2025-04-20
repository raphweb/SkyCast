#ifndef LANG_DE_HPP_
#define LANG_DE_HPP_

#define FONT(x) x##_tf

//Temperatur - Luftfeuchtigkeit - Vorschau
const char *TXT_FORECAST_VALUES  = "3 Tages Vorhersage";
const char *TXT_CONDITIONS       = "Aktuelles Wetter";
const char *TXT_DAYS             = "(Tage)";
const char *TXT_TEMPERATURES     = "Temperatur";
const char *TXT_TEMPERATURE_C    = "Temperatur [°C]";
const char *TXT_TEMPERATURE_F    = "Temperatur [°F]";
const char *TXT_HUMIDITY_PERCENT = "Luftfeuchte [%]";

//Luftdruck
const char *TXT_PRESSURE         = "Luftdruck";
const char *TXT_PRESSURE_HPA     = "Luftdruck [hPa]";
const char *TXT_PRESSURE_IN      = "Luftdruck [in]";
const char *TXT_PRESSURE_STEADY  = "beständig";
const char *TXT_PRESSURE_RISING  = "steigend";
const char *TXT_PRESSURE_FALLING = "fallend";

//Niederschlag / Schneefall
const char *TXT_RAINFALL_MM = "mm Regen";
const char *TXT_RAINFALL_IN = "in Regen";
const char *TXT_SNOWFALL_MM = "mm Schnee";
const char *TXT_SNOWFALL_IN = "in Schnee";
const char *TXT_PRECIPITATION_SOON = "in Kürze";

//Sonne
const char *TXT_SUNRISE  = "Sonnenaufgang";
const char *TXT_SUNSET   = "Sonnenuntergang";

//Mond
const char *TXT_MOON_NEW             = "Neumond";              // Neumond
const char *TXT_MOON_WAXING_CRESCENT = "zunehmende Sichel";    // erstes Viertel bzw. zunehmende Sichel
const char *TXT_MOON_FIRST_QUARTER   = "zunehmender Halbmond"; // zunehmender Halbmond 
const char *TXT_MOON_WAXING_GIBBOUS  = "zweites Viertel";      // zweites Viertel
const char *TXT_MOON_FULL            = "Vollmond";             // Vollmond
const char *TXT_MOON_WANING_GIBBOUS  = "drittes Viertel";      // drittes Viertel 
const char *TXT_MOON_THIRD_QUARTER   = "abnehmender Halbmond"; // abnehmender Halbmond
const char *TXT_MOON_WANING_CRESCENT = "abnehmende Sichel";    // letztes Viertel bzw. abnehmende Sichel 

//Energie / WLAN
const char *TXT_POWER   = "Akku";
const char *TXT_WIFI    = "WLAN";
const char *TXT_UPDATED = "aktualisiert:";

//Wind
const char *TXT_WIND_SPEED_DIRECTION = "Wind Geschw./Richtung";
const char *TXT_N   = "N";
const char *TXT_NNE = "NNO";
const char *TXT_NE  = "NO";
const char *TXT_ENE = "ONO";
const char *TXT_E   = "O";
const char *TXT_ESE = "OSO";
const char *TXT_SE  = "SO";
const char *TXT_SSE = "SSO";
const char *TXT_S   = "S";
const char *TXT_SSW = "SSW";
const char *TXT_SW  = "SW";
const char *TXT_WSW = "WSW";
const char *TXT_W   = "W";
const char *TXT_WNW = "WNW";
const char *TXT_NW  = "NW";
const char *TXT_NNW = "NNW";
const char *TXT_RH  = "relative Luftfeuchtigkeit";

const char *dateFormat = "%s, %02u. %s %04u";

//Wochentag
const char *weekday_D[] =  { "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag" };

//Monat
const char *month_M[] = { "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember" };

//Geburtstagsnachricht
const char *bDayStr = "Strahlender Sonnenschein, %s wird heute %u!";

#endif