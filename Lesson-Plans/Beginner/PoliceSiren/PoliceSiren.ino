
// sets up and initialize CGShield
#include <Adafruit_NeoPixel.h>
#include <CGShield.h>
#include <Wire.h>         // Require for I2C communication
CGShield fs;             // Instanciate CGShield instance

// piezo speaker connected to pin A1 
int buzzerPin = A1;
#define wait 1
#define time 10
int freq;

void setup()
     {
      
     }

void loop() 
    {
      for(freq = 150; freq < 1800; freq += 1)
         {
           color(0,0,freq);  
           tone(buzzerPin, freq, time);     // Beep pin, freq, time
           delay(wait);
         }
      for(freq = 1800; freq > 150; freq -= 1) 
         {
           color(freq,0,0);
           tone(buzzerPin, freq, time);     // Beep pin, freq, time
           delay(wait);
         }
    }

