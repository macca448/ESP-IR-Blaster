## Update 30 July 2025
#### Have updated sketch's to use current libraries with ESP32 Arduion core v3.3.0 in Arduino IDE v2.3.6 as follows
  1.  My IRremoteESP8266 library patch v2.8.7 located [here](https://github.com/macca448/IRremoteESP8266)
  2.  ElegantOTA v3.1.7 via Arduino IDE
  3.  ESPAsyncWebServer v3.7.10 via Arduino IDE
  4.  Async_TCP v3.4.5 (dependancy of ESPAsyncWebServer) via IDE
  5.  ArduinoJson v7.4.2 ia IDE

NOTE 1: There is a compile warning about `StaticJsonDocument` but it's benign so you can ignore.

NOTE 2: Currently sthe sketch's only support ESP32

NOTE 3: Compile Testing ESP8266 it complains about SPIFFS being depreciated.

<br>

To-do : Create sketch's for ESP8266 using LittleFS for data folder files.
#### End Update

# ESP-IR-Blaster
Use an ESP8266 or ESP32 to create a WiFi Infrared Blaster and send code using your mobile phone or a tablet

## Core code source project:
[This is the Project](https://github.com/e-tinkers/esp32_ir_remote) that gave me the core code for the Arduino sketch and the ajax script.
### Note:
I stripped out all "HTTP_GET" functionality in my project as I only wanted to send "HTTP_POST" commands. If you need to pull data into your HTML like sensor readings then you'll need to look at the [core code source project](https://github.com/e-tinkers/esp32_ir_remote) for that detail.

## Arduino IDE libraries and tools:
[IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) This library has an impressive list of supported protocols.

[ArduinoJson](https://github.com/bblanchon/ArduinoJson) to facilitate the AJAX exchanges

You need to make sure the Arduino IDE has support for the [ESP8266](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/) or the [ESP32](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) Dev boards installed.

Plus SPIFFS upload support for the Arduino IDE is also needed. Here are links to instructions for the [ESP32](https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/) and the [ESP8266](https://randomnerdtutorials.com/install-esp8266-filesystem-uploader-arduino-ide/) for adding this tool.

## Tips
### (1) Randomness when capturing IR codes
You will most likely need to capture your own IR codes and if you get random captures (as I did) I strongly recommend you read [this blog](https://arduinoplusplus.wordpress.com/2015/11/22/large-button-remote-control-part-1-design-and-prototyping/) for the answer. In a nutshell it comes down to IR reciever center frequency and what carrier frequency the remote you are capturing uses. In my case both the Sky and Panasonic used 37Khz and the standard Arduino IR receiver has a center frequency of 38Khz. 

I swapped to a TSOP4136 (36Khz center frequency) IR receiver hardwired to the ESP controller and everything burst into life. 

To capture IR codes I recommend you use the example sketch in the library called "IRrecvDumpV2". It's what worked for me. The switch and the LED shown in the image below were only for developement and debugging.
![image](https://user-images.githubusercontent.com/20883620/131914870-c079865c-026f-4c96-882f-d0af76dda977.png)
### and this is my final setup using an ESP32
![image](https://user-images.githubusercontent.com/20883620/131915077-1aee791e-1459-45b9-8441-4c00f3d31061.png)

### (2) Handling a large IR code library
The HTML and CSS are the most time consuming and to help avoid incorrectly assigning ID's and IR codes to the wrong buttons I added them as comments to the IR library (irCodes.h) The DOM ID is the array index position. As the remotes are numerically grouped this also let me do a simple "IF" statement for switch case encoder assignment. The [core code-source project](https://github.com/e-tinkers/esp32_ir_remote) used IR header hex values that may work for you but were not optimal in my case.

### (3) Javascript and Fontawesome
In my project I'm not hosting the javascript or Font Awesome libraries so internet access is required. 

### Watch the video demo
[![Watch the video](https://img.youtube.com/vi/qXwKDf_D-9c/maxresdefault.jpg)](https://youtu.be/qXwKDf_D-9c)
