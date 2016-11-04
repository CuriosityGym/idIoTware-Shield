#**idIoTWare Shield Hydroponics**
#####**Attention: Please install all libraries from our Github Repository to enable this example to run.**

In this example we are using ESP8266-01 Wifi Module with esp-link firmware, Soil moisture sensor to sense moisture in soil, walter level sensor to detect water level and motor to water the plant. If plant's soil is dry then moisture sensor will give HIGH input to arduino then arduino turns ON the motor. We have used mosfet to turn ON and turn OFF the motor. Arduino logs the data on thingspeak and sends mail to the user.                                                    

ThingSpeak is an open source Internet of Things (IoT) application and API to store and retrieve data from things using the HTTP protocol over the Internet or via a Local Area Network. ThingSpeak enables the creation of sensor logging applications, location tracking applications, and a social network of things with status updates. To log the data to thingspeak, you need to create thingspeak account and then create channel and use that channel's API Key in this code.                

Here we are using IFTTT to trigger an event.                                                                             
IFTTT is a free web-based service that allows users to create chains of simple conditional statements,
called "recipes", which are triggered based on changes to other web services such as Gmail, Facebook,
Instagram, and many more.IFTTT is an abbreviation of "If This Then That"
Create account on IFTTT and create your recipe.

We are using Maker and Gmail channel to trigger an event.

If plant is watered, Arduino will send POST request to maker channel.  
if Maker then Gmail
If Maker Event "Plant_watered", then send an email from "abcd@gmail.com"  
      

#####**Setup:**
1.create account on thingspeak and channel                                         
2.create account on IFTTT, connect to maker channel and make recipe.                                                    
3.replace API key for thingspek channel and maker channel                                                     
4.install all libraries from our Github Repository to enable this example to run.                                         
5.connect sensors to proper input pins and caliberate the sensors for perfect reading.



#####**Precautions:**
1.Use mosfet or relay module to drive motor.                                                                             
2.keep sensor properly in soil to get proper readings


#####**Images:**  put link here


#####**Video:**  put link here
