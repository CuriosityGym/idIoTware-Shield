/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
  **************************************************************/

/**************************************************************
 * the idIoTware shield uses a RGB ws2812B LED connected on d6 of the arduino.
 *  Since there is no representation in the Blynk Library for the WS2812B LED, we use a virtual pin to recieved RGB color data. 
 *  This color data, is recieved via the Blynk http api, and not through the app. The Specific URL for the API is called from the Maker channel on IFTTT
 *  
 *  Whenever there is an event on a Google Calender, the Maker channel triggers the RGB LED to glow. 
 * 
 * 
 ***************************************************************/
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>


#include <Adafruit_NeoPixel.h>

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Add your blynk token here";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Add your SSID Here";
char pass[] = "Add your Wifi Password here";

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial debugSerial(10, 9); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

void setup()
    {
      // Set console baud rate
      debugSerial.begin(9600);
      delay(10);
      // Set ESP8266 baud rate
      EspSerial.begin(ESP8266_BAUD);
      delay(10);

      Blynk.begin(auth, wifi, ssid, pass);
      strip.begin();
      strip.show();
    }

BLYNK_WRITE(V0)
{
 int red = param.asInt(); // We send 255 if we want to switch on the red LED inside the idIoTware Shield, or a 0 if we want to turn it off. 
 strip.setPixelColor(0,red,0,0); //Turn the First LED, to the color recieved into red component, set the blue and green to zero 
 strip.show();
}


             

void loop()
{
  Blynk.run();
}

