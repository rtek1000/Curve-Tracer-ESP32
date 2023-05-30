ESP32Time rtc(0);  // offset in seconds GMT-3: -10800 (3600 x3)

AsyncWebServer server(80);

int prog_select = 0;
int soak_timer = 0;
int wash_timer = 0;
int rinse_timer = 0;
int cent_timer = 0;
int water_level = 0;
int power_state = 0;

unsigned long millis1 = 0;
unsigned long millis2 = 0;
unsigned long millis3 = 0;
unsigned long millis4 = 0;

WebSocketsServer webSocket = WebSocketsServer(88);
String javaScript, JSONtxt;
unsigned long websockCount = 1, wait000 = 0UL, wait001 = 0UL;
unsigned int senData = 0;
int websockMillis = 50;                  // SocketVariables are sent to client every 50 milliseconds

void server_setup(void) {
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    // return;
  }

  // https://techtutorialsx.com/2020/04/17/esp32-mdns-address-resolution/
  if (!MDNS.begin("esp32ct")) {
    Serial.println("Error starting mDNS");
    return;
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String());
    //request->send(SPIFFS, "/index2.html", String());
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to load style.css file
  server.on("/style_cookie_consent.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style_cookie_consent.css", "text/css");
  });

  server.on("/panel", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/panel.jpg", "image/jpg");
  });

  server.on("/panel_disabled", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/panel_disabled.jpg", "image/jpg");
  });

  server.on("/lock", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/lock.jpg", "image/jpg");
  });

  server.on("/unlock", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/unlock.jpg", "image/jpg");
  });

  server.on("/setBTN", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->hasParam("BTNstate")) {
      handleBTN(request);
    } else {
      request->send(200, "text/plain", "ERROR");
    }
  });

  server.on("/readDATA", HTTP_GET, [](AsyncWebServerRequest * request) {
    handleDATA(request);
  });

  server.on("/setDATA", HTTP_GET, [](AsyncWebServerRequest * request) {
    handleSetDATA(request);
  });

  server.on("/panel", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/panel.jpg", "image/jpg");
  });

  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void handleBTN(AsyncWebServerRequest * request) {
  String t_state = "";
  String ledState = "OFF";
  String str_response = "ERROR";

  if (request->hasParam("BTNstate") == false) {
    request->send(200, "text/plain", "ERROR");

    return;
  }

  t_state = request->arg("BTNstate");

  Serial.println(t_state);

  if (t_state == "1") {
    if (power_state == 0) {
      power_state = 1;
    } else {
      power_state = 0;
    }

    if (power_state == 1) {
      ledState = "ON"; //Feedback parameter
    }
    else
    {
      ledState = "OFF"; //Feedback parameter
    }

    str_response = "OK";
  } else if (t_state == "2") {
    if ((millis() - millis2) > 3000) {
      millis2 = millis();

      if ((millis() - millis3) < 15000) {
        millis3 = millis();

        if (prog_select < 3) {
          prog_select++;
        } else {
          prog_select = 0;
        }
      } else {
        millis3 = millis();
      }
    } else {
      millis2 = millis();

      if (prog_select == 0) {
        if (soak_timer < 2) {
          soak_timer++;
        } else {
          soak_timer = 0;
        }
      } else if (prog_select == 1) {
        if (wash_timer < 2) {
          wash_timer++;
        } else {
          wash_timer = 0;
        }
      } else if (prog_select == 2) {
        if (rinse_timer < 2) {
          rinse_timer++;
        } else {
          rinse_timer = 0;
        }
      } else if (prog_select == 3) {
        if (cent_timer < 2) {
          cent_timer++;
        } else {
          cent_timer = 0;
        }
      }
    }

    str_response = "OK";
  } else if (t_state == "3") {
    if (water_level < 3) {
      water_level++;
    } else {
      water_level = 0;
    }

    str_response = "OK";
  }

  request->send(200, "text/plain", str_response);

  Serial.print("handleBTN: ");

  Serial.println(str_response);
}

void handleDATA(AsyncWebServerRequest * request) {
  String str1 = sendLocalTime(); // String(counter1);
  str1 = str1 + ';' +
         prog_select + ';' +
         soak_timer + ';' +
         wash_timer + ';' +
         rinse_timer + ';' +
         cent_timer + ';' +
         water_level + ';' +
         power_state + "; #";

  request->send(200, "text/plain", str1);
}

