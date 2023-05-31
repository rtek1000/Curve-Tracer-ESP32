#define buff_ch 2

volatile int buff_index = 0;
volatile int buff_status[buff_ch] = {0};

volatile int i = 0;
volatile int adc0[65][buff_ch] = {{0}, {0}};
volatile int adc1[65][buff_ch] = {{0}, {0}};
volatile int adc2[65][buff_ch] = {{0}, {0}};

volatile int f_adc0_tmp = 0.0;
volatile int f_adc1_tmp = 0.0;
volatile int f_adc2_tmp = 0.0;

volatile int val_tmp = 0;

// https://www.daycounter.com/Calculators/Sine-Generator-Calculator.phtml
// Number of points: 64
// Max Amplitude: 203

volatile const int wave[] = {
  0x65, 0x6f, 0x79, 0x82, 0x8c, 0x95, 0x9d, 0xa5,
  0xac, 0xb3, 0xb9, 0xbe, 0xc2, 0xc6, 0xc8, 0xca,
  0xca, 0xca, 0xc8, 0xc6, 0xc2, 0xbe, 0xb9, 0xb3,
  0xac, 0xa5, 0x9d, 0x95, 0x8c, 0x82, 0x79, 0x6f,
  0x65, 0x5b, 0x51, 0x48, 0x3e, 0x35, 0x2d, 0x25,
  0x1e, 0x17, 0x11, 0xc, 0x8, 0x4, 0x2, 0x0,
  0x0, 0x0, 0x2, 0x4, 0x8, 0xc, 0x11, 0x17,
  0x1e, 0x25, 0x2d, 0x35, 0x3e, 0x48, 0x51, 0x5b, 0x65
};

volatile const byte offset = 31; // wave center

volatile int adc0_tmp = 0;
volatile int adc1_tmp = 0;
volatile int adc2_tmp = 0;

byte val_100 = 0;
byte val_10 = 0;
byte val_1 = 0;

#define adc_res 0.000976563 // 1/1024

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//volatile unsi/gned int counter1 = 0;

void curveTracer_setup(void) {
  //  pinMode(led, OUTPUT);

  pinMode(HC595_SER, OUTPUT);
  pinMode(HC595_RCLK, OUTPUT);
  pinMode(HC595_SRCLK, OUTPUT);

  dac_setup();

  // https://docs.espressif.com/projects/esp-idf/en/v4.4.3/esp32/api-reference/peripherals/adc.html
  adc1_config_width(ADC_WIDTH_BIT_10);

  timer_setup();

  buff_status[0] = 1;
  buff_status[1] = 1;

  set_hc595(0, 0);
}

void timer_setup(void) {
  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  timer = timerBegin(0, 80, true);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer, true);

  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timer, 188, true);

  timerAlarmEnable(timer);
}

void dac_setup(void) {
  // https://deepbluembedded.com/esp32-dac-audio-arduino-examples/
  // Enable DAC1 Channel's Output
  dac_output_enable(DAC_CHANNEL_1);
  //  dac_output_enable(DAC_CHANNEL_2);

  //  dac_output_voltage(DAC_CHANNEL_2, 127);
}

void ARDUINO_ISR_ATTR onTimer() {
  //portENTER_CRITICAL_ISR(&timerMux);
  //digitalWrite(led, !digitalRead(led));
  //portEXIT_CRITICAL_ISR(&timerMux);

  //  count/er1++;

  int val_tmp = 129; // mid_point

  if (i < 65) {
    portENTER_CRITICAL_ISR(&timerMux);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
    f_adc0_tmp = adc1_get_raw(ADC1_CHANNEL_0); // analogRead(ADC1_CH0_pin);

    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);
    f_adc1_tmp = adc1_get_raw(ADC1_CHANNEL_3); // analogRead(ADC1_CH3_pin);

    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    f_adc2_tmp = adc1_get_raw(ADC1_CHANNEL_6); // analogRead(ADC1_CH6_pin);

    portEXIT_CRITICAL_ISR(&timerMux);

    if (buff_status[0] == 2) {
      adc0[i][0] = f_adc0_tmp + adc0[i][0];
      adc0[i][0] /= 2.0;

      adc1[i][0] = f_adc1_tmp + adc1[i][0];
      adc1[i][0] /= 2.0;

      adc2[i][0] = f_adc2_tmp + adc2[i][0];
      adc2[i][0] /= 2.0;
    } else if (buff_status[1] == 2) {
      adc0[i][1] = f_adc0_tmp + adc0[i][1];
      adc0[i][1] /= 2.0;

      adc1[i][1] = f_adc1_tmp + adc1[i][1];
      adc1[i][1] /= 2.0;

      adc2[i][1] = f_adc2_tmp + adc2[i][1];
      adc2[i][1] /= 2.0;
    }
  } else {
    if (buff_status[1] == 2) {
      buff_status[1] = 3;
    } else if (buff_status[0] == 2) {
      buff_status[0] = 3;
    }
  }

  if (i < 65) {
    val_tmp = wave[i] + offset;
  }

  portENTER_CRITICAL_ISR(&timerMux);
  dac_output_voltage(DAC_CHANNEL_1, val_tmp);

  portEXIT_CRITICAL_ISR(&timerMux);

  if (i < 390) {
    i++;
  } else {
    i = 0;

    if (buff_status[1] == 1) {
      buff_status[1] = 2;
    } else if (buff_status[0] == 1) {
      buff_status[0] = 2;
    }
  }
}

