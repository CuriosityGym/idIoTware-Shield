#idIoTware Shield LED Ring Demo

This Arduino program demonstrates the ring of LED's on the shield to light up one after the other.

[![LED Ring Demo](http://img.youtube.com/vi/NEE72VXsWSE/0.jpg)](https://www.youtube.com/watch?v=NEE72VXsWSE)

#Hardware Required
1. idIoTware Shield
2. Arduino Uno.
3. USB cable to connect arduino  to computer
4. One jumper to enable the LED Ring

The following patterns are already defined in the idIoTware Arduino library.
a) Sweep,
b) Half Circle
c) A Curtain
d) Fast and Slow Pattern.


The LED’s on the ring are connected from Digital Pin 1 to Digital pin 12 of the Arduino.
First of all we need to include the idiotware library to our sketch.
We need to initialize this leds in void setup() function of arduino sketch.
The void loop() runs over and over again, repeatedly till power is removed. 
We will use the function for example the sweep(1). 
The “1” means how many repetitions you wish to do for that animation.
This value can only be whole numbers, and not with decimal points. 


In a similar manner, you can also use the other functions to mix and match animations on the LED Ring..


Additionally,you can use touch pad or potentiometer as the input to control the 12 led’s.




You can now make your own program and create patterns on the LED’s and put this up on your Christmas tree
or any other decorations to bring that interactive look to your project. 
No more connecting leds to breadboard and messy wiring.





