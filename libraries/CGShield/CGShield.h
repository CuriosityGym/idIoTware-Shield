/* 
  FunShield.h - Libary for using the FunShield
  MakersBox.blogspot.com
  648.ken@gmail.com
  January 2014

*/

#include <Wire.h>


#ifndef CGShield_h
#define CGShield_h
#include <Adafruit_NeoPixel.h>
#include "Arduino.h"
#define Led 1
#define LEFT false
#define RIGHT true
#define ON true
#define OFF false
#define servoPin 5
  
class CGShield
{
  public:
    // variables
	byte ME;
	byte FRIEND;
    
    //boolean C;
	//boolean F;
	//boolean L;
	//boolean R;

    //byte datapin; 
	//byte clockpin;
    //byte latchpin;
	//byte data;


	
	// Function declarations
	// FunShield();
	CGShield();
    CGShield(byte ME, byte FRIEND);
	//void LED(byte led, boolean state);

	//int trim();   // returns 10-bit analog trim potentimeter reading
	//int analog(); // returns 10-bit analog header reading
	//int temp();   // returns 10-bit analog temperature reading
	//float temp(boolean mode);     // return temp in C (true) or F (false)
	//void RGB(byte RGBcolor[]);    // light RGB with (R, G, B) values
	//void RGB(int color);          // light RGB with spectrum of 0 to 768 
	//boolean button(boolean side); // return value of left (true) or right //(false) button
	//boolean touch();              // returns boolean if touch sensor is touched
	//void motor(int speed);        // provide PWM to motor transistor

	
  private:

};

void LED(byte led, boolean state);
void LED(byte ADDRESS, byte led, boolean state);
void LEDByte(byte data);
void receiveEvent(int howMany);
void requestEvent();
int getReading(byte ADDRESS, byte sensor);
int light();
int light(byte ADDRSS);
int trim();
int trim(byte ADDRESS);
int analog();
int analog(byte ADDRESS);
boolean touch();
boolean touch(byte ADDRESS);
float temp();
float temp(byte ADDRESS);
void motor(byte speed);
void motor(byte ADDRESS, byte speed);
boolean button(boolean side);
boolean button(byte ADDRESS, boolean side);
//void initialize();
void color(byte red,byte green,byte blue);
void brightness(byte intensity);
//void update();
void RGB(byte red, byte green, byte blue);
void RGB(byte RGBcolor[]);    // light RGB with (R, G, B) values
void RGB(int color);          // light RGB with spectrum of 0 to 768 
void RGB(byte ADDRESS, byte RGBcolor[]);
void RGB(byte ADDRESS, int color);
void RGB(); // switch between common cathod and common annode
void speaker(unsigned int frequency, unsigned long duration); 
void speaker(byte ADDRESS, unsigned int frequency, unsigned long duration);
#endif
