
# Police Siren with the idIoTware Shield

### Hackster project
https://www.hackster.io/30210/police-siren-bf8169?ref=channel&ref_id=29476_trending___&offset=0

### Video
https://youtu.be/00wCUanNaoU

### Project Description

In this project, I am going to demonstrate how to make your own police siren alarm with the help of the idIoTware shield.

You will need following things to get started. 
An Arduino Uno.
An idIoTware shield. 
A USB cable to connect Arduino to Computer.
A 2 pin Jumper to connect the Buzzer to the Arduino A1 pin.
A jumper that connects CLR to D6 for Color LED.


The buzzer is already connected on the shield, but it has to be enabled using the 3 pin  Selection Jumper SEL 1.


We need to Connect the “BUZ” pin to the center pin on SEL1 3 pin header, effectively connecting the Buzzer to the A1 pin.


In method police_siren() the tone for the siren is mentioned.Use this function in the loop to run the siren over and over again.


For more reference on tone function please visit the link in the description.  


Once we upload the code in arduino, we are going to hear the police siren, along with the RGB Color LED showing a Red and Blue blinking light.


You can now make your own program that play siren when someone unauthorized comes at your house.This will alert the neighbours about activity inside your home.  


You can also use the buzzer to alert you when the temperature increases beyond a certain limit, as measured by the integrated temperature sensor on the idIoTware shield. We will learn how to measure temperature in a later video. 


Another use for the buzzer is to generate any set of tones like nursery rhymes or the happy birthday song. 


