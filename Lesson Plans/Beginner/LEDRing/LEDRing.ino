/*This Arduino program demonstrates the ring of LED's on the shield to light up one after the other.
 * Learners can change the variable blinkSpeed to see how a large value or a small value affects the timing of the lights going one and off, one after the other.
 * 
*/

const byte LEDCount = 12; // // the number of leds
int blinkSpeed=50;// The time in milliseconds the LED's are going to stay on.
//int potentiometerPin = A2;   // The Blue Potentiometer is connected to Analog pin 2
//int minSpeed = 500; // The maximimum value of time in milliseconds to be set between two adjacent led's glowing.
//int maxSpeed = 10;  // The minimum value of time in milliseconds to be set between two adjacent led's glowing.

void setup()
{
for(int i = 1; i <= LEDCount; i++)   // set all LEDs to output
  {
    pinMode(i, OUTPUT); //Set Pins on Arduino to be outputs
  }  
}

void loop()
{       
  //int userSpeed = potentiometerValue();   //this variable will store the Potentiometer value
  //map the Potentiometer value to minimum and maximum speed You can read about the map function here
  //https://www.arduino.cc/en/Reference/Map

  // The map function takes an input value, which has a range 0-1023, and calculates its corresponding value between minSpeed and maxSpeed
 // int blinkSpeed = map(userSpeed, 0, 1023, minSpeed, maxSpeed);  
  
  for(int i = 1; i <=LEDCount; i++) 
  {
    digitalWrite(i, HIGH); //Turn on the LED
    delay(blinkSpeed); //Keep the LED turned on for a duration of "blinkSpeed" milliseconds. 
    digitalWrite(i, LOW);   //Turn of the LED 
  }
}

//function to calculate potentiometer value
/*int potentiometerValue()
{
  int val = analogRead(potentiometerPin); //Read the analog position of the potentiometer
  return val; //return it
} */
