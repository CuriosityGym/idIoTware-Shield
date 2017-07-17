
#include <Adafruit_NeoPixel.h>
#include "Arduino.h"
#include "CGShield.h"
#include "../Wire/Wire.h"
//#include <Wire.h>

#define WS2812_PIN 6
#define NUMPIXELS 1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WS2812_PIN, NEO_GRB + NEO_KHZ800);

boolean DEBUG = false;
boolean RGB_CC = true;

byte THIS_ADDRESS;
byte THAT_ADDRESS;

// CG Shield Pin Outs
byte datapin    = 4; 
byte clockpin   = A4;
byte latchpin   = 2;  
byte data = 0;  // global data to send to shift register

byte lightPin    = A0;
byte trimPin   = A1;
byte speakerPin    = A2;
byte tempPin  = A3; 

byte motorPin   = 3;
byte IRPin   = 3;


byte buttonPinR = 7;  // right side
byte buttonPinL = 8;  // left side

byte touchPin   = A3;
byte analogPin=A5;	
int analogValue;
int trimValue;
int tempValue;
byte buttonLvalue;
byte buttonRvalue;
byte package[8];
long lastUpdate = 0;	
byte pointer = 0;	 // points to what data to return
byte MSB, LSB;


CGShield::CGShield(){
  CGShield(0, 0); // no I2C needed.
}

	
CGShield::CGShield(byte ME, byte FRIEND){

  THIS_ADDRESS = ME;
  THAT_ADDRESS = FRIEND;
 
  // don't know a better way to load these arrays
  //RED[0] = 255; RED[1] = 0;  RED[2] = 0;
  //GREEN[0] = 0; GREEN[1] = 255;  GREEN[2] = 0;
  //BLUE[0] = 0; BLUE[1] = 0;  BLUE[2] = 255;
  
  
  // Set the three SPI pins to be outputs:
  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);  
 // pinMode(latchpin, OUTPUT);
  
  // turn all off
  shiftOut(datapin, clockpin, MSBFIRST, 0);

  // Toggle the state of the latchPin,
  digitalWrite(latchpin, HIGH);
  digitalWrite(latchpin, LOW); 

  // disable digital13
  pinMode(13, INPUT);
  digitalWrite(13, LOW);
  
  // set input pins
  pinMode(trimPin, INPUT);
  pinMode(analogPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(buttonPinR, INPUT);
  pinMode(buttonPinL, INPUT);
  
  // engage pull ups
  digitalWrite(lightPin, HIGH);
  digitalWrite(analogPin, HIGH);
  digitalWrite(buttonPinR, HIGH);
  digitalWrite(buttonPinL, HIGH);  
  
  // setup outputs
 
  pinMode(speakerPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  
  // initial RGB states
 

  if(THAT_ADDRESS > 0){
	//Serial.println("WIRE BEGIN");
	Wire.begin(ME);
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent); // register event
  }

}


void color(byte red,byte green,byte blue){
  pixels.begin();
  pixels.setPixelColor(NUMPIXELS-1, pixels.Color(red, green, blue)); 
  pixels.show();
  delay(1);  
}
//The overall brightness of all the LEDs can be adjusted using Brightness(). 
//This takes a single argument, a number in the range 0 (off) to 255 (max brightness)
void brightness(byte intensity){
     pixels.setBrightness(intensity);
     pixels.show();
}



void motor(byte speed){
  analogWrite(motorPin, speed);
}


void motor(byte ADDRESS, byte speed){
	if (ADDRESS == THIS_ADDRESS){
		analogWrite(motorPin, speed);
	}
	else{
		if(THAT_ADDRESS > 0){
			Wire.beginTransmission(THAT_ADDRESS);
			Wire.write(4);      // instuction
			Wire.write(speed);  // led	
			Wire.endTransmission();  
		}
	}
}


int trim(){
  int val = analogRead(trimPin);
  return val;
}


int trim(byte ADDRESS){
	if (ADDRESS == THIS_ADDRESS){
		return trim();
	}
	else{
		return getReading(ADDRESS, 0);
	}
}


int getReading(byte ADDRESS, byte sensor){
	int delayTime = 1; // just a pause to let wire events happen

	if(THAT_ADDRESS > 0){
		// set pointer
		Wire.beginTransmission(THAT_ADDRESS);
		Wire.write(sensor);    // instuction
		Wire.endTransmission();
		delay(delayTime);
		
		// retrieve bytes
		Wire.requestFrom(int(ADDRESS), 1);
		MSB = Wire.read();
		delay(delayTime);
		Wire.requestFrom(int(ADDRESS), 1);
		LSB =Wire.read();
		delay(delayTime);
		unsigned int ret = ((unsigned int) MSB << 8) | LSB;
		return ret;
	}
}


int light(){
  int val = analogRead(lightPin);
  return 1023 - val;  
}


int light(byte ADDRESS){
	if (ADDRESS == THIS_ADDRESS){
		return light();
	}
	else{
		return getReading(ADDRESS, 2);
	}
}


float temp(){
  int val = analogRead(tempPin);
  float voltage = val * (5000 / 1024.0);    // voltage at pin milliVolts
  float degreesC = (voltage / 10);  // voltage to C
  return degreesC;
}


float temp(byte ADDRESS){
	int val;
	if (ADDRESS == THIS_ADDRESS){
		return temp();
	}
	else{
		val = getReading(ADDRESS, 4);
	}
	float voltage = val * (5000 / 1024.0);    // voltage at pin milliVolts
    float degreesC = (voltage / 10) - 50;  // voltage to C
	return degreesC;
}


int analog(){
  int val = analogRead(analogPin);
  return val;  
}


int analog(byte ADDRESS){
	if (ADDRESS == THIS_ADDRESS){
		return analog();
	}
	else{
		return getReading(ADDRESS, 6);
	}
}


