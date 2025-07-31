The original release of this project was over 4 years ago. It was due a refresh to work with current libraries.

## Update 31 July 2025
- Currently the Arduino IDE Library Manager version of IRremoteESP8266 (v2.8.6) does not support the ESP32 Arduino core V3.x.
- If you are using an ESP32 I have a patch version of `IRrecv.cpp` class file you can download and manually install from [here](https://github.com/macca448/IRremoteESP8266_patch)
- I have tested `IRrecv.h` and `IRsend.h` library functions with the [patch](https://github.com/macca448/IRremoteESP8266_patch) applied and it all works as it should. My ESP32 IR-Blaster now has OTA and is working as it should.

### Update for ESP32 Arduino core v3.3.0 in Arduino IDE v2.3.6 as follows
  1.  IRremoteESP8266 (v2.8.6) via Arduino IDE library manager with my [patch](https://github.com/macca448/IRremoteESP8266_patch) applied
  2.  ElegantOTA v3.1.7 via Arduino IDE library manager
  3.  ESPAsyncWebServer v3.7.10 via Arduino IDE library manager
  4.  Async_TCP v3.4.6 (dependancy of ESPAsyncWebServer) via Arduino IDE library manager
  5.  ArduinoJson v7.4.2 via Arduino IDE library manager

#### for ESP8266 Arduino core v3.1.2 in Arduino IDE v2.3.6 as follows
  1.  IRremoteESP8266 v2.8.6 via Arduino IDE library manager
  2.  ElegantOTA v3.1.7 via Arduino IDE library manager
  3.  ESPAsyncWebServer v3.7.10 via Arduino IDE library manager
  4.  ESPAsyncTCP v1.2.2 (dependancy of ESPAsyncWebServer)via Arduino IDE library manager
  5.  ArduinoJson v7.4.2 via Arduino IDE library manager

## End Update
