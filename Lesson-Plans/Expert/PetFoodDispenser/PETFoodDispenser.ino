 /*
    idIoTWare Shield Dog Food Feeder
    Attention: Please install all libraries from our Github Repository to enable this example to run.
    
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

*/


#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <Servo.h>

Servo servo;

int delayValue = 5000;   

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "xxxxxxxxxxxxxxxxxxxxxx"; //replace it with your token number

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "abcd";    
char pass[] = "**********";

// Hardware Serial on Mega, Leonardo, Micro...
#define EspSerial Serial

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial debugSerial(10, 9); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);

void setup()
    { 
      servo.attach(9);
      servo.write(0);
      // Set console baud rate
      debugSerial.begin(115200);
      delay(10);
      // Set ESP8266 baud rate
      EspSerial.begin(ESP8266_BAUD);
      delay(10);

      Blynk.begin(auth, wifi, ssid, pass);
      
       
    }



void loop()
    {
      Blynk.run();
    }
//create virtual pin 1 and read value from that pin
BLYNK_WRITE(V1)
     {
       int value = param.asInt();
       if(value == 1)  //if button pressed on blynk app, start the servo
         {
           servo.write(60);  // tell servo to go to particular position   
           delay(delayValue); // wait for particular time       
           servo.write(0);    //rotate servo to initial position    
         }     
       
     }    
        
    


