/*

  A potentiometer, or "pot" for short, is a control knob.
  It's the same type of control you'd use to change volume,
  dim a lamp, etc. A potentiometer changes resistance as it
  is turned. By using it as a "voltage divider", the Arduino
  can sense the position of the knob, and use that value to
  control whatever you wish.

  In this example we are going to Turn ON and OFF sequence of 12 LEDs
  depending upon the position of the knob of potentiometer.
  As we turn potentiometer from initial position to final position,
  12 LEDs will glow one by one depending to value of potentiometer.
  If we turn knob in reverse direction then LEDs will turn OFF one by
  one depending upon the value of potentiometer

*/


int potentiometer = A2;    // The potentiometer is connected to analog pin 2

int led_Pins[] = {1,2,3,4,5,6,7,8,9,10,11,12};   //an array of pin numbers to which LEDs are attached

int pin_Count = 12;   // the number of pins (i.e. the length of the array)

void setup()
{
    // put your setup code here, to run once:

    // the array elements are numbered from 0 to (pinCount - 1).
    // use a for loop to initialize each pin as an output:
    for ( int thisPin = 0; thisPin < pin_Count; thisPin++) // set all LEDs to output
    {
        pinMode(led_Pins[thisPin],OUTPUT);
    }
}

void loop()
{
    // put your main code here, to run repeatedly:

    int potentiometer_Value;   //this variable will store the potentometer value
    potentiometer_Value = analogRead(potentiometer); // reads the value of the potentiometer (value between 0 and 1023)
    potentiometer_Value = map(potentiometer_Value, 0, 1023, 0, 11); // map() function Re-maps a number from one range to another
    // maps 10 bit potentiometer value (0 to 1023) to 0 to 11
//  for loop will iterate from 0 to 11 and will turn ON and OFF LEDs according to value of potentiometer
    for (int i = 0; i<= potentiometer_Value;i++)
    {
        digitalWrite(led_Pins[i],HIGH);
    }

    for (int j = 11; j>=potentiometer_Value;j--)
    {
        digitalWrite(led_Pins[j],LOW);
    }
}

