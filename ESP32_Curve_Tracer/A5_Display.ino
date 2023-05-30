
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define TFT_GREY 0x5AEB

float ltx = 0;    // Saved x coord of bottom of needle
uint16_t osx = 120, osy = 120; // Saved x & y coords
//uint32_t updateTime = 0;       // time for next update

int old_analog =  -999; // Value last displayed
int old_digital = -999; // Value last displayed

//int value[6] = {0, 0, 0, 0, 0, 0};
//int old_value[6] = { -1, -1, -1, -1, -1, -1};
//int d = 0;

void display_setup(void) {
  pinMode(LCD_LED, OUTPUT);

  digitalWrite(LCD_LED, HIGH);

  tft.init();
  tft.setRotation(1);
  //Serial.begin(57600); // For debug
  tft.fillScreen(TFT_BLACK);

//  analogMeter(); // Draw analogue meter
//
//  // Draw 6 linear meters
//  byte d = 40;
//  plotLinear("A0", 0, 160);
//  plotLinear("A1", 1 * d, 160);
//  plotLinear("A2", 2 * d, 160);
//  plotLinear("A3", 3 * d, 160);
//  plotLinear("A4", 4 * d, 160);
//  plotLinear("A5", 5 * d, 160);

  //  screen_saver_Matrix_init();
}

//void drawXY(void) {
//  float calc_diff = 0.0;
//  bool flag_save = flag_capture_data;
//  int p2_x[65];
//  int p2_y[65];
//  int p2_x2[65];
//  int p2_y2[65];
//
//  for (i = 0; i < 65; i++) {
//    if (flag_save == true) {
//      hold_data_x[i] = remote_data_x[i] * 1000;
//      hold_data_y[i] = remote_data_y[i] * 1000;
//    }
//
//    point1_x1[i] = ((remote_data_x[i] - remote_data_y[i]) * tft.width) + (tft.width / 2);
//    point1_y1[i] = remote_data_y[i] * tft.height;
//
//    point1_x2[i] = ((remote_data_x[i] - remote_data_z[i]) * tft.width) + (tft.width / 2);
//    point1_y2[i] = remote_data_z[i] * tft.height;
//
//    if (ch1_vs_ch2 == true) {
//      if (1 > (remote_data_y[i] / remote_data_z[i])) {
//        calc_diff += 1 - (remote_data_y[i] / remote_data_z[i]);
//      } else {
//        calc_diff += 1 - (remote_data_z[i] / remote_data_y[i]);
//      }
//    } else {
//      p2_x[i] = point2_x2[current_pin1 - 1][i] / 1000;
//      p2_y[i] = point2_y2[current_pin1 - 1][i] / 1000;
//
//      p2_x2[i] = ((p2_x[i] - p2_y[i]) * tft.width) + (tft.width / 2);
//      p2_y2[i] = p2_y[i] * tft.height;
//
//      if (1 > (remote_data_y[i] / p2_y[i])) {
//        calc_diff += (1 - (remote_data_y[i] / p2_y[i])) * 2;
//      } else {
//        calc_diff += (1 - (p2_y[i] / remote_data_y[i])) * 2;
//      }
//    }
//  }
//
//  if (flag_save == true) {
//    flag_save = false;
//
//    flag_capture_data = false;
//  }
//
//  diff_percent = calc_diff * 3.966; // 3.956: to fit in the 0-100 range
//  diff_percent = ((diff_percent + diff_percent_prev) / 2).toFixed(1);
//
//  //if (diff_percent > 99.9) {
//  //    diff_percent = 100;
//  //}
//
//  if (ch1_vs_ch2 == true) {
//    document.getElementById("diff").textContent = " (Diff: " + diff_percent + "%)";
//  } else if (show_file_pin == true) {
//    document.getElementById("diff").textContent = " (Diff: " + diff_percent + "%)";
//  }
//
//  var ctx = canvas1.getContext("2d");
//
//  ctx.clearRect(0, 0, tft.width, tft.height);
//  ctx.stroke();
//
//  var ch1color = document.getElementById("ch1color").value;
//  var ch2color = document.getElementById("ch2color").value;
//
//  for (i = 1; i < 6; i++) {
//    ctx.beginPath();
//    ctx.moveTo((tft.width / 6) * i, 0);
//    ctx.lineTo((tft.width / 6) * i, tft.height);
//    ctx.lineWidth = 1;
//    ctx.strokeStyle = "#4e4e4e";
//    ctx.stroke();
//
//    ctx.beginPath();
//    ctx.moveTo(0, (tft.height / 6) * i);
//    ctx.lineTo(tft.width, (tft.height / 6) * i);
//    ctx.lineWidth = 1;
//    ctx.strokeStyle = "#4e4e4e";
//    ctx.stroke();
//  }
//
//  for (i = 0; i < 63; i++) {
//    // trace 1:
//    ctx.beginPath();
//    ctx.moveTo(point1_x1[i], point1_y1[i]);
//    ctx.lineTo(point1_x1[i + 1], point1_y1[i + 1]);
//    ctx.lineWidth = 3;
//    ctx.strokeStyle = ch1color;
//    ctx.stroke();
//
//    if (ch1_vs_ch2 == true) {
//      // trace 2:
//      ctx.beginPath();
//      ctx.moveTo(point1_x2[i], point1_y2[i]);
//      ctx.lineTo(point1_x2[i + 1], point1_y2[i + 1]);
//      ctx.lineWidth = 3;
//      ctx.strokeStyle = ch2color;
//      ctx.stroke();
//    } else if (show_file_pin == true) {
//      // trace 2:
//      ctx.beginPath();
//      ctx.moveTo(p2_x2[i], p2_y2[i]);
//      ctx.lineTo(p2_x2[i + 1], p2_y2[i + 1]);
//      ctx.lineWidth = 3;
//      ctx.strokeStyle = ch2color;
//      ctx.stroke();
//    }
//  }
//}

