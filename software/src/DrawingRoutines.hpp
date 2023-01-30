#ifndef DRAWING_ROUTINES_HPP_
#define DRAWING_ROUTINES_HPP_

enum AlignmentKind {
  LEFT          = 1,
  CENTER        = 2,
  RIGHT         = 4,
  BOTTOM        = 8,
  MIDDLE        = 16,
  TOP           = 32,
  BOTTOM_LEFT   = BOTTOM | LEFT,
  MIDDLE_LEFT   = MIDDLE | LEFT,
  TOP_LEFT      = TOP    | LEFT,
  BOTTOM_CENTER = BOTTOM | CENTER,
  MIDDLE_CENTER = MIDDLE | CENTER,
  TOP_CENTER    = TOP    | CENTER,
  BOTTOM_RIGHT  = BOTTOM | RIGHT,
  MIDDLE_RIGHT  = MIDDLE | RIGHT,
  TOP_RIGHT     = TOP    | RIGHT
};


void drawString(int16_t x, int16_t y, String text, const AlignmentKind alignment = BOTTOM_LEFT) {
    //drawString(x, y, text.c_str(), alignment);
}

void drawString(int16_t x, int16_t y, const char *text, const AlignmentKind alignment = BOTTOM_LEFT) {
    const uint16_t w = u8g2Fonts.getUTF8Width(text);
    const uint16_t h = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent();
    if (alignment & RIGHT)  x -= w;
    if (alignment & CENTER) x -= w / 2;
    if (alignment & MIDDLE) y += h / 2;
    if (alignment & TOP)    y += h;
    //display.drawRect(x, y-u8g2Fonts.getFontAscent(), w, h, GxEPD_RED);
    u8g2Fonts.drawUTF8(x, y, text);
}

void drawStringMaxWidth(int16_t x, int16_t y, uint16_t text_width, char *text, const AlignmentKind alignment = BOTTOM_LEFT) {
    const uint16_t w = u8g2Fonts.getUTF8Width(text);
    const uint16_t h = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent();
    if (alignment & RIGHT)  x -= w;
    if (alignment & CENTER) x -= w /2;
    if (alignment & MIDDLE) y += h / 2;
    if (alignment & TOP)    y += h;
    size_t length = strlen(text);
    if (length > text_width) {
        text[text_width-1] = 0;
    }
    u8g2Fonts.drawUTF8(x, y, text);
    if (length > text_width) {
        text = &(text[text_width]);
        u8g2Fonts.drawUTF8(x, y + h + 10, text);
    }
}


#define autoscale_on  true
#define autoscale_off false
#define barchart_on   true
#define barchart_off  false

#define largeIcon true
#define smallIcon false
#define large 11
#define small 5


String windDegToDirection(float winddirection) {
  if (winddirection >= 348.75 || winddirection < 11.25)  return TXT_N;
  if (winddirection >=  11.25 && winddirection < 33.75)  return TXT_NNE;
  if (winddirection >=  33.75 && winddirection < 56.25)  return TXT_NE;
  if (winddirection >=  56.25 && winddirection < 78.75)  return TXT_ENE;
  if (winddirection >=  78.75 && winddirection < 101.25) return TXT_E;
  if (winddirection >= 101.25 && winddirection < 123.75) return TXT_ESE;
  if (winddirection >= 123.75 && winddirection < 146.25) return TXT_SE;
  if (winddirection >= 146.25 && winddirection < 168.75) return TXT_SSE;
  if (winddirection >= 168.75 && winddirection < 191.25) return TXT_S;
  if (winddirection >= 191.25 && winddirection < 213.75) return TXT_SSW;
  if (winddirection >= 213.75 && winddirection < 236.25) return TXT_SW;
  if (winddirection >= 236.25 && winddirection < 258.75) return TXT_WSW;
  if (winddirection >= 258.75 && winddirection < 281.25) return TXT_W;
  if (winddirection >= 281.25 && winddirection < 303.75) return TXT_WNW;
  if (winddirection >= 303.75 && winddirection < 326.25) return TXT_NW;
  if (winddirection >= 326.25 && winddirection < 348.75) return TXT_NNW;
  return "?";
}


void arrow(int x, int y, int asize, float aangle, int pwidth, int plength) {
  float dx = (asize+7) * cos((aangle - 90) * PI / 180) + x; // calculate X position
  float dy = (asize+7) * sin((aangle - 90) * PI / 180) + y; // calculate Y position
  float x1 = 0;           float y1 = plength;
  float x2 = pwidth / 2;  float y2 = pwidth / 2;
  float x3 = -pwidth / 2; float y3 = pwidth / 2;
  float angle = aangle * PI / 180;
  float xx1 = x1 * cos(angle) - y1 * sin(angle) + dx;
  float yy1 = y1 * cos(angle) + x1 * sin(angle) + dy;
  float xx2 = x2 * cos(angle) - y2 * sin(angle) + dx;
  float yy2 = y2 * cos(angle) + x2 * sin(angle) + dy;
  float xx3 = x3 * cos(angle) - y3 * sin(angle) + dx;
  float yy3 = y3 * cos(angle) + x3 * sin(angle) + dy;
  display.fillTriangle(xx1, yy1, xx3, yy3, xx2, yy2, GxEPD_BLACK);
}


