

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>


#include <Adafruit_NeoPixel.h>

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

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


int pot = A2;
int temp = A0;
int ldr = A3;
int pot_val;
float temp_val;
int ldr_val;

BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  pot_val = analogRead(pot);
//Serial.println(pot_val);
//delay(1000);
ldr_val = analogRead(ldr);
//Serial.println(ldr_val);
temp_val = analogRead(temp)*0.48828;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V16, temp_val);
  Blynk.virtualWrite(V5, pot_val);
  Blynk.virtualWrite(V7, ldr_val);
  Blynk.virtualWrite(V9, pot_val, ldr_val, temp_val);
}

void setup()
{
  // Set console baud rate
      debugSerial.begin(115200);
      delay(10);
      // Set ESP8266 baud rate
      EspSerial.begin(ESP8266_BAUD);
      delay(10);
      Blynk.begin(auth, wifi, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(18000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
