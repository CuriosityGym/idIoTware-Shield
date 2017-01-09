/*
Getting started with idIoTware Shield and BLYNK.
Attention: Please install all libraries from our Github Repository to enable this example to run.

In this example we are using ESP8266-01 Wifi Module.

Blynk is a platform with iOS and Android apps to control Arduino, Raspberry Pi and the likes over the Internet. You can easily build graphic interfaces for all your projects by simply dragging and dropping widgets.

Downloads, docs, tutorials: http://www.blynk.cc Blynk community: http://community.blynk.cc

To send data from Blynk to arduino or get data from arduino to blynk app you need to install Blynk app on your smartphone, create account to log in to the app then you will get a unique token number then use that token number in this code to create your own project.

In this example we have used ESP8266_Lib.h and BlynkSimpleShieldEsp8266.h library to make communication between arduino and Blynk app

*/
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "xxxxxxxxxxxxxxxxxxxx";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "abcd";
char pass[] = "**********";

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


             

void loop()
{
  Blynk.run();
}