void displayDisplayWindSection(int x, int y, float angle, float windspeed, int cradius) {
  arrow(x, y, cradius, angle, 12, 18); // Show wind direction on outer circle of width and length
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  int dxo, dyo, dxi, dyi;
  display.drawLine(0, 15, 0, y + cradius + 30, GxEPD_BLACK);
  display.drawCircle(x, y, cradius, GxEPD_BLACK);     // Draw compass circle
  display.drawCircle(x, y, cradius + 1, GxEPD_BLACK); // Draw compass circle
  display.drawCircle(x, y, cradius * 0.7, GxEPD_BLACK); // Draw compass inner circle
  for (float a = 0; a < 360; a = a + 22.5) {
    dxo = cradius * cos((a - 90) * PI / 180);
    dyo = cradius * sin((a - 90) * PI / 180);
    if (a == 45)  drawString(dxo + x + 10, dyo + y - 11, TXT_NE, TOP_CENTER);
    if (a == 135) drawString(dxo + x + 10, dyo + y + 1,  TXT_SE, TOP_CENTER);
    if (a == 225) drawString(dxo + x - 15, dyo + y - 3,  TXT_SW, TOP_CENTER);
    if (a == 315) drawString(dxo + x - 13, dyo + y - 11, TXT_NW, TOP_CENTER);
    dxi = dxo * 0.9;
    dyi = dyo * 0.9;
    display.drawLine(dxo + x, dyo + y, dxi + x, dyi + y, GxEPD_BLACK);
    dxo = dxo * 0.7;
    dyo = dyo * 0.7;
    dxi = dxo * 0.9;
    dyi = dyo * 0.9;
    display.drawLine(dxo + x, dyo + y, dxi + x, dyi + y, GxEPD_BLACK);
  }
  drawString(x, y - cradius - 13,     TXT_N, TOP_CENTER);
  drawString(x, y + cradius + 2,      TXT_S, TOP_CENTER);
  drawString(x - cradius - 10, y - 6, TXT_W, TOP_CENTER);
  drawString(x + cradius + 8,  y - 6, TXT_E, TOP_CENTER);
  drawString(x, y - 21, windDegToDirection(angle).c_str(), TOP_CENTER);
  drawString(x, y + 11, String(String(angle, 0) + "°").c_str(), TOP_CENTER);
  drawString(x, y - 4, String(String(windspeed, 1) + (units == METRIC ? "m/s" : "mph")).c_str(), TOP_CENTER);
}


void drawPressureAndTrend(int x, int y, float pressure, String slope) {
  drawString(x, y - 1, String(String(pressure, (units == METRIC ? 0 : 1)) + (units == METRIC ? "hPa" : "in")).c_str(), TOP_CENTER);
  x = x + 40; y = y + 2;
  if      (slope == "+") {
    display.drawLine(x,  y, x + 4, y - 4, GxEPD_BLACK);
    display.drawLine(x + 4, y - 4, x + 8, y, GxEPD_BLACK);
  }
  else if (slope == "0") {
    display.drawLine(x + 4, y - 4, x + 8, y, GxEPD_BLACK);
    display.drawLine(x + 4, y + 4, x + 8, y, GxEPD_BLACK);
  }
  else if (slope == "-") {
    display.drawLine(x,  y, x + 4, y + 4, GxEPD_BLACK);
    display.drawLine(x + 4, y + 4, x + 8, y, GxEPD_BLACK);
  }
}