bool curveTracer_handle(void) {
  // Serial.write(120); // "x": (ASCII Table: 120) http://www.asciitable.com/

  // delayMicroseconds(1);

  if (buff_status[buff_index] == 3) {

    for (int i = 0; i < 64; i++) {
      f_x[i] = (float)adc0[i][buff_index] * 0.000976563;
    }

    f_x[64] = (float)adc0[64][buff_index] * 0.000976563;

    //Serial.println(f_x[15]);

    //  Serial.write(121); // "y": 121

    for (int i = 0; i < 64; i++) {
      f_y[i] = (float)adc1[i][buff_index] * 0.000976563;
    }

    f_y[64] = (float)adc1[64][buff_index] * 0.000976563;

    //  Serial.write(122); // "z": 122

    for (int i = 0; i < 64; i++) {
      f_z[i] = (float)adc2[i][buff_index] * 0.000976563;
    }

    f_z[64] = (float)adc2[64][buff_index] * 0.000976563;

    //  Serial.write(35); // "#": 35

    buff_status[buff_index] = 1;

#define Pane1Width 480
#define Pane1Height 480

#define Pane1Width_2 220

    //    float x_tmp;
    //    float y_tmp;

    float calc_diff = 0;

    // Serial.println(f_x[15] * 1000);

    for (int i = 0; i < 65; i++) {
      f_x1[i] = (f_x[i] - f_y[i]) * 1000;

      f_y1[i] = (f_y[i]) * 1000;

      f_x2[i] = (f_x[i] - f_z[i]) * 1000;

      f_y2[i] = (f_z[i]) * 1000;

      if (1 > (f_y[i] / f_z[i])) {
        calc_diff += 1 - (f_y[i] / f_z[i]);
      } else {
        calc_diff += 1 - (f_z[i] / f_y[i]);
      }
    }

    diff_percent = calc_diff * 3.956; // 3.956: to fit in the 0-100 range

    if (buff_index == 0) {
      buff_index = 1;
      //digitalWrite(led, HIGH);
    } else {
      buff_index = 0;
      //digitalWrite(led, LOW);
    }

    return true;
  } else {
    return false;
  }
  //  delayMicroseconds(2000);
}

bool hc595_buffer0 = 0;
byte hc595_buffer1 = 0;

void set_relay_current(bool state) {
  if (state == true) {
    hc595_buffer0 = 1;
  } else {
    hc595_buffer0 = 0;
  }

  set_hc595(hc595_buffer0, hc595_buffer1);
}

void set_relay_ch(byte state) {
  hc595_buffer1 = state;

  set_hc595(hc595_buffer0, hc595_buffer1);
}

void set_hc595(bool _dat0, byte _dat1) {
  bool dat_test;
  uint64_t dat_tmp = 0;

  if(_dat1 > 0) {
    //dat_tmp = (uint64_t)(((uint64_t)(1 << (_dat1 - 1)) << 1) + _dat0);
    dat_tmp = _dat1 - 1;
    dat_tmp = 1 << dat_tmp;
    dat_tmp = dat_tmp << 1;
    dat_tmp = (uint64_t)(dat_tmp | (uint64_t)_dat0);
  } else {
    dat_tmp = _dat0;
  }

  for (int i = 63; i >= 0; i--) {
    dat_test = (uint64_t)(dat_tmp >> i) & 1;
    if (dat_test == 1) {
      digitalWrite(HC595_SER, HIGH);
    } else {
      digitalWrite(HC595_SER, LOW);
    }

    digitalWrite(HC595_SRCLK, HIGH);
    digitalWrite(HC595_SRCLK, LOW);
  }

  digitalWrite(HC595_RCLK, HIGH);
  digitalWrite(HC595_RCLK, LOW);
}
