/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.
    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  This example shows how to fetch data using WebHook GET method
  App project setup:
    WebHook widget on V0, method: GET, url: /pin/
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "80a9953b71584ee4854e0fbf71823901";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "The Promised LAN";
char pass[] = "imagineyourquestion";
// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial debugSerial(10, 9); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

BlynkTimer timer;

BLYNK_WRITE(V0)
{
  
  u8g.firstPage();
  do { u8g.setFont(u8g_font_timB10);
      u8g.drawStr(1,10,"Bitcoin value in USD:");
      u8g.setFont(u8g_font_courB18); 
      u8g.setPrintPos(5, 50);
      u8g.print(param.asStr());
     } while( u8g.nextPage() );

      Blynk.virtualWrite(V2,param.asStr());
}

void getBitcoinValue()
{
  // You can perform HTTPS requests even if your hardware alone can't handle SSL
  // Blynk  can also fetch much bigger messages,
  // if hardware has enough RAM (set BLYNK_MAX_READBYTES to 4096)
  //Blynk.virtualWrite(V0, "https://api.sunrise-sunset.org/json?lat=50.4495484&lng=30.5253873&date=2016-10-01");

   Blynk.virtualWrite(V0, "https://api.thingspeak.com/apps/thinghttp/send_request?api_key=558W30NPHWSXVP3J");

}
void setup()
{
  // Debug console
  Serial.begin(115200);
  u8g.setColorIndex(1);
  // Set console baud rate
  debugSerial.begin(115200);
  delay(10);
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
  timer.setInterval(16000L, getBitcoinValue);
 
  }

void loop()
{
  Blynk.run();
  timer.run();
}