// #########################################################################
//  Draw the analogue meter on the screen
// #########################################################################
void analogMeter()
{
  // Meter outline
  tft.fillRect(0, 0, 239, 126, TFT_GREY);
  tft.fillRect(5, 3, 230, 119, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);  // Text colour

  // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  for (int i = -50; i < 51; i += 5) {
    // Long scale tick length
    int tl = 15;

    // Coodinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (100 + tl) + 120;
    uint16_t y0 = sy * (100 + tl) + 140;
    uint16_t x1 = sx * 100 + 120;
    uint16_t y1 = sy * 100 + 140;

    // Coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * 0.0174532925);
    float sy2 = sin((i + 5 - 90) * 0.0174532925);
    int x2 = sx2 * (100 + tl) + 120;
    int y2 = sy2 * (100 + tl) + 140;
    int x3 = sx2 * 100 + 120;
    int y3 = sy2 * 100 + 140;

    // Red zone limits
    if (i >= -50 && i < -25) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_RED);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_RED);
    }

    // Green zone limits
    if (i >= -25 && i < 0) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_GREEN);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_GREEN);
    }

    // Yellow zone limits
    if (i >= 0 && i < 25) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_YELLOW);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_YELLOW);
    }

    // Orange zone limits
    if (i >= 25 && i < 50) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_ORANGE);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_ORANGE);
    }

    // Short scale tick length
    if (i % 25 != 0) tl = 8;

    // Recalculate coords incase tick lenght changed
    x0 = sx * (100 + tl) + 120;
    y0 = sy * (100 + tl) + 140;
    x1 = sx * 100 + 120;
    y1 = sy * 100 + 140;

    // Draw tick
    tft.drawLine(x0, y0, x1, y1, TFT_BLACK);

    // Check if labels should be drawn, with position tweaks
    if (i % 25 == 0) {
      // Calculate label positions
      x0 = sx * (100 + tl + 10) + 120;
      y0 = sy * (100 + tl + 10) + 140;
      switch (i / 25) {
        case -2: tft.drawCentreString("0", x0, y0 - 12, 2); break;
        case -1: tft.drawCentreString("25", x0, y0 - 9, 2); break;
        case 0: tft.drawCentreString("50", x0, y0 - 6, 2); break;
        case 1: tft.drawCentreString("75", x0, y0 - 9, 2); break;
        case 2: tft.drawCentreString("100", x0, y0 - 12, 2); break;
      }
    }

    // Now draw the arc of the scale
    sx = cos((i + 5 - 90) * 0.0174532925);
    sy = sin((i + 5 - 90) * 0.0174532925);
    x0 = sx * 100 + 120;
    y0 = sy * 100 + 140;
    // Draw scale arc, don't draw the last part
    if (i < 50) tft.drawLine(x0, y0, x1, y1, TFT_BLACK);
  }

  tft.drawString("%RH", 5 + 230 - 40, 119 - 20, 2); // Units at bottom right
  tft.drawCentreString("%RH", 120, 70, 4); // Comment out to avoid font 4
  tft.drawRect(5, 3, 230, 119, TFT_BLACK); // Draw bezel line

  plotNeedle(0, 0); // Put meter needle at 0
}

