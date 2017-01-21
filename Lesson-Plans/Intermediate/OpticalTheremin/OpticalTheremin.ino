/***********************************************************************

  The theremin is a musical instrument that makes sounds as you wave your hand above it.
  The light Dependent resistor or LDR is a light controlled variable resistor,
  the resistance of a LDR decreases with increasing light intensity,
  the resistance of LDR increases when the light intensity is decreasing,
  However on the idIoTware shield, the LDR is wired such that with a higher Light intensity
  we get a higher Analog Voltage on Pin A3.
  In this example we are going to use LDR sensor to make sounds.
  When we wave our hand the light intensity falls on Light Dependent Resistor goes low .
  This data is then captured by arduino through a voltage divider.
  We set the frequency according to the light value by using  map and tone functions in arduino to generate specific frequency .

*/
int ldrPin = A3;
const int buzzerPin = A1;

// variable declaration
int value;  // sensor value
int low;    // low sensor value calibration
int high;   // high sensor value calibration
// LED pin alert calibration
int led = 13;

void setup()
{
  pinMode (buzzerPin, OUTPUT);
  pinMode (led, OUTPUT);
  // turn LED on
  digitalWrite(led, HIGH);
  // calibration for the first 3 seconds after program runs
  while (millis() < 3000)
  {
    // record the maximum sensor value
    value = analogRead(ldrPin);
    if (value > high)
    {
      high = value;
    }
    // record the minimum sensor value
    if (value < low)
    {
      low = value;
    }
  }
  // turn LED off
  digitalWrite (led, LOW);
}

void loop()
{
  //read the input from A0 and store it in a variable
  value = light();
  // map the sensor values to a frequency range 50 Hz - 4000 Hz
  int pitch = map(value, low, high, 100, 4000);
  // play the tone for 20 ms on buzzer pin 8
  speaker(pitch, 20);
  // wait for a moment
  delay(10);
}

//function to calculate potentiometer value
int light()
{
  int val = analogRead(ldrPin);
  return val;
}

void speaker(unsigned int frequency, unsigned long duration)
{
  tone(buzzerPin, frequency, duration);
}

