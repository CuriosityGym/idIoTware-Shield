**IOT OnAir**
-------------



This is a device that could be used to notify any specific content on google calendar with idIoTware Shield.

The event from the Google calendar notifies the Maker channel on the IFTTT server. 
Once the maker channel is triggered, it will then send a ‘Get request’ to the blynk server.

For example: http://blynk-cloud.com/yourAPIkey/update/V0?value=250?pin=V0&value=0&pin=V0&value=0

Note: here "value" is the whole number that is used to set intensity of red color on idIoTware shield from 0 to 255.

So what is IFTTT?
-----------------




IFTTT is a free web-based service that allows users to create chains of simple conditional statements, called "recipes", which are triggered based on changes to other web services such as Gmail, Facebook, Instagram, Twitter and many more.

IFTTT is an abbreviation of "If This Then That".


User can use the maker channel to connect there hardware with other services and trigger them according to the recipes.


For example: The event from the google calendar notifies the maker channel on the IFTTT server.
Once the maker channel is triggered, it will then send a ‘Get request’ to the blynk server.

Configuring IFTTT and Blynk APP
-------------------------------





1. Go to www.ifttt.com

2. Sign in the account

3. Create new applet

4. Select Google Calendar.


5. For this example we choose event from search starts. NOTE:You can choose any of the three.

6. Write the keyword you want to trigger from Google Calendar.

7. Add Maker channel

8. Select Make a web request

9. You will need to add url for blynk server and change content type to text/plain
10. Create new project and email the token key


In your Arduino program, you need to copy the Token key generated in the blynk app and also need to write your ssid and password for wifi . You are now ready to upload the code. You will see your ON Air light up when the google calendar event is called.

1. Configuring Arduino IDE and idIoTware shield

2. Connect One jumper to CHPD for enabling esp8266

3. Connect One jumper to CLR means color that is RGB LED and D6. 
