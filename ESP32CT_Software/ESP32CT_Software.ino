#include <WiFi.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include <ArduinoOTA.h>

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ESPmDNS.h>

#include <ESP32Time.h>
#include <NTPClient.h>

#include <driver/gpio.h>
#include <driver/adc.h>
#include <driver/dac.h>

#include <WebSocketsServer.h>
#include <WiFiClient.h>
#include <WebServer.h>

/* Pinout - begin */
#define WM_TRIGGER_PIN 0

#define ADC1_CH0_pin 36
#define ADC1_CH3_pin 39
#define ADC1_CH6_pin 34

#define DAC1_OUT 25

//#define led 2
//#define LED 2  //On board LED

#define HC595_SER   14
#define HC595_RCLK  12
#define HC595_SRCLK 13

/* Pinout - end */

float f_x[65];
float f_y[65];
float f_z[65];

float f_x1[65]; // = {337, 360, 387, 414, 438, 465, 486, 508, 529, 548, 563, 577, 588, 598, 604, 606, 610, 607, 604, 599, 589, 577, 563, 548, 529, 508, 487, 465, 438, 414, 387, 360, 337, 310, 283, 255, 232, 205, 184, 164, 142, 124, 107, 94, 83, 73, 67, 65, 62, 65, 67, 73, 83, 94, 107, 124, 142, 164, 183, 205, 232, 255, 282, 308, 336};
float f_y1[65]; // = {338, 352, 369, 385, 399, 416, 430, 442, 456, 468, 478, 488, 496, 504, 508, 510, 512, 510, 508, 504, 496, 488, 478, 468, 456, 442, 430, 416, 400, 385, 369, 352, 338, 322, 306, 288, 274, 258, 245, 233, 219, 208, 198, 190, 183, 176, 174, 172, 170, 172, 174, 177, 183, 190, 198, 208, 219, 233, 245, 258, 274, 288, 306, 322, 338};

float f_x2[65]; // = {337, 360, 387, 414, 438, 465, 486, 508, 529, 548, 563, 577, 588, 598, 604, 606, 610, 607, 604, 599, 589, 577, 563, 548, 529, 508, 487, 465, 438, 414, 387, 360, 337, 310, 283, 255, 232, 205, 184, 164, 142, 124, 107, 94, 83, 73, 67, 65, 62, 65, 67, 73, 83, 94, 107, 124, 142, 164, 183, 205, 232, 255, 282, 308, 336};
float f_y2[65]; // = {338, 352, 369, 385, 399, 416, 430, 442, 456, 468, 478, 488, 496, 504, 508, 510, 512, 510, 508, 504, 496, 488, 478, 468, 456, 442, 430, 416, 400, 385, 369, 352, 338, 322, 306, 288, 274, 258, 245, 233, 219, 208, 198, 190, 183, 176, 174, 172, 170, 172, 174, 177, 183, 190, 198, 208, 219, 233, 245, 258, 274, 288, 306, 322, 338};

float diff_percent = 1.23;

unsigned long loop_millis1;
unsigned long loop_counter1 = 0;
byte loop_cn2 = 0;

int led_state = 0;
int led_cnt1 = 0;
int led_cnt2 = 0;

#define LOOP_PERIOD 35 // Display updates every 35 ms

unsigned long updateTime;
int value[6] = {0, 0, 0, 0, 0, 0};
int old_value[6] = { -1, -1, -1, -1, -1, -1};
int d = 0;

int ch_relay = 0;

int ch_relay_max = 0;

void setup() {
  pinMode(HC595_SER, OUTPUT);
  pinMode(HC595_RCLK, OUTPUT);
  pinMode(HC595_SRCLK, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(115200);

  wifiManager_setup();

  arduinoOTA_setup();

  server_setup();

  curveTracer_setup();
}

void loop() {
  // put your main code here, to run repeatedly:

  webSocket_handle();

  wifiManager_handle();

  ArduinoOTA.handle();

  //  if ((millis() - loop_millis1) > 1500) {
  //    loop_millis1 = millis();
  //
  //    if (loop_cn2 < 20) {
  //      loop_cn2++;
  //    } else {
  //      loop_cn2 = 0;
  //    }
  //
  //    set_hc595(0, loop_cn2);
  //  }

}
