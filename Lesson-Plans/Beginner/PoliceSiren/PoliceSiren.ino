

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
// piezo speaker connected to pin A1 
// Which pin on the Arduino is connected to the NeoPixels?

#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int buzzerPin = A1;
#define wait 1
#define time 10
int freq;

void setup()
     {
      pixels.begin(); // This initializes the NeoPixel library.
     }

void loop() 
    {
      for(freq = 150; freq < 1800; freq += 1)
         {
           
           pixels.setPixelColor(0, pixels.Color(0,0,freq)); // Blue Color.  
           tone(buzzerPin, freq, time);     // Beep pin, freq, time
           delay(wait);
         }
      for(freq = 1800; freq > 150; freq -= 1) 
         {
           
           pixels.setPixelColor(0, pixels.Color(freq,0,0)); // Red Color
           tone(buzzerPin, freq, time);     // Beep pin, freq, time
           delay(wait);
         }
    }

