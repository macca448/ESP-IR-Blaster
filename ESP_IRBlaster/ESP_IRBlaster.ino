#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <FS.h>   
#endif  // ESP8266
#if defined(ESP32)
#include <WiFi.h>
#include <SPIFFS.h> 
#endif  // ESP32
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "credentials.h"
#define LED_BUILTIN 2
const int port = 80;

// StaticIP and gateway address based on your home router settings
IPAddress staticIP(192, 168, 1, 22);
IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);

String request;
int codeName;
DynamicJsonDocument doc(200);
AsyncWebServer server(port);

byte SEND_PIN = 4;
IRsend irsend(SEND_PIN);

// StaticIP Method
void connectToWiFi() {
    Serial.print("Setting WiFi Configuration ... ");
    Serial.println(WiFi.config(staticIP, gateway, subnet) ? "Ready" : "Failed");
    Serial.printf("Connecting to WiFi %s ... ", ssid);
    Serial.println(WiFi.begin(ssid, password) ? "Ready" : "Failed");
    while (WiFi.status() != WL_CONNECTED) {
      delay(200);
      Serial.print(".");
    }
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.macAddress());
}

/* DHCP Method 
   void connectToWiFi() {
    Serial.print("Setting WiFi Configuration ... ");
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP().toString());
}*/

// Array to hold your IR codes
// unsigned long infraRedCode[] = {0x23802600, 0x23802601, etc, etc};
// But when there are a lot of IR codes store them is their own file
#include "irCodes.h"

void onRequest(AsyncWebServerRequest *request) {
    // dummy callback function for handling params, etc.
}

void onFileUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
    // dummy callback function signature, not used in our code
}

void setSwitch(uint8_t newCode){
    if (newCode >= 49){
      codeName = PANASONIC;
    }else if(newCode <= 40){
      codeName = RCMM;
    }else{
      codeName = NEC;
    }
    return;
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    DeserializationError error = deserializeJson(doc, (char*)data);
    if (!error) {
      const char* command = doc["command"];
      uint8_t code = atoi(doc["code"]);
      setSwitch(code);
      Serial.printf("command=%s, code=0x%lx, protocol = ", command, infraRedCode[code]);
      switch (codeName) {
        case RCMM:
          irsend.sendRCMM(infraRedCode[code], 32);
          digitalWrite(LED_BUILTIN, HIGH);
          Serial.println("mySKY HDi \n");
          break;
        case NEC:
          irsend.sendNEC(infraRedCode[code], 32);
          digitalWrite(LED_BUILTIN, HIGH);
          Serial.println("ONKYO RECEIVER \n");
          break;
        case PANASONIC:
          irsend.sendPanasonic(0x4004, infraRedCode[code], 48);
          digitalWrite(LED_BUILTIN, HIGH);
          Serial.println("PANASONIC BLURAY / DVR \n");
          break; 
      }
      request->send(200, "text/plain", "Ok");
      digitalWrite(LED_BUILTIN, LOW);
    }
}

void setup() {

    irsend.begin();
    
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

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

    server.begin();
}
void loop(){}