//#########################################################################################
/* (C) D L BIRD
    This function will draw a graph on a ePaper/TFT/LCD display using data from an array containing data to be graphed.
    The variable 'max_readings' determines the maximum number of data elements for each array. Call it with the following parametric data:
    x_pos - the x axis top-left position of the graph
    y_pos - the y-axis top-left position of the graph, e.g. 100, 200 would draw the graph 100 pixels along and 200 pixels down from the top-left of the screen
    width - the width of the graph in pixels
    height - height of the graph in pixels
    Y1_Max - sets the scale of plotted data, for example 5000 would scale all data to a Y-axis of 5000 maximum
    data_array1 is parsed by value, externally they can be called anything else, e.g. within the routine it is called data_array1, but externally could be temperature_readings
    auto_scale - a logical value (TRUE or FALSE) that switches the Y-axis autoscale On or Off
    barchart_on - a logical value (TRUE or FALSE) that switches the drawing mode between bar and line graphs
    barchart_colour - a sets the title and graph plotting colour
    If called with Y!_Max value of 500 and the data never goes above 500, then autoscale will retain a 0-500 Y scale, if on, the scale increases/decreases to match the data.
    auto_scale_margin, e.g. if set to 1000 then autoscale increments the scale by 1000 steps.
*/
void drawGraph(int x_pos, int y_pos, int gwidth, int gheight, float y1Min, float y1Max, const char *title, float dataArray[], int readings, boolean auto_scale, boolean barchart_mode) {
#define auto_scale_margin 0 // Sets the autoscale increment, so axis steps up in units of e.g. 3
#define y_minor_axis 5      // 5 y-axis division markers
  float maxYscale = -10000;
  float minYscale =  10000;
  int last_x, last_y;
  float x2, y2;
  if (auto_scale == true) {
    for (int i = 1; i < readings; i++ ) {
      if (dataArray[i] >= maxYscale) maxYscale = dataArray[i];
      if (dataArray[i] <= minYscale) minYscale = dataArray[i];
    }
    maxYscale = round(maxYscale + auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Max
    y1Max = round(maxYscale + 0.5);
    if (minYscale != 0) minYscale = round(minYscale - auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Min
    y1Min = round(minYscale);
  }
  // Draw the graph
  last_x = x_pos + 1;
  last_y = y_pos + (y1Max - constrain(dataArray[1], y1Min, y1Max)) / (y1Max - y1Min) * gheight;
  display.drawRect(x_pos, y_pos, gwidth + 3, gheight + 2, GxEPD_BLACK);
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  drawString(x_pos + gwidth / 2, y_pos - 12, title, TOP_CENTER);
  // Draw the data
  for (int gx = 1; gx < readings; gx++) {
    x2 = x_pos + gx * gwidth / (readings - 1) - 1 ; // max_readings is the global variable that sets the maximum data that can be plotted
    y2 = y_pos + (y1Max - constrain(dataArray[gx], y1Min, y1Max)) / (y1Max - y1Min) * gheight + 1;
    if (barchart_mode) {
      display.fillRect(x2, y2, (gwidth / readings) - 1, y_pos + gheight - y2 + 1, GxEPD_BLACK);
    } else {
      display.drawLine(last_x, last_y, x2, y2, GxEPD_BLACK);
    }
    last_x = x2;
    last_y = y2;
  }
  //Draw the Y-axis scale
#define number_of_dashes 15
  for (int spacing = 0; spacing <= y_minor_axis; spacing++) {
    for (int j = 0; j < number_of_dashes; j++) { // Draw dashed graph grid lines
      if (spacing < y_minor_axis) display.drawFastHLine((x_pos + 3 + j * gwidth / number_of_dashes), y_pos + (gheight * spacing / y_minor_axis), gwidth / (2 * number_of_dashes), GxEPD_BLACK);
    }
    if ((y1Max - (float)(y1Max - y1Min) / y_minor_axis * spacing) < 5 || title == TXT_PRESSURE_IN) {
      drawString(x_pos, y_pos + gheight * spacing / y_minor_axis - 5, String((y1Max - (float)(y1Max - y1Min) / y_minor_axis * spacing + 0.01), 1).c_str(), TOP_RIGHT);
    } else {
      if (y1Min < 1 && y1Max < 10)
        drawString(x_pos - 3, y_pos + gheight * spacing / y_minor_axis - 5, String((y1Max - (float)(y1Max - y1Min) / y_minor_axis * spacing + 0.01), 1).c_str(), TOP_RIGHT);
      else
        drawString(x_pos - 3, y_pos + gheight * spacing / y_minor_axis - 5, String((y1Max - (float)(y1Max - y1Min) / y_minor_axis * spacing + 0.01), 0).c_str(), TOP_RIGHT);
    }
  }
  for (int i = 0; i <= 2; i++) {
    drawString(15 + x_pos + gwidth / 3 * i, y_pos + gheight + 3, String(i).c_str(), TOP_LEFT);
  }
  drawString(x_pos + gwidth / 2, y_pos + gheight + 10, TXT_DAYS, TOP_CENTER);
}


void drawMoon(int x, int y, int dd, int mm, int yy) {
  const int diameter = 38;
  double phase = normalizedMoonPhase(dd, mm, yy);
  if (hemisphere == SOUTH) phase = 1 - phase;
  // Draw dark part of moon
  display.fillCircle(x + diameter - 1, y + diameter, diameter / 2 + 1, GxEPD_BLACK);
  const int number_of_lines = 90;
  for (double ypos = 0; ypos <= 45; ypos++) {
    double xpos = sqrt(45 * 45 - ypos * ypos);
    // Determine the edges of the lighted part of the moon
    double rpos = 2 * xpos;
    double xpos1, xpos2;
    if (phase < 0.5) {
      xpos1 = - xpos;
      xpos2 = (rpos - 2 * phase * rpos - xpos);
    } else {
      xpos1 = xpos;
      xpos2 = (xpos - 2 * phase * rpos + rpos);
    }
    // Draw light part of moon
    double pW1x = (xpos1 + number_of_lines) / number_of_lines * diameter + x;
    double pW1y = (number_of_lines - ypos)  / number_of_lines * diameter + y;
    double pW2x = (xpos2 + number_of_lines) / number_of_lines * diameter + x;
    double pW2y = (number_of_lines - ypos)  / number_of_lines * diameter + y;
    double pW3x = (xpos1 + number_of_lines) / number_of_lines * diameter + x;
    double pW3y = (ypos + number_of_lines)  / number_of_lines * diameter + y;
    double pW4x = (xpos2 + number_of_lines) / number_of_lines * diameter + x;
    double pW4y = (ypos + number_of_lines)  / number_of_lines * diameter + y;
    display.drawLine(pW1x, pW1y, pW2x, pW2y, GxEPD_WHITE);
    display.drawLine(pW3x, pW3y, pW4x, pW4y, GxEPD_WHITE);
  }
  display.drawCircle(x + diameter - 1, y + diameter, diameter / 2 + 1, GxEPD_BLACK);
}


String moonPhase(int d, int m, int y) {
  int c, e;
  double jd;
  int b;
  if (m < 3) {
    y--;
    m += 12;
  }
  ++m;
  c   = 365.25 * y;
  e   = 30.6  * m;
  jd  = c + e + d - 694039.09;     /* jd is total days elapsed */
  jd /= 29.53059;                  /* divide by the moon cycle (29.53 days) */
  b   = jd;                        /* int(jd) -> b, take integer part of jd */
  jd -= b;                         /* subtract integer part to leave fractional part of original jd */
  b   = jd * 8 + 0.5;              /* scale fraction from 0-8 and round by adding 0.5 */
  b   = b & 7;                     /* 0 and 8 are the same phase so modulo 8 for 0 */
  if (hemisphere == SOUTH) b = 7 - b;
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


// Symbols are drawn on a relative 10x10grid and 1 scale unit = 1 drawing unit
void addcloud(int x, int y, int scale, int linesize) {
  //Draw cloud outer
  display.fillCircle(x - scale * 3, y, scale, GxEPD_BLACK);                      // Left most circle
  display.fillCircle(x + scale * 3, y, scale, GxEPD_BLACK);                      // Right most circle
  display.fillCircle(x - scale, y - scale, scale * 1.4, GxEPD_BLACK);            // left middle upper circle
  display.fillCircle(x + scale * 1.5, y - scale * 1.3, scale * 1.75, GxEPD_BLACK); // Right middle upper circle
  display.fillRect(x - scale * 3 - 1, y - scale, scale * 6, scale * 2 + 1, GxEPD_BLACK); // Upper and lower lines
  //Clear cloud inner
  display.fillCircle(x - scale * 3, y, scale - linesize, GxEPD_WHITE);           // Clear left most circle
  display.fillCircle(x + scale * 3, y, scale - linesize, GxEPD_WHITE);           // Clear right most circle
  display.fillCircle(x - scale, y - scale, scale * 1.4 - linesize, GxEPD_WHITE); // left middle upper circle
  display.fillCircle(x + scale * 1.5, y - scale * 1.3, scale * 1.75 - linesize, GxEPD_WHITE); // Right middle upper circle
  display.fillRect(x - scale * 3 + 2, y - scale + linesize - 1, scale * 5.9, scale * 2 - linesize * 2 + 2, GxEPD_WHITE); // Upper and lower lines
}


void addraindrop(int x, int y, int scale) {
  display.fillCircle(x, y, scale / 2, GxEPD_BLACK);
  display.fillTriangle(x - scale / 2, y, x, y - scale * 1.2, x + scale / 2, y , GxEPD_BLACK);
  x = x + scale * 1.6; y = y + scale / 3;
  display.fillCircle(x, y, scale / 2, GxEPD_BLACK);
  display.fillTriangle(x - scale / 2, y, x, y - scale * 1.2, x + scale / 2, y , GxEPD_BLACK);
}


void addrain(int x, int y, int scale, bool iconSize) {
  if (iconSize == smallIcon) scale *= 1.34;
  for (int d = 0; d < 4; d++) {
    addraindrop(x + scale * (7.8 - d * 1.95) - scale * 5.2, y + scale * 2.1 - scale / 6, scale / 1.6);
  }
}


void addsnow(int x, int y, int scale) {
  int dxo, dyo, dxi, dyi;
  for (int flakes = 0; flakes < 5; flakes++) {
    for (int i = 0; i < 360; i = i + 45) {
      dxo = 0.5 * scale * cos((i - 90) * 3.14 / 180); dxi = dxo * 0.1;
      dyo = 0.5 * scale * sin((i - 90) * 3.14 / 180); dyi = dyo * 0.1;
      display.drawLine(dxo + x + flakes * 1.5 * scale - scale * 3, dyo + y + scale * 2, dxi + x + 0 + flakes * 1.5 * scale - scale * 3, dyi + y + scale * 2, GxEPD_BLACK);
    }
  }
}


void addtstorm(int x, int y, int scale) {
  y = y + scale / 2;
  for (int i = 0; i < 5; i++) {
    display.drawLine(x - scale * 4 + scale * i * 1.5 + 0, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 0, y + scale, GxEPD_BLACK);
    if (scale != small) {
      display.drawLine(x - scale * 4 + scale * i * 1.5 + 1, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 1, y + scale, GxEPD_BLACK);
      display.drawLine(x - scale * 4 + scale * i * 1.5 + 2, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 2, y + scale, GxEPD_BLACK);
    }
    display.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 0, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 0, GxEPD_BLACK);
    if (scale != small) {
      display.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 1, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 1, GxEPD_BLACK);
      display.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 2, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 2, GxEPD_BLACK);
    }
    display.drawLine(x - scale * 3.5 + scale * i * 1.4 + 0, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5, GxEPD_BLACK);
    if (scale != small) {
      display.drawLine(x - scale * 3.5 + scale * i * 1.4 + 1, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 1, y + scale * 1.5, GxEPD_BLACK);
      display.drawLine(x - scale * 3.5 + scale * i * 1.4 + 2, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 2, y + scale * 1.5, GxEPD_BLACK);
    }
  }
}


