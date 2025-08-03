#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ElegantOTA.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "credentials.h"
#define LED_BUILTIN 2
const uint16_t port = 80;

// StaticIP and gateway address based on your home router settings
IPAddress local_IP(192, 168, 1, 26);
IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 1, 254);

String request;

// Current ArduinoJson release v7.x gets "StaticJsonDocument' is deprecated"
// Just ignor it or see if ArduinoJson v8.x is released
ArduinoJson::StaticJsonDocument<200> doc;           
//ArduinoJson::StaticJsonDocument<200> doc_storage;
//JsonDocument& doc = doc_storage;

AsyncWebServer server(port);

#define SEND_PIN 4
IRsend irsend(SEND_PIN);

// StaticIP Method
void connectToWiFi() {
  
  Serial.println("Setting WiFi Configuration ... \n");
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  Serial.printf("Connecting to WiFi %s ... \n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.printf("WiFi connected at http:\\%s ... \n", WiFi.localIP());
  //Serial.println(WiFi.macAddress());
}

// Array to hold your IR codes
// unsigned long infraRedCode[] = {0x23802600, 0x23802601, etc, etc};
// But when there are a lot of IR codes store them in their own file
#include "irCodes.h"

void onRequest(AsyncWebServerRequest *request) {
    // dummy callback function for handling params, etc.
}

void onFileUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
    // dummy callback function signature, not used in our code
}

uint8_t setSwitch(uint8_t newCode){
  uint8_t protocolName;
    if (newCode >= 49){
      protocolName = PANASONIC;
    }else if(newCode <= 40){
      protocolName = RCMM;
    }else{
      protocolName = NEC;
    }
  return protocolName;
}
 
void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  digitalWrite(LED_BUILTIN, HIGH);
  DeserializationError error = deserializeJson(doc, (char*)data);
  if (!error) {
    const char* command = doc["command"];
    uint8_t code = atoi(doc["code"]);
    uint8_t codeName = setSwitch(code); //Assigns correct switch case value so correct protocol is used

    Serial.printf("command=%s, code=0x%lx, protocol = ", command, infraRedCode[code]);

    switch (codeName) {
      case RCMM:
        irsend.sendRCMM(infraRedCode[code], 32);
        Serial.println("mySKY HDi \n");
        break;
      case NEC:
        irsend.sendNEC(infraRedCode[code], 32);
        Serial.println("ONKYO RECEIVER \n");
        break;
      case PANASONIC:
        irsend.sendPanasonic(0x4004, infraRedCode[code], 48);
        Serial.println("PANASONIC BLURAY / DVR \n");
        break; 
    }
    request->send(200, "text/plain", "Ok");
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void setup() {

    irsend.begin();
    
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Connect to WiFi
    connectToWiFi();

    Serial.print(F("Inizializing FS..."));
    if (SPIFFS.begin()){
        Serial.println(F("done."));
    }else{
        Serial.println(F("fail."));
    }

    // serving static content for GET requests on '/' from SPIFFS directory '/'
    server.serveStatic("/", SPIFFS, "/")
          .setDefaultFile("index.html")
          .setCacheControl("max-age=86400");

    server.on("/api", HTTP_POST, onRequest, onFileUpload, onBody);

    server.onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Page Not Found");
    });
    ElegantOTA.begin(&server);    // Start ElegantOTA
    server.begin();
    digitalWrite(LED_BUILTIN, LOW);
}
void loop(){
  ElegantOTA.loop();
}
