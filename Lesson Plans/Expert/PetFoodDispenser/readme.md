# **idIoTWare Shield Dog Food Feeder**
#####  **Attention: Please install all libraries from our Github Repository to enable this example to run.**
    
In this example we are using ESP8266-01 Wifi Module  and a Servo motor to dispense food from 3d printed food jar.

Blynk is a platform with iOS and Android apps to control Arduino, Raspberry Pi and the likes over the Internet. 
You can easily build graphic interfaces for all your projects by simply dragging and dropping widgets.

Downloads, docs, tutorials: http://www.blynk.cc
  Blynk community:            http://community.blynk.cc 

To send data from Blynk to arduino or get data from arduino to blynk app you need to install Blynk app on your 
smartphone, create account to log in to the app then you will get a unique token number then use that token number in 
this code to create your own project.

In this example we have used ESP8266_Lib.h and BlynkSimpleShieldEsp8266.h  library to make communication between arduino
and Blynk app.We have used Servo.h library to control servo motor. In this code we have used vitual pin(v1) to receive data 
from blynk app. oN Blynk app we have added a button icon to send ON and OFF signal to arduino. If you press button then servo
will rotate 60 degree in clockwise direction and food will get dispensed for 5 seconds and after thet servo will come back to 
initial positon. You can change rotation angle of servo and time for which you want to dispense food. 

#####**Setup:**

1.install all libraries from our Github Repository to enable this example to run.                                 
2.create account on BLYNK and use your Auth Token number in code.                               


#####**Precautions:**

1.Enter correct Auth Token number.                                                                                 
2.Virtual pin number in code and BLYNK app must be same.                                                                 

#####**Images:** put link here
#####**Video:** put link here