// #########################################################################
// Update needle position
// This function is blocking while needle moves, time depends on ms_delay
// 10ms minimises needle flicker if text is drawn within needle sweep area
// Smaller values OK if text not in sweep area, zero for instant movement but
// does not look realistic... (note: 100 increments for full scale deflection)
// #########################################################################
void plotNeedle(int value, byte ms_delay)
{
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  char buf[8]; dtostrf(value, 4, 0, buf);
  tft.drawRightString(buf, 40, 119 - 20, 2);

  if (value < -10) value = -10; // Limit value to emulate needle end stops
  if (value > 110) value = 110;

  // Move the needle util new value reached
  while (!(value == old_analog)) {
    if (old_analog < value) old_analog++;
    else old_analog--;

    if (ms_delay == 0) old_analog = value; // Update immediately id delay is 0

    float sdeg = map(old_analog, -10, 110, -150, -30); // Map value to angle
    // Calcualte tip of needle coords
    float sx = cos(sdeg * 0.0174532925);
    float sy = sin(sdeg * 0.0174532925);

    // Calculate x delta of needle start (does not start at pivot point)
    float tx = tan((sdeg + 90) * 0.0174532925);

    // Erase old needle image
    tft.drawLine(120 + 20 * ltx - 1, 140 - 20, osx - 1, osy, TFT_WHITE);
    tft.drawLine(120 + 20 * ltx, 140 - 20, osx, osy, TFT_WHITE);
    tft.drawLine(120 + 20 * ltx + 1, 140 - 20, osx + 1, osy, TFT_WHITE);

    // Re-plot text under needle
    tft.setTextColor(TFT_BLACK);
    tft.drawCentreString("%RH", 120, 70, 4); // // Comment out to avoid font 4

    // Store new needle end coords for next erase
    ltx = tx;
    osx = sx * 98 + 120;
    osy = sy * 98 + 140;

    // Draw the needle in the new postion, magenta makes needle a bit bolder
    // draws 3 lines to thicken needle
    tft.drawLine(120 + 20 * ltx - 1, 140 - 20, osx - 1, osy, TFT_RED);
    tft.drawLine(120 + 20 * ltx, 140 - 20, osx, osy, TFT_MAGENTA);
    tft.drawLine(120 + 20 * ltx + 1, 140 - 20, osx + 1, osy, TFT_RED);

    // Slow needle down slightly as it approaches new postion
    if (abs(old_analog - value) < 10) ms_delay += ms_delay / 5;

    // Wait before next update
    delay(ms_delay);
  }
}


// #########################################################################
//  Draw a linear meter on the screen
// #########################################################################
void plotLinear(char *label, int x, int y)
{
  int w = 36;
  tft.drawRect(x, y, w, 155, TFT_GREY);
  tft.fillRect(x + 2, y + 19, w - 3, 155 - 38, TFT_WHITE);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawCentreString(label, x + w / 2, y + 2, 2);

  for (int i = 0; i < 110; i += 10)
  {
    tft.drawFastHLine(x + 20, y + 27 + i, 6, TFT_BLACK);
  }

  for (int i = 0; i < 110; i += 50)
  {
    tft.drawFastHLine(x + 20, y + 27 + i, 9, TFT_BLACK);
  }

  tft.fillTriangle(x + 3, y + 127, x + 3 + 16, y + 127, x + 3, y + 127 - 5, TFT_RED);
  tft.fillTriangle(x + 3, y + 127, x + 3 + 16, y + 127, x + 3, y + 127 + 5, TFT_RED);

  tft.drawCentreString("---", x + w / 2, y + 155 - 18, 2);
}


// #########################################################################
//  Adjust 6 linear meter pointer positions
// #########################################################################
void plotPointer(void)
{
  int dy = 187;
  byte pw = 16;

  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  // Move the 6 pointers one pixel towards new value
  for (int i = 0; i < 6; i++)
  {
    char buf[8]; dtostrf(value[i], 4, 0, buf);
    tft.drawRightString(buf, i * 40 + 36 - 5, 187 - 27 + 155 - 18, 2);

    int dx = 3 + 40 * i;
    if (value[i] < 0) value[i] = 0; // Limit value to emulate needle end stops
    if (value[i] > 100) value[i] = 100;

    while (!(value[i] == old_value[i])) {
      dy = 187 + 100 - old_value[i];
      if (old_value[i] > value[i])
      {
        tft.drawLine(dx, dy - 5, dx + pw, dy, TFT_WHITE);
        old_value[i]--;
        tft.drawLine(dx, dy + 6, dx + pw, dy + 1, TFT_RED);
      }
      else
      {
        tft.drawLine(dx, dy + 5, dx + pw, dy, TFT_WHITE);
        old_value[i]++;
        tft.drawLine(dx, dy - 6, dx + pw, dy - 1, TFT_RED);
      }
    }
  }
}
