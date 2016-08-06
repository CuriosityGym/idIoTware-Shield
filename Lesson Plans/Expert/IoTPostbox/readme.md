#IoT PostBox
    
#####**Attention: Please install all libraries from our Github Repository to enable the example to run.**

In this example we are using

1) An ESP8266-01 Module with ESP-Link Firmware

2) An idIoTWare Shield

3) A Arduino Uno Board.


The Light Sensor/Light Dependent Resistor(LDR) on IdIoTWare shield is used as sensor to detect the fall of the letter in a letterbox.
The RGB WS2812 Led (addressable RGB LED) on idIoTWare shield is On (WHITE Color) and reflects light on LDR.

As soon as new letter dropped in postbox light incident on LDR changes due to reflection and an IFTTT event gets triggered.
Here we are using IFTTT Recipe that triggers the Maker Channel to send an email. 

#####What is IFTTT? 

IFTTT is an abbreviation of "If This Then That"

IFTTT is a free web-based service that allows users to create chains of simple conditional statements,
called "recipes", which are triggered based on changes to other web services such as Gmail, Facebook,
Instagram, and many more.


Create account on IFTTT and create your recipe in www.ifttt.com

