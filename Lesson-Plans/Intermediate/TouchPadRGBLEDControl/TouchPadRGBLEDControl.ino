/*

  The IdIoTWare Shield uses Touchpad attached to pin 4.
  
  The IdIoTWare Shield uses a addressable  RGB LED to display a spectrum of colors.
  The CGShield library abstracts the details and provides 
  you the following function:
  
    color(RED, GREEN, BLUE);   // three arguments are the pixel color, expressed as red, 
                               // green and blue brightness levels, 
                               // where 0 is dimmest (off) and 255 is maximum brightness (0-255).
                               // for example to set RED color, color(255,0,0);
  
  In this example, WS2812 Led changes its color as we touch the touchpad.  

*/

// sets up and initialize CGShield
#include <Adafruit_NeoPixel.h>
#define PIN 6
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int buzzerPin = A1;
const byte touchpadPin= 4;
int counter;

void setup()
    {
      Serial.begin(9600);
      pinMode(touchpadPin,INPUT); // declare touchpad as input
      pinMode(buzzerPin,OUTPUT);
      strip.begin();
      strip.show(); // Initialize all pixels to 'off'

      strip.setBrightness(150);
    }
  
void loop()
    {  
       touchpad();
       if(touchpad())
       { 
         beep(250);
         counter++;
         Serial.println(counter);
         switch(counter) 
              { 
                case 1: strip.setPixelColor(0,127,0,255);strip.show();
                break;
                  
                case 2: strip.setPixelColor(0,0,0,255);strip.show();
                break;
                  
                case 3: strip.setPixelColor(0,0,255,0);strip.show();
                break;
                  
                case 4: strip.setPixelColor(0,255,255,0);strip.show();
                break;
                  
                case 5: strip.setPixelColor(0,255,128,0);strip.show();
                break; 
                  
                case 6: strip.setPixelColor(0,255,0,0);strip.show();
                break; 
                  
                case 7: strip.setPixelColor(0,255,255,255);strip.show();
                break;
                  
                case 8: strip.setPixelColor(0,0,0,0);strip.show();    
                break;    
              } 
       }
      if(counter==8) counter = 0; 
      delay(100);
    }
//function to check input from touchpad
int touchpad()
   {
     int touched = digitalRead(touchpadPin); 
     if(touched == HIGH)   // check if the input is HIGH 
     
     return true;
     else
     return false;
   }
   
void beep(int delayValue)
     {
       digitalWrite(buzzerPin,HIGH);
       delay(delayValue);
       digitalWrite(buzzerPin,LOW);
     }