void addsun(int x, int y, int scale, bool iconSize) {
  int linesize = 3;
  if (iconSize == smallIcon) linesize = 1;
  display.fillRect(x - scale * 2, y, scale * 4, linesize, GxEPD_BLACK);
  display.fillRect(x, y - scale * 2, linesize, scale * 4, GxEPD_BLACK);
  display.drawLine(x - scale * 1.3, y - scale * 1.3, x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
  display.drawLine(x - scale * 1.3, y + scale * 1.3, x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
  if (iconSize == largeIcon) {
    display.drawLine(1 + x - scale * 1.3, y - scale * 1.3, 1 + x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
    display.drawLine(2 + x - scale * 1.3, y - scale * 1.3, 2 + x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
    display.drawLine(3 + x - scale * 1.3, y - scale * 1.3, 3 + x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
    display.drawLine(1 + x - scale * 1.3, y + scale * 1.3, 1 + x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
    display.drawLine(2 + x - scale * 1.3, y + scale * 1.3, 2 + x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
    display.drawLine(3 + x - scale * 1.3, y + scale * 1.3, 3 + x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
  }
  display.fillCircle(x, y, scale * 1.3, GxEPD_WHITE);
  display.fillCircle(x, y, scale, GxEPD_BLACK);
  display.fillCircle(x, y, scale - linesize, GxEPD_WHITE);
}


void addfog(int x, int y, int scale, int linesize, bool iconSize) {
  if (iconSize == smallIcon) {
    y -= 10;
    linesize = 1;
  }
  for (int i = 0; i < 6; i++) {
    display.fillRect(x - scale * 3, y + scale * 1.5, scale * 6, linesize, GxEPD_BLACK);
    display.fillRect(x - scale * 3, y + scale * 2.0, scale * 6, linesize, GxEPD_BLACK);
    display.fillRect(x - scale * 3, y + scale * 2.5, scale * 6, linesize, GxEPD_BLACK);
  }
}


void addmoon(int x, int y, int scale, bool iconSize) {
  if (iconSize == largeIcon) {
    x = x + 12; y = y + 12;
    display.fillCircle(x - 50, y - 55, scale, GxEPD_BLACK);
    display.fillCircle(x - 35, y - 55, scale * 1.6, GxEPD_WHITE);
  }
  else
  {
    display.fillCircle(x - 20, y - 12, scale, GxEPD_BLACK);
    display.fillCircle(x - 15, y - 12, scale * 1.6, GxEPD_WHITE);
  }
}


void sunny(int x, int y, bool iconSize, String iconName) {
  int scale = small, offset = 3;
  if (iconSize == largeIcon) {
    scale = large;
    y = y - 8;
    offset = 18;
  } else y = y - 3; // Shift up small sun icon
  if (iconName.endsWith("n")) addmoon(x, y + offset, scale, iconSize);
  scale = scale * 1.6;
  addsun(x, y, scale, iconSize);
}


void mostlySunny(int x, int y, bool iconSize, String iconName) {
  int scale = small, linesize = 3, offset = 3;
  if (iconSize == largeIcon) {
    scale = large;
    offset = 10;
  } else linesize = 1;
  if (iconName.endsWith("n")) addmoon(x, y + offset, scale, iconSize);
  addcloud(x, y + offset, scale, linesize);
  addsun(x - scale * 1.8, y - scale * 1.8 + offset, scale, iconSize);
}


void mostlyCloudy(int x, int y, bool iconSize, String iconName) {
  int scale = small, linesize = 3;
  if (iconSize == largeIcon) {
    scale = large;
    linesize = 1;
  }
  if (iconName.endsWith("n")) addmoon(x, y, scale, iconSize);
  addcloud(x, y, scale, linesize);
  addsun(x - scale * 1.8, y - scale * 1.8, scale, iconSize);
  addcloud(x, y, scale, linesize);
}


void cloudy(int x, int y, bool iconSize, String iconName) {
  int scale = large, linesize = 3;
  if (iconSize == smallIcon) {
    scale = small;
    if (iconName.endsWith("n")) addmoon(x, y, scale, iconSize);
    linesize = 1;
    addcloud(x, y, scale, linesize);
  }
  else {
    y += 10;
    if (iconName.endsWith("n")) addmoon(x, y, scale, iconSize);
    addcloud(x + 30, y - 35, 5, linesize); // Cloud top right
    addcloud(x - 20, y - 25, 7, linesize); // Cloud top left
    addcloud(x, y, scale, linesize);       // Main cloud
  }
}


void rain(int x, int y, bool iconSize, String iconName) {
  int scale = large, linesize = 3;
  if (iconSize == smallIcon) {
    scale = small;
    linesize = 1;
  }
  if (iconName.endsWith("n")) addmoon(x, y + 10, scale, iconSize);
  addcloud(x, y, scale, linesize);
  addrain(x, y, scale, iconSize);
}


void expectRain(int x, int y, bool iconSize, String iconName) {
  int scale = large, linesize = 3;
  if (iconSize == smallIcon) {
    scale = small;
    linesize = 1;
  }
  if (iconName.endsWith("n")) addmoon(x, y, scale, iconSize);
  addsun(x - scale * 1.8, y - scale * 1.8, scale, iconSize);
  addcloud(x, y, scale, linesize);
  addrain(x, y, scale, iconSize);
}


void chanceRain(int x, int y, bool iconSize, String iconName) {
  int scale = large, linesize = 3;
  if (iconSize == smallIcon) {
    scale = small;
    linesize = 1;
  }
  if (iconName.endsWith("n")) addmoon(x, y, scale, iconSize);
  addsun(x - scale * 1.8, y - scale * 1.8, scale, iconSize);
  addcloud(x, y, scale, linesize);
  addrain(x, y, scale, iconSize);
}


void tstorms(int x, int y, bool iconSize, String iconName) {
  int scale = large, linesize = 3;
  if (iconSize == smallIcon) {
    scale = small;
    linesize = 1;
  }
  if (iconName.endsWith("n")) addmoon(x, y, scale, iconSize);
  addcloud(x, y, scale, linesize);
  addtstorm(x, y, scale);
}


void snow(int x, int y, bool iconSize, String iconName) {
  int scale = large, linesize = 3;
  if (iconSize == smallIcon) {
    scale = small;
    linesize = 1;
  }
  if (iconName.endsWith("n")) addmoon(x, y + 15, scale, iconSize);
  addcloud(x, y, scale, linesize);
  addsnow(x, y, scale);
}


void fog(int x, int y, bool iconSize, String iconName) {
  int linesize = 3, scale = large;
  if (iconSize == smallIcon) {
    scale = small;
    linesize = 1;
  }
  if (iconName.endsWith("n")) addmoon(x, y, scale, iconSize);
  addcloud(x, y - 5, scale, linesize);
  addfog(x, y - 5, scale, linesize, iconSize);
}


void haze(int x, int y, bool iconSize, String iconName) {
  int linesize = 3, scale = large;
  if (iconSize == smallIcon) {
    scale = small;
    linesize = 1;
  }
  if (iconName.endsWith("n")) addmoon(x, y, scale, iconSize);
  addsun(x, y - 5, scale * 1.4, iconSize);
  addfog(x, y - 5, scale * 1.4, linesize, iconSize);
}


void cloudCover(int x, int y, int cCover) {
  addcloud(x - 9, y - 3, small * 0.5, 2); // Cloud top left
  addcloud(x + 3, y - 3, small * 0.5, 2); // Cloud top right
  addcloud(x, y,         small * 0.5, 2); // Main cloud
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  drawString(x + 15, y - 5, String(String(cCover) + "%").c_str(), TOP_LEFT);
}


void visibility(int x, int y, String visi) {
  y = y - 3; //
  float start_angle = 0.52, end_angle = 2.61;
  int r = 10;
  for (float i = start_angle; i < end_angle; i = i + 0.05) {
    display.drawPixel(x + r * cos(i), y - r / 2 + r * sin(i), GxEPD_BLACK);
    display.drawPixel(x + r * cos(i), 1 + y - r / 2 + r * sin(i), GxEPD_BLACK);
  }
  start_angle = 3.61; end_angle = 5.78;
  for (float i = start_angle; i < end_angle; i = i + 0.05) {
    display.drawPixel(x + r * cos(i), y + r / 2 + r * sin(i), GxEPD_BLACK);
    display.drawPixel(x + r * cos(i), 1 + y + r / 2 + r * sin(i), GxEPD_BLACK);
  }
  display.fillCircle(x, y, r / 4, GxEPD_BLACK);
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  drawString(x + 12, y - 3, visi.c_str(), TOP_LEFT);
}


void nodata(int x, int y, bool iconSize, String iconName) {
  if (iconSize == largeIcon) u8g2Fonts.setFont(u8g2_font_helvB24_tf); else u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  drawString(x - 3, y - 8, "?", CENTER);
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
}


void displayWXicon(int x, int y, String iconName, bool iconSize) {
  Serial.println(iconName);
  if      (iconName == "01d" || iconName == "01n")  sunny(x, y, iconSize, iconName);
  else if (iconName == "02d" || iconName == "02n")  mostlySunny(x, y, iconSize, iconName);
  else if (iconName == "03d" || iconName == "03n")  cloudy(x, y, iconSize, iconName);
  else if (iconName == "04d" || iconName == "04n")  mostlySunny(x, y, iconSize, iconName);
  else if (iconName == "09d" || iconName == "09n")  chanceRain(x, y, iconSize, iconName);
  else if (iconName == "10d" || iconName == "10n")  rain(x, y, iconSize, iconName);
  else if (iconName == "11d" || iconName == "11n")  tstorms(x, y, iconSize, iconName);
  else if (iconName == "13d" || iconName == "13n")  snow(x, y, iconSize, iconName);
  else if (iconName == "50d")                       haze(x, y, iconSize, iconName);
  else if (iconName == "50n")                       fog(x, y, iconSize, iconName);
  else                                              nodata(x, y, iconSize, iconName);
}


void drawBattery(int x, int y) {
  uint8_t percentage = 100;
  float voltage = analogRead(35) / 4096.0 * 7.46;
  Serial.println("Voltage = " + String(voltage));
  if (voltage > 1 ) { // Only display if there is a valid reading
    percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
    if (voltage >= 4.20) percentage = 100;
    if (voltage <= 3.50) percentage = 0;
    display.drawRect(x + 15, y - 12, 19, 10, GxEPD_BLACK);
    display.fillRect(x + 34, y - 10, 2, 5, GxEPD_BLACK);
    display.fillRect(x + 17, y - 10, 15 * percentage / 100.0, 6, GxEPD_BLACK);
    const char *percentageStr = String(String(percentage) + "%").c_str();
    drawString(x + 65, y - 11, percentageStr, TOP_RIGHT);
    //drawString(x + 13, y + 5,  String(voltage, 2) + "v", CENTER);
  }
}


void drawHeadingSection(const char *dateStr, const char *timeStr) {
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  const String owmCity = preferences.getString("OWM_CITY");
  drawString(display.width() >> 1, 0, owmCity.c_str(), TOP_RIGHT);
  drawString(display.width(), 0, dateStr, TOP_RIGHT);
  drawString(4, 0, timeStr, TOP_LEFT);
  drawBattery(65, 12);
  display.drawLine(0, 12, display.width(), 12, GxEPD_BLACK);
}


void drawMainWx(int x, int y) {
  u8g2Fonts.setFont(u8g2_font_helvB14_tf);
  drawString(x - 25, y - 24, String(String(wxConditions[0].temperature, 1) + "°" + (units == METRIC ? "C" : "F")).c_str(), TOP_CENTER); // Show current Temperature
  u8g2Fonts.setFont(u8g2_font_helvB12_tf);
  drawString(x - 15, y - 5, String(String(wxConditions[0].high, 0) + "° | " + String(wxConditions[0].low, 0) + "°").c_str(), TOP_CENTER); // Show forecast high and Low
  drawString(x + 30, y - 22, String(String(wxConditions[0].humidity, 0) + "%").c_str(), TOP_CENTER);
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  drawString(x + 32, y - 3, "RH", TOP_CENTER);
}


void drawMainWeatherSection(int x, int y) {
  displayDisplayWindSection(x - 115, y - 3, wxConditions[0].winddir, wxConditions[0].windspeed, 40);
  displayWXicon(x + 5, y - 5, wxConditions[0].icon, largeIcon);
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  drawPressureAndTrend(x - 120, y + 52, wxConditions[0].pressure, wxConditions[0].trend);
  u8g2Fonts.setFont(u8g2_font_helvB12_tf);
  String wxDescription = wxConditions[0].forecast0;
  if (wxConditions[0].forecast1 != "") wxDescription += " & " +  wxConditions[0].forecast1;
  if (wxConditions[0].forecast2 != "" && wxConditions[0].forecast1 != wxConditions[0].forecast2) wxDescription += " & " +  wxConditions[0].forecast2;
  char *description = titleCase(wxDescription.begin());
  Serial.println(description);
  drawStringMaxWidth(x - 170, y + 69, 28, description, TOP_LEFT);
  drawMainWx(x, y + 50);
  display.drawRect(0, y + 68, 232, 48, GxEPD_BLACK);
}


void drawForecastWeather(int x, int y, int index) {
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  display.drawRect(x, y, 55, 65, GxEPD_BLACK);
  display.drawLine(x + 1, y + 13, x + 54, y + 13, GxEPD_BLACK);
  displayWXicon(x + 28, y + 35, wxForecast[index].icon, smallIcon);
  drawString(x + 28, y + 1, String(convertUnixTime(wxForecast[index].dt + wxConditions[0].timezone).substring(0,5)).c_str(), TOP_CENTER);
  drawString(x + 28, y + 50, String(String(wxForecast[index].high, 0) + "° / " + String(wxForecast[index].low, 0) + "°").c_str(), TOP_CENTER);
}


void drawForecastSection(int x, int y) {
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  drawForecastWeather(x, y, 0);
  drawForecastWeather(x + 56, y, 1);
  drawForecastWeather(x + 112, y, 2);
  // (x,y,width,height,MinValue, MaxValue, Title, Data Array, AutoScale, ChartMode)
  for (uint8_t r = 0; r < max_readings; r++) {
    if (units == IMPERIAL) {
      pressure_readings[r] = wxForecast[r].pressure * 0.02953;
      rain_readings[r]     = wxForecast[r].rainfall * 0.0393701;
    } else {
      pressure_readings[r] = wxForecast[r].pressure;
      rain_readings[r]     = wxForecast[r].rainfall;
    }
    temperature_readings[r] = wxForecast[r].temperature;
  }
  display.drawLine(0, y + 172, display.width(), y + 172, GxEPD_BLACK);
  //u8g2Fonts.setFont(u8g2_font_helvB12_tf);
  drawString(display.width() >> 1, y + 176, TXT_FORECAST_VALUES, TOP_CENTER);
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  drawGraph(30,  215, display.width() >> 2, display.height() / 5, 900, 1050, units == METRIC ? TXT_PRESSURE_HPA : TXT_PRESSURE_IN, pressure_readings, max_readings, autoscale_on, barchart_off);
  drawGraph(158, 215, display.width() >> 2, display.height() / 5, 10, 30, units == METRIC ? TXT_TEMPERATURE_C : TXT_TEMPERATURE_F, temperature_readings, max_readings, autoscale_on, barchart_off);
  drawGraph(288, 215, display.width() >> 2, display.height() / 5, 0, 30, units == METRIC ? TXT_RAINFALL_MM : TXT_RAINFALL_IN, rain_readings, max_readings, autoscale_on, barchart_on);
}


void displayPrecipitationSection(int x, int y) {
  display.drawRect(x, y - 1, 167, 56, GxEPD_BLACK); // precipitation outline
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  //if (wxForecast[1].rainfall > 0.005) { // Ignore small amounts
    drawString(x + 5, y + 7, String(String(wxForecast[1].rainfall, 2) + (units == METRIC ? "mm" : "in")).c_str(), TOP_LEFT); // Only display rainfall total today if > 0
    addraindrop(x + 65 - (units == IMPERIAL ? 10 : 0), y + 16, 7);
  //}
  //if (wxForecast[1].snowfall > 0.005)  // Ignore small amounts
    drawString(x + 5, y + 27, String(String(wxForecast[1].snowfall, 2) + (units == METRIC ? "mm" : "in") + " * *").c_str(), TOP_LEFT); // Only display snowfall total today if > 0
}


void drawAstronomySection(int x, int y) {
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  display.drawRect(x, y + 64, 167, 48, GxEPD_BLACK);
  drawString(x + 3, y + 66, String(convertUnixTime(wxConditions[0].sunrise + wxConditions[0].timezone).substring(0, (units == METRIC ? 5 : 7)) + " " + TXT_SUNRISE).c_str(), TOP_LEFT);
  drawString(x + 3, y + 81, String(convertUnixTime(wxConditions[0].sunset + wxConditions[0].timezone).substring(0, (units == METRIC ? 5 : 7)) + " " + TXT_SUNSET).c_str(), TOP_LEFT);
  time_t now = time(NULL);
  struct tm *now_utc  = gmtime(&now);
  const int day_utc   = now_utc->tm_mday;
  const int month_utc = now_utc->tm_mon + 1;
  const int year_utc  = now_utc->tm_year + 1900;
  drawString(x + 3, y + 96, moonPhase(day_utc, month_utc, year_utc).c_str(), TOP_LEFT);
  drawMoon(x + 105, y + 50, day_utc, month_utc, year_utc);
}


void displayWeather(const char *dateStr, const char *timeStr) {
  drawHeadingSection(dateStr, timeStr); // Top line of the display
  drawMainWeatherSection(172, 70);      // Centre section of display for Location, temperature, Weather report, current Wx Symbol and wind direction
  drawForecastSection(233, 15);         // 3hr forecast boxes
  displayPrecipitationSection(233, 82); // Precipitation section
  if (wxConditions[0].visibility > 0) visibility(335, 100, String(wxConditions[0].visibility) + "M");
  if (wxConditions[0].cloudcover > 0) cloudCover(350, 125, wxConditions[0].cloudcover);
  drawAstronomySection(233, 74);        // Astronomy section Sun rise/set, Moon phase and Moon icon
}


#endif