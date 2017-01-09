#IoT PostBox
    
In this project, I am going to demonstrate how to build your own IoT aware Postbox with the help of the idIoTware shield in a few easy steps. This postbox alerts you about new envelopes dropped inside a postbox by the mailman. 

### Hackster Project
https://www.hackster.io/30290/smart-iot-postbox-3287f8?ref=user&ref_id=11008&offset=1

### Video 

https://www.youtube.com/watch?v=VOWjkQzXQHM




You will need the following things to get started. 
An Arduino UNO
idIoTware shield. 
USB cable to connect arduino  to computer.
Esp 8266 - 01, programmed with ESP-link firmware
One jumper to enable the Chpd pin.
One Postbox
External 9v 1A Power adapter.   



The LDR ( Light Dependent Resistor)  on IdIoTWare shield is used as sensor to detect a letter being dropped into the letterbox. The on board WS2812 Led (addressable RGB LED) on IdIoTWare shield is continuously ON (WHITE Color) which acts as a light source, and reflects light onto the LDR. As soon as a letter is dropped into the postbox, there is an interruption in light -  the light intensity on LDR changes due to reflection. The Arduino continuously monitors the change in the value of the LDR and when it notices a change in value, it sends an email.  Here we are using IFTTT to send a pre configured email. 


###So what is IFTTT?
Introduction Video to IFTTT https://www.youtube.com/watch?v=CEAVFU3ELcI

IFTTT is an abbreviation of "If This Then That"

IFTTT is a free web-based service that allows users to create chains of simple conditional statements, called "recipes", which are triggered based on changes to other web services such as Gmail, Facebook, Instagram, and many more.

You need to  create an account on IFTTT and create your recipe.

You need to make the IFTTT account. click on the link below https://ifttt.com/join and sign up the account.


We are using Maker and Gmail channel to trigger an event.
If there is new letter in postbox, Arduino will send POST request to Maker channel.
if Maker then Gmail’
If Maker Event "New Letter",
then send an email from "abcd@gmail.com".




Connect the esp8266 01 module to ESP-01 header on the top right, such that the antenna is facing outside. ( See Image) We need to Connect jumper to the chpd pin header for the esp to work correctly. 


Once we upload the code in arduino, place the Arduino with double sided sticky tape inside the postbox. Make sure the whole bottom of the arduino board is insulated from the metal areas of the postbox. Post the letter in the box, and you will see the message” new Letter!! “ in serial monitor and when you check your mail you will find IFTTT message saying you got a letter!!


 


