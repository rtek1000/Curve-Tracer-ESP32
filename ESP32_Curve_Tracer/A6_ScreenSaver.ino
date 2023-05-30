// A fun MATRIX-like screen demo of scrolling (Alternative)
// Screen will flicker initially until fully drawn
// then scroll smoothly

// Needs GLCD font

// by RTEK1000 - May/12/2023

/*
  Make sure all the display driver and pin connections are correct by
  editing the User_Setup.h file in the TFT_eSPI library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/

//#include <TFT_eSPI.h> // Hardware-specific library
//#include <SPI.h>

///TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

/* Select only one */
#define TFT_LANDSCAPE
//#define TFT_PORTRAIT

/* Select only one */
//#define TFT_INVERTED
#define TFT_NORMAL

#define TEXT_HEIGHT 8     // Height of text to be printed and scrolled
#define TEXT_WIDHT 6      // Widht of text to be printed and scrolled

#define LINE_HEIGHT 9     // TEXT_HEIGHT + 1
#define COL_WIDTH 8       // TEXT_WIDHT + 2

// 240 x 320 Landscape - Begin
// tft.height: 240; tft.width: 320 - Landscape
#ifdef TFT_LANDSCAPE
#define MAX_CHR 30        // characters per line (tft.height() / LINE_HEIGHT);
#define MAX_COL 40        // maximum number of columns (tft.width() / COL_WIDTH);
#define MAX_COL_DOT6 24   // MAX_COL * 0.6

#ifdef TFT_NORMAL
#define TFT_ROTATE 1
#else // #ifdef TFT_NORMAL
#define TFT_ROTATE 3
#endif // #ifdef TFT_NORMAL

#endif // #ifdef TFT_LANDSCAPE
// 240 x 320 Landscape - End

// 320 x 240 Portrait - Begin
#ifndef TFT_LANDSCAPE // #ifdef TFT_PORTRAIT
#define MAX_CHR 35        // characters per line (tft.height() / LINE_HEIGHT);
#define MAX_COL 30        // maximum number of columns (TFT_WIDTH / COL_WIDTH);
#define MAX_COL_DOT6 18   // MAX_COL * 0.6

#ifdef TFT_NORMAL
#define TFT_ROTATE 0
#else // #ifdef TFT_NORMAL
#define TFT_ROTATE 2
#endif // #ifdef TFT_NORMAL

#endif // #ifndef TFT_LANDSCAPE
// 320 x 240 Portrait - End

/*
  // 160 x 128 Landscape - Begin
  // tft.height: 160; tft.width: 128 - Landscape
  #ifdef TFT_LANDSCAPE
  #define MAX_CHR 14        // characters per line (tft.height() / LINE_HEIGHT);
  #define MAX_COL 20        // maximum number of columns (tft.width() / COL_WIDTH);
  #define MAX_COL_DOT6 12    // MAX_COL * 0.6

  #ifdef TFT_NORMAL
  #define TFT_ROTATE 1
  #else // #ifdef TFT_NORMAL
  #define TFT_ROTATE 3
  #endif // #ifdef TFT_NORMAL

  #endif // #ifdef TFT_LANDSCAPE
  // 160 x 128 Landscape - End

  // 128 x 160 Portrait - Begin
  #ifndef TFT_LANDSCAPE // #ifdef TFT_PORTRAIT
  #define MAX_CHR 17        // characters per line (tft.height() / LINE_HEIGHT);
  #define MAX_COL 16        // maximum number of columns (tft.width() / COL_WIDTH);
  #define MAX_COL_DOT6 9    // MAX_COL * 0.6

  #ifdef TFT_NORMAL
  #define TFT_ROTATE 0
  #else // #ifdef TFT_NORMAL
  #define TFT_ROTATE 2
  #endif // #ifdef TFT_NORMAL

  #endif // #ifndef TFT_LANDSCAPE
  // 128 x 160 Portrait - End
*/

int col_pos[MAX_COL];

int chr_map[MAX_COL][MAX_CHR];
byte color_map[MAX_COL][MAX_CHR];

uint16_t yPos = 0;

int screen_saver_for1 = MAX_CHR;
int screen_saver_for2 = 0;
int rnd_x;
int rnd_col_pos;
int color;

void screen_saver_Matrix_init(void) {
  for (int j = 0; j < MAX_COL; j++) {
    for (int i = 0; i < MAX_CHR; i++) {
      chr_map[j][i] = 0;
      color_map[j][i] = 0;
    }

    color_map[j][0] = 63;
  }
}

void screen_saver_Matrix(void) {
  if (screen_saver_for1 < MAX_CHR) {
    tft.setTextColor(color_map[rnd_col_pos][screen_saver_for1] << 5, ILI9341_BLACK); // Set the green character brightness

    if (color_map[rnd_col_pos][screen_saver_for1] == 63) {
      tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK); // Draw white character
    }

    if ((chr_map[rnd_col_pos][screen_saver_for1] == 0) || (color_map[rnd_col_pos][screen_saver_for1] == 63)) {
      chr_map[rnd_col_pos][screen_saver_for1] = random(31, 128);

      if (screen_saver_for1 > 1) {
        chr_map[rnd_col_pos][screen_saver_for1 - 1] = chr_map[rnd_col_pos][screen_saver_for1];
        chr_map[rnd_col_pos][screen_saver_for1 - 2] = chr_map[rnd_col_pos][screen_saver_for1];
      }
    }

    tft.drawChar(chr_map[rnd_col_pos][screen_saver_for1], rnd_x, yPos, 1); // Draw the character

    yPos += LINE_HEIGHT;

    screen_saver_for1++;
  } else {
    if (screen_saver_for2 < MAX_COL) {
      rnd_col_pos = random(1, MAX_COL);

      rnd_x = rnd_col_pos * COL_WIDTH;

      col_pos[rnd_col_pos - 1] = rnd_x; // save position

      for (int n = 0; n < MAX_CHR; n++) {
        chr_map[rnd_col_pos][n] = chr_map[rnd_col_pos][n + 1];
      }

      for (int n = MAX_CHR; n > 0; n--) {
        color_map[rnd_col_pos][n] = color_map[rnd_col_pos][n - 1];
      }

      chr_map[rnd_col_pos][0] = 0;

      if (color_map[rnd_col_pos][0] > 20) {
        color_map[rnd_col_pos][0] -= 3; // Rapid fade initially brightness values
      }

      if (color_map[rnd_col_pos][0] > 0) {
        color_map[rnd_col_pos][0] -= 1; // Slow fade later
      }

      if ((random(20) == 1) && (screen_saver_for2 < MAX_COL_DOT6)) { // MAX_COL * 0.6
        color_map[rnd_col_pos][0] = 63; // ~1 in 20 probability of a new character
      }

      screen_saver_for2++;
    } else {
      screen_saver_for2 = 0;
    }

    screen_saver_for1 = 0;

    yPos = 0;
  }
}