void handleSetDATA(AsyncWebServerRequest * request) {
  String data_state = "";
  String str_response = "ERROR";

  if (request->hasParam("PROGstate")) {
    data_state = request->arg("PROGstate");

    if ((data_state.toInt() >= 0) && (data_state.toInt() <= 3)) {
      prog_select = data_state.toInt();

      Serial.print("prog_select: ");

      Serial.println(prog_select);

      str_response = "OK";
    }
  } else if (request->hasParam("SOAKstate")) {
    data_state = request->arg("SOAKstate");

    if ((data_state.toInt() >= 0) && (data_state.toInt() <= 2)) {
      soak_timer = data_state.toInt();

      str_response = "OK";
    }
  } else if (request->hasParam("WASHstate")) {
    data_state = request->arg("WASHstate");

    if ((data_state.toInt() >= 0) && (data_state.toInt() <= 2)) {
      wash_timer = data_state.toInt();

      str_response = "OK";
    }
  } else if (request->hasParam("RINSEstate")) {
    data_state = request->arg("RINSEstate");

    if ((data_state.toInt() >= 0) && (data_state.toInt() <= 2)) {
      rinse_timer = data_state.toInt();

      str_response = "OK";
    }
  } else if (request->hasParam("CENTstate")) {
    data_state = request->arg("CENTstate");

    if ((data_state.toInt() >= 0) && (data_state.toInt() <= 2)) {
      cent_timer = data_state.toInt();

      str_response = "OK";
    }
  } else if (request->hasParam("LEVELstate")) {
    data_state = request->arg("LEVELstate");

    if ((data_state.toInt() >= 0) && (data_state.toInt() <= 3)) {
      water_level = data_state.toInt();

      str_response = "OK";
    }
  }

  request->send(200, "text/plain", str_response);

  Serial.print("handleSetDATA: ");

  Serial.println(str_response);
}

void printLocalTime(void)
{
  struct tm timeinfo = rtc.getTimeStruct();

  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

String sendLocalTime(void)
{
  struct tm timeinfo = rtc.getTimeStruct();

  char timeStringBuff[50]; //50 chars should be enough
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);

  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  return timeStringBuff;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t wslength)
{
  String payloadString = (const char *)payload;
  Serial.println(payloadString);
  //Serial.println("payload: '"+payloadString+"', channel: "+(String)num);

  switch (type) {
    case WStype_DISCONNECTED: // enum that read status this is used for debugging.

      break;
    case WStype_CONNECTED:  // Check if a WebSocket client is connected or not

      break;
    case WStype_TEXT: // check responce from client
      byte separator = payloadString.indexOf(':');
      String var = payloadString.substring(4, separator - 1);
      String val = payloadString.substring(separator + 1);

      // Serial.println("filter1: " + filter1);
      Serial.println("var: " + var);
      Serial.println("val: " + val);

      if (var == "client_relay") {
        ch_relay = val.toInt();

        if(ch_relay > 63) {
          ch_relay = 63;
        }

        set_relay_ch(ch_relay);
      }

      //    if (var == "LEDonoff") {
      //      LEDonoff = false;
      //      if (val == "ON")LEDonoff = true;
      //      digitalWrite(LED, HIGH);
      //    } else if (var == "sliderVal") {
      //      sliderVal = val.toInt();
      //      LEDmillis = 9 * (100 - sliderVal) + 100;
      //    }
      break;
  }
}

void webSocket_handle(void) {
  webSocket.loop();

  if (millis() > wait001) {
    if (curveTracer_handle() == true) {
      websockCount++;

      // websockCount = websockCount << 1;
      //  Serial.println(websockCount);
      senData = websockCount;

      //    String LEDswitch = "LED = OFF";
      //    if (LEDonoff == true)LEDswitch = "LED = ON";
      JSONtxt = "{";

      // x
      JSONtxt += "\"val_x\":[";

      for (int i = 0; i < 64; i++) {
        JSONtxt += "\"" + (String)(f_x[i] * 1000) + "\",";
      }

      JSONtxt += "\"" + (String)(f_x[64] * 1000) + "\"],";

      // y
      JSONtxt += "\"val_y\":[";

      for (int i = 0; i < 64; i++) {
        JSONtxt += "\"" + (String)(f_y[i] * 1000) + "\",";
      }

      JSONtxt += "\"" + (String)(f_y[64] * 1000) + "\"],";

      // z
      JSONtxt += "\"val_z\":[";

      for (int i = 0; i < 64; i++) {
        JSONtxt += "\"" + (String)(f_z[i] * 1000) + "\",";
      }

      JSONtxt += "\"" + (String)(f_z[64] * 1000) + "\"],";

      // diff
      JSONtxt += "\"diff\":\"" + (String)(diff_percent) + "\",";

      // relay
      JSONtxt += "\"srv_relay\":\"" + (String)(ch_relay) + "\",";

      // relay max
      JSONtxt += "\"srv_relay\":\"" + (String)(ch_relay_max) + "\"";

      JSONtxt += "}";

      webSocket.broadcastTXT(JSONtxt);
      wait001 = millis() + websockMillis;
    }
  }
}
