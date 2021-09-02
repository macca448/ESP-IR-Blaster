# ESP-IR-Blaster
Use an ESP8266 or ESP32 to create a WiFi Infrared Blaster and send code using your mobile phone or a tablet

## Credits:
Project that gave me the core code for Arduino sketch and ajax script <https://github.com/e-tinkers/esp32_ir_remote>

## Libraries needed:
[IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266)

[ArduinoJson](https://github.com/bblanchon/ArduinoJson)

You also need to make sure you install support for your [ESP8266](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/) or for the [ESP32](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) Dev boards.

Once you have support for your developement board installed you will need to install the SPIFFS upload JAVA applet into the Arduino IDE. Here are links to instructions for the [ESP32](https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/) and the [ESP8266](https://randomnerdtutorials.com/install-esp8266-filesystem-uploader-arduino-ide/).

## Tips
### (1) Randomness when capturing IR codes
You will most likely need to capture your own IR codes and if you get random captures (as I did) I strongly recommend you read [this blog](https://arduinoplusplus.wordpress.com/2015/11/22/large-button-remote-control-part-1-design-and-prototyping/) for the answer. In a nutshell it comes down to IR reciever center carrier frequency and what frequency the remote you are capturing uses. IN my case both the Sky and Panasonic used 37Khz and the standard Arduino IR receiver has a center frequency of 38Khz. I swapped to a TSOP4136 (36Khz center frequency) hardwired and everything burst into life. 
The switch and the LED shown in the image below were only for developement and debugging.
![2_Developement_setup](https://user-images.githubusercontent.com/20883620/131769679-007e9228-ec3f-4c8a-888a-9c588138fe0e.jpg)
This is my final setup on an ESP32
![image](https://user-images.githubusercontent.com/20883620/131770408-c979e788-0745-4e63-940a-e35e04b5d6c3.png)

### (2) Handling a large IR code library
The HTML and CSS are the most time consuming and to help avoid incorrectly assigning ID's and IR codes to the wrong buttons I added them as comments to the IR library (irCodes.h) The DOM ID is the array index position. As the remotes are numerically grouped this also let me do a simple "IF" statement for switch case encoder assignment. Depending on your remotes you can do as the core code author did and use the opening hex values. 

### (3) Javascript and Fontawesome
In my project I'm not hosting the javascript or Font Awesome libraries so internet access is required. 

