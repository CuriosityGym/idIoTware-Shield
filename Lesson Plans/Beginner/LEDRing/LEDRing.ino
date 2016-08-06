/*
* idIoTware Shield LED Ring Demo
* This Arduino Sketch can control the speed of the LED's 1 to 12 by using the potentiometer on the Shield.
* Turning the potentiometer will enable the LED's to blink faster and slower
* 
*/
const byte led_Pin_Count = 12; // // the number of leds
int potentiometerPin = A2;   // The Blue Potentiometer is connected to Analog pin 2
int minSpeed = 500; // The maximimum value of time in milliseconds to be set between two adjacent led's glowing.
int maxSpeed = 50;  // The minimum value of time in milliseconds to be set between two adjacent led's glowing.

void setup()
{
for(int i = 1; i <= led_Pin_Count; i++)   // set all LEDs to output
  {
    pinMode(i, OUTPUT); //Set Pins on Arduino to be outputs
  }  
}

void loop()
{       
  int userSpeed = potentiometerValue();   //this variable will store the Potentiometer value
  //map the Potentiometer value to minimum and maximum speed You can read about the map function here
  //https://www.arduino.cc/en/Reference/Map

  // The map function takes an input value, which has a range 0-1023, and calculates its corresponding value between minSpeed and maxSpeed
  int blinkSpeed = map(userSpeed, 0, 1023, minSpeed, maxSpeed);  
  
  for(int i = 1; i <=led_Pin_Count; i++) 
  {
    digitalWrite(i, HIGH); //Turn on the LED
    delay(blinkSpeed); //Wait for the calculated delay
    digitalWrite(i, LOW);   //Turn of the LED 
  }
}

//function to calculate potentiometer value
int potentiometerValue()
{
  int val = analogRead(potentiometerPin); //Read the analog position of the potentiometer
  return val; return it
} 
