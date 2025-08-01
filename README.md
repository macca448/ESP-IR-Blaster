# ESP-IR-Blaster
Use an ESP8266 or ESP32 to create a WiFi Infrared Blaster and send code using a browser enabled device on your local network like a mobile phone, a tablet or even a Laptop. **IMPORTANT** Please read the [update notes](https://github.com/macca448/ESP-IR-Blaster/blob/main/update-notes.md) for current library requirements

## Sketch Source Project:
[This is the Project](https://github.com/e-tinkers/esp32_ir_remote) that gave me the required functionality and AJAX script.

**Note:** I stripped out all "HTTP_GET" functionality in my project as I only wanted to send "HTTP_POST" commands. If you need to pull data into your HTML like sensor readings then you'll need to look at the [core code source project](https://github.com/e-tinkers/esp32_ir_remote) for that detail.  

## Sketch Flow Basics
In my sketch there is just a single task being the  `"void onBody()"` callback and this function is triggered by a HTML button press. There is javaScript that creates the `JASON` string when a button is pressed. In the string is the button's HTML ID and the button's description text. This description string isn't needed for functionality but was very handy debugging as I could see if the IR code emitted matched the function printed.  

The button ID is the key as all the IR codes for the buttons live in a single array and the HTML ID is the array index position of the IR code for the button that was pressed. As there are three (3) protocols `“NEC, PANASONIC and RCMM”` the array is sorted in groups so I can assign protocol by ID/index number.  

This means on a button Press > Stringify ID and Name > Pass to “onBody” callback > parse JSON > check ID and set protocol > pass the switch case to emit IR code.

**Minor stuff:** There are near 100 buttons and I used 9 x Buttons that load model pop-ups with buttons grouped by device and function. I call “FontAwesome” externally so I need internet access and this is for button icons. 
At the bottom of this page is a link to a short video showing the ESP-IR-Blaster in operation.

<br>

## Arduino IDE libraries and tools:

First you need to make sure the Arduino IDE has support for your [ESP8266](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/) or [ESP32](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) library installed.

**IMPORTANT** Please read the [update notes](https://github.com/macca448/ESP-IR-Blaster/blob/main/update-notes.md) for current library requirements as a patch may be required if you are using an ESP32

[IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) This library has an impressive list of supported protocols.

[ESPAsyncWebServer](https://github.com/ESP32Async/ESPAsyncWebServer) Important if you are using sensor data as you can update values without full page reloads
  
[ArduinoJson](https://github.com/bblanchon/ArduinoJson) to facilitate the AJAX exchanges

[ElegantOTA](https://github.com/ayushsharma82/ElegantOTA) Enables `Over The Air` updates to your controller



### For Arduino IDE v1.8.19
This older IDE still works and has two JAVA plug-ins that allow easy `data`folder upload to your `HTML` content to the controller. Here are links to instructions for the [ESP32](https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/) and the [ESP8266](https://randomnerdtutorials.com/install-esp8266-filesystem-uploader-arduino-ide/) for adding this tool.

### For Arduino IDE v2.x
You'll note that I have `OTA` sample sketch's for both the `ESP8266` using `LittleFS` and `ESP32` using `SPIFFS`.

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
