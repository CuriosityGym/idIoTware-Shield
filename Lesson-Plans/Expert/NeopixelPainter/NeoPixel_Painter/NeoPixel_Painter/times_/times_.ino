/* 
 Keyboard Button test
 
 For the Arduino Leonardo and Micro.
 
 Sends a text string when a button is pressed.
 
 The circuit:
 * pushbutton attached from pin 2 to +5V
 * 10-kilohm resistor attached from pin 4 to ground
 
 created 24 Oct 2011
 modified 27 Mar 2012
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/KeyboardButton
 */

const int buttonPin = 4;          // input pin for pushbutton
int previousButtonState = HIGH;   // for checking the state of a pushButton
int counter = 0;                  // button push counter

void setup() {
  // make the pushButton pin an input:
  pinMode(buttonPin, INPUT);
  // initialize control over the keyboard:
  Keyboard.begin();
}times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 timthe button 0 times.
Youtpressed the button 0 times.
You pressed the button 0 times.
You pressed the buu on sseimthe buttopressed the button 0 ed the buttopressed the button 0 times.
You pressed the button 0 times.
You pressed the butt nressed th
You pressed the button 0 times.
You pressedmes.ssed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
You pressed the button 0 times.
Yo



void loop() {
  
    Keyboard.print("You pressed the button ");
    Keyboard.print(counter); 
    Keyboard.println(" times.");
  
  
}

