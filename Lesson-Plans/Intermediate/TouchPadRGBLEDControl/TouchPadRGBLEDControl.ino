/*

  The IdIoTWare Shield uses Touchpad attached to pin 4.
  
  The IdIoTWare Shield uses a addressable  RGB LED to display a spectrum of colors.
  The CGShield library abstracts the details and provides 
  you the following function:
  
    color(RED, GREEN, BLUE);   // three arguments are the pixel color, expressed as red, 
                               // green and blue brightness levels, 
                               // where 0 is dimmest (off) and 255 is maximum brightness (0-255).
                               // for example to set RED color, color(255,0,0);
  
  In this example, WS2812 Led changes its color as we toch the touchpad.  

*/

// sets up and initialize CGShield
#include <Adafruit_NeoPixel.h>
#include <idIoTwareShield.h>
#include <Wire.h>         // Require for I2C communication
idIoTwareShield fs;             // Instanciate CGShield instance
int buzzerPin = A1;
const byte touchpadPin= 4;
int counter;

void setup()
    {
      Serial.begin(9600);
      pinMode(touchpadPin,INPUT); // declare touchpad as input
      pinMode(buzzerPin,OUTPUT);
      brightness(150);
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
                case 1: color(127,0,255);
                break;
                  
                case 2: color(0,0,255);
                break;
                  
                case 3: color(0,255,0);
                break;
                  
                case 4: color(255,255,0);
                break;
                  
                case 5: color(255,128,0);
                break; 
                  
                case 6: color(255,0,0);
                break; 
                  
                case 7: color(255,255,255);
                break;
                  
                case 8: color(0,0,0);    
                break;    
              } 
       }
      if(counter==8) counter = 0; 
      
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
