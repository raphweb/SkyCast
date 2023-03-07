#ifndef COMMON_FUNCTIONS_HPP_
#define COMMON_FUNCTIONS_HPP_

float mm_to_inches(float value_mm) {
  return 0.0393701 * value_mm;
}

float hPa_to_inHg(float value_hPa) {
  return 0.02953 * value_hPa;
}

float sumOfPrecip(float dataArray[], int readings) {
  float sum = 0;
  for (int i = 0; i < readings; i++) {
    sum += dataArray[i];
  }
  return sum;
}

char *titleCase(char *text) {
  if (text == nullptr) return text;
  size_t length = strlen(text);
  if (length < 1) return text;
  text[0] = toupper(text[0]);
  return text;
}

String convertUnixTime(int unix_time) {
  // Returns either '21:12  ' or ' 09:12pm' depending on Units mode
  time_t tm = unix_time;
  struct tm *now_tm = gmtime(&tm);
  char output[40];
  if (units == METRIC) {
    strftime(output, sizeof(output), "%H:%M %d/%m/%y", now_tm);
  } else {
    strftime(output, sizeof(output), "%I:%M%P %m/%d/%y", now_tm);
  }
  return output;
}

// from range 0 .. 1.0: moon phases 0 -> new moon .. waxing .. 0.5 full moon .. waning .. 1 -> new moon
float getMoonPhase(int d, int m, int y) {
  int c, e;
  float jd;
  if (m < 3) {
    y--;
    m += 12;
  }
  ++m;
  c   = 365.25 * y;
  e   = 30.6  * m;
  jd  = c + e + d - 694039.09;              /* jd is total days elapsed */
  jd /= 29.53059;                           /* divide by the moon cycle (29.53 days) */
  jd -= (int)jd;                            /* subtract integer part to leave fractional part of jd */
  jd = (hemisphere == SOUTH) ? 1 - jd : jd; /* consider hemisphere */
  return jd;
}

String moonPhase(int d, int m, int y) {
  const float phase = getMoonPhase(d, m, y);
  /* scale fraction from 0-8 and round by adding 0.5; 0 and 8 are the same phase so modulo 8 for 0 */
  const uint8_t b = (uint8_t)(phase * 8 + 0.5) & 7;
  if (b == 0) return TXT_MOON_NEW;              // New;              0%  illuminated
  if (b == 1) return TXT_MOON_WAXING_CRESCENT;  // Waxing crescent; 25%  illuminated
  if (b == 2) return TXT_MOON_FIRST_QUARTER;    // First quarter;   50%  illuminated
  if (b == 3) return TXT_MOON_WAXING_GIBBOUS;   // Waxing gibbous;  75%  illuminated
  if (b == 4) return TXT_MOON_FULL;             // Full;            100% illuminated
  if (b == 5) return TXT_MOON_WANING_GIBBOUS;   // Waning gibbous;  75%  illuminated
  if (b == 6) return TXT_MOON_THIRD_QUARTER;    // Third quarter;   50%  illuminated
  if (b == 7) return TXT_MOON_WANING_CRESCENT;  // Waning crescent; 25%  illuminated
  return "";
}


#endif /* ifndef COMMON_FUNCTIONS_H_ */
