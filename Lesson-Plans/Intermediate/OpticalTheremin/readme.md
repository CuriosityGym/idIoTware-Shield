#Optical Theremin
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