boolean touch(){
  int val = analogRead(touchPin);
  if (val < 1000){
	return true;
  }
  else {
    return false;
  }
}


boolean touch(byte ADDRESS){
	if (ADDRESS == THIS_ADDRESS){
		return touch();
	}
	else{
		int val = getReading(ADDRESS, 6);
		if (val < 1000){
			return true;
		}
		else {
			return false;
		}
	}
}


boolean button(boolean side){ // return 1 if pressed, 0 if not
  // false == left, true == right
  if (side == LEFT){
    return !digitalRead(buttonPinL);
  }
  else {
    return !digitalRead(buttonPinR);
  }
}


boolean button(byte ADDRESS, boolean side){ // return 1 if pressed, 0 if not
	// false == left, true == right
	if (ADDRESS == THIS_ADDRESS){
		return button(side);
	}
	else{
		int val = getReading(ADDRESS, 8);
		if (side == LEFT){
			return !highByte(val);
		}
		else {
			return !lowByte(val);
		}
	}
}


void speaker(unsigned int frequency, unsigned long duration){
	tone(speakerPin, frequency, duration);
}


void speaker(byte ADDRESS, unsigned int frequency, unsigned long duration){
	byte buf[4];
	
	if (ADDRESS == THIS_ADDRESS){
		tone(speakerPin, frequency, duration);
	}
	else{
		// convert from an unsigned long int to a 4-byte array
		buf[0] = (int)((duration >> 24) & 0xFF) ;
		buf[1] = (int)((duration >> 16) & 0xFF) ;
		buf[2] = (int)((duration >> 8) & 0XFF);
		buf[3] = (int)((duration & 0XFF));
		if(THAT_ADDRESS > 0){
			Wire.beginTransmission(THAT_ADDRESS);
			Wire.write(3);    // instuction
			Wire.write(highByte(frequency));  // led
			Wire.write(lowByte(frequency));// state
			Wire.write(buf[0]);
			Wire.write(buf[1]);
			Wire.write(buf[2]);
			Wire.write(buf[3]);
			Wire.endTransmission(); 
		}
	}
}


void LED(byte led, boolean state){
	if (led < 8){
		// Mask desired bit on to global _data variable
		bitWrite(data, led, state);

		// Now we'll actually send that data to the shift register.
		shiftOut(datapin, clockpin, MSBFIRST, data);

		// Toggle the state of the latchPin,
		digitalWrite(latchpin, HIGH);
		digitalWrite(latchpin, LOW);
	}
	else if (led < 16){
		if (false){
			Serial.print("Send");	
			Serial.print(led);
			Serial.print(" ");
			Serial.println(state);
		}	
		if(THAT_ADDRESS > 0){
			Wire.beginTransmission(THAT_ADDRESS);
			Wire.write(1);    // instuction
			Wire.write(15 - led);  // led
			Wire.write(state);// state
			Wire.endTransmission(); 
		}
  }
}
void LEDByte(byte data)
{
	// Mask desired bit on to global _data variable
		//bitWrite(data, led, HIGH);

		// Now we'll actually send that data to the shift register.
		shiftOut(datapin, clockpin, MSBFIRST, data);

		// Toggle the state of the latchPin,
		digitalWrite(latchpin, HIGH);
		digitalWrite(latchpin, LOW);
	
}


void receiveEvent(int howMany){
	if(DEBUG){
		Serial.print("\nReceive Event: howMany = ");
		Serial.println(howMany);
	}

	// get data
	byte buf[8];
	int counter = 0;
	for (int x=0; x<howMany; x++){
		buf[x] = Wire.read();
	}

	// show data
	if(DEBUG){
		for(int x=0; x<howMany; x++){
			Serial.print(buf[x], DEC);
			Serial.print(", ");
		}
		Serial.println();
	}

	byte instruction = buf[0];

	// POINTER INSTRUCTION?
	if (howMany == 1){
		pointer = instruction;
		if(false){
			Serial.print("pointer=");
			Serial.println(pointer);
		}
	}	
	
	// LED INSTRUCTION
	else if (instruction == 1 && howMany == 3){
		if(DEBUG){Serial.println("LED command");}
		LED(buf[1], buf[2]);
	}	
	
	
	else if (instruction == 3 && howMany == 7){
		unsigned int frequency;
		unsigned long duration;	
		frequency = word(buf[1], buf[2]);
		duration  = ((unsigned long) buf[3]) << 24;
		duration |= ((unsigned long) buf[4]) << 16;
		duration |= ((unsigned long) buf[5]) << 8;
		duration |= ((unsigned long) buf[6]);
		speaker(frequency, duration);
	}
	
	else if (instruction == 4 && howMany == 2){
		byte speed = buf[1];
		analogWrite(motorPin, speed);
	}	
	
}


void requestEvent(){ // get analog reading and return MSB, LSB
	int val;
	if(DEBUG){
		Serial.print("p=");
		Serial.println(pointer);}
	if(pointer == 0){				// POT
		val = analogRead(trimPin);
	}
	else if (pointer == 2){			// LIGHT
		val = analogRead(lightPin);
	}
	else if (pointer == 4){			// TEMP
		val = analogRead(tempPin);
	}
	else if (pointer == 6){			// ANALOG
		val = analogRead(analogPin);
	}
	else if (pointer == 8){			// Buttons
		byte valL = digitalRead(buttonPinL);
		byte valR = digitalRead(buttonPinR);
		val = word(valL, valR);
	}	
	
	if(pointer % 2){ 				// Odd pointer, return LSB
		Wire.write(LSB);
	}
	else{
		MSB = highByte(val);
		LSB = lowByte(val);
		Wire.write(MSB);
		pointer++;	
	}
}

