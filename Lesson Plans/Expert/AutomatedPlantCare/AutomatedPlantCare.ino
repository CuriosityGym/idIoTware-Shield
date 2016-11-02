/*
idIoTWare Shield Automated Plant Care
Attention: Please install all libraries from our Github Repository to enable this example to run.

In this example we are using ESP8266-01 Wifi Module with esp-link firmware, Soil moisture sensor to sense moisture in soil,
walter level sensor to detect water level and motor to water the plant. If plant's soil is dry then moisture sensor will give 
HIGH input to arduino then arduino turns ON the motor. We have used mosfet to turn ON and turn OFF the motor. Arduino logs the 
data on thingspeak and sends mail to the user.

ThingSpeak is an open source Internet of Things (IoT) application and API to store and retrieve data from things using the HTTP
protocol over the Internet or via a Local Area Network. ThingSpeak enables the creation of sensor logging applications, location 
tracking applications, and a social network of things with status updates.
To log the data to thingspeak, you need to create thingspeak account and then create channel and use that channel's API Key in this code.

Here we are using IFTTT to trigger an event.
    
    IFTTT is a free web-based service that allows users to create chains of simple conditional statements,
    called "recipes", which are triggered based on changes to other web services such as Gmail, Facebook,
    Instagram, and many more.IFTTT is an abbreviation of "If This Then That"
    Create account on IFTTT and create your recipe.
    
    We are using Maker and Gmail channel to trigger an event.
    
    If plant is watered, Arduino will send POST request to maker channel.  
    if Maker then Gmail
    If Maker Event "Plant_watered", then send an email from "abcd@gmail.com" 
    
note: replace API key for thingspeak channel and maker channel    
*/


#include <ELClient.h>
#include <ELClientRest.h>

char buff[128];
String sendData = "";
boolean sendMail = true;
long previousMillis = 0; 
long interval = 60000;
const byte waterLevelSensor = 7;// digital pin 7 has a water level sensor attached to it
const byte moistureSensor = 5;  // digital pin 5 has a water level sensor attached to it
const byte motorPin = 3;   // digital pin 3 has a motor attached to it

// replace with your channel's thingspeak API key
String API_KEY = "XXXXXXXXXXXXXX";
// Initialize a connection to esp-link using the normal hardware serial port both for
// SLIP and for debug messages.
ELClient esp(&Serial, &Serial);

// Initialize a REST client on the connection to esp-link
ELClientRest rest(&esp);

boolean wifiConnected = false;

// Callback made from esp-link to notify of wifi status changes
// Here we print something out and set a global flag
void wifiCb(void *response) 
    {
      ELClientResponse *res = (ELClientResponse*)response;
      if (res->argc() == 1) 
         {
           uint8_t status;
           res->popArg(&status, 1);

           if(status == STATION_GOT_IP) 
             {
               Serial.println("WIFI CONNECTED");  //Wifi gets connected at this place
               wifiConnected = true;
             } 
           else 
             {
               Serial.print("WIFI NOT READY: ");//Wifi not connected,check connection
               Serial.println(status);
               wifiConnected = false;
             }
         }
    }




void setup()
    {
      Serial.begin(9600);   // the baud rate here needs to match the esp-link config
      Serial.println("EL-Client starting!");

      // Sync-up with esp-link, this is required at the start of any sketch and initializes the
      // callbacks to the wifi status change callback. The callback gets called with the initial
      // status right after Sync() below completes.
      esp.wifiCb.attach(wifiCb); // wifi status change callback, optional (delete if not desired)
      bool ok;
      do 
       {
         ok = esp.Sync();  // sync up with esp-link, blocks for up to 2 seconds
         if (!ok) Serial.println("EL-Client sync failed!");
       } while(!ok);
      Serial.println("EL-Client synced!");

      // Get immediate wifi status info for demo purposes. This is not normally used because the
      // wifi status callback registered above gets called immediately. 
      esp.GetWifiStatus();
      ELClientPacket *packet;
      if((packet=esp.WaitReturn()) != NULL) 
        {
          Serial.print("Wifi status: ");
          Serial.println(packet->value);
        }

      pinMode(moistureSensor, INPUT);
      pinMode(waterLevelSensor, INPUT);
      pinMode(motorPin, OUTPUT);
      digitalWrite(motorPin, LOW);
     }
     
// the loop routine runs over and over again forever:
void loop()
    { 
      unsigned long currentMillis = millis();
      digitalWrite(motorPin, LOW);  // motor off
      
      // read the input pins:
      int moistureSensorState = digitalRead(moistureSensor);
      int waterLevelSensorState = digitalRead(waterLevelSensor); 
      delay(1);        // delay in between reads for stability
      
      if(waterLevelSensorState == HIGH && sendMail == true)  // if water level is low, send mail to user
        { 
          digitalWrite(motorPin, LOW);  // motor off
      
          // Set up the REST client to talk to www.maker.ifttt.com, this doesn't connect to that server,
          // it just sets-up stuff on the esp-link side
          int err = rest.begin("maker.ifttt.com");
          if(err != 0) 
            {
              Serial.print("REST begin failed: ");
              Serial.println(err);
              while(1) ;
            }
          Serial.println("EL-REST ready");
          delay(1000);
          //replace with your maker channel's API key
          sprintf(buff, "/trigger/water_level_indicator/with/key/XXXXXXXXXXXXXXXX"); // send mail to user
          logToMaker();  //Log to Maker using commands under void LogToMaker()
          // print to the serial port too:              
          Serial.print("Mail sent!!");
          delay(1000);
          sendMail = false;   // make send_Mail variable false
          previousMillis = currentMillis;
        }
      
       // if water level is low then it will send mail after every 2 seconds until the tank filled with water. 
       // to avoid this send mail for water level indiaction after particular time 
      if(currentMillis - previousMillis > interval) 
        {
          sendMail = true;
        }
      
      if(moistureSensorState == HIGH)   // if soil is dry then water the plant
        {
          char moisture[6];
          dtostrf(moistureSensorState, 4, 2, moisture);
          // Set up the REST client to talk to www.thingspeak.com, this doesn't connect to that server,
          // it just sets-up stuff on the esp-link side
          int err = rest.begin("api.thingspeak.com");
          if(err != 0) 
            {
              Serial.print("REST begin failed: ");
              Serial.println(err);
              while(1) ;
            }
          Serial.println("EL-REST ready");
          sprintf(buff, "/update?api_key=%s&field1=%s",API_KEY.c_str(), moisture);  // log values to thingspeak
          
          logToThingspeak();  //Log to thingspeak using commands under void LogToThingspeak()
          
          for(byte i=0; i<1; i++)  // to water the plant, start the motor
             {
               motorStart();
               delay(500);
             }
        
          // Set up the REST client to talk to www.maker.ifttt.com, this doesn't connect to that server,
          // it just sets-up stuff on the esp-link side
          int err = rest.begin("maker.ifttt.com");
          if(err != 0) 
            {
              Serial.print("REST begin failed: ");
              Serial.println(err);
              while(1) ;
            }
          Serial.println("EL-REST ready");
          delay(1000);
          //replace with your maker channel's API key
          sprintf(buff, "/trigger/Plant_Watered/with/key/XXXXXXXXXXXXXXX ");// send mail to user
          logToMaker();  //Log to Maker using commands under void LogToMaker()
          // print to the serial port too:              
          Serial.print("Plant Watered");
          delay(20000);
        }
      
      
    }   
   
 void logToThingspeak()
    {
           // process any callbacks coming from esp_link
      esp.Process();

     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {  Serial.println("wifi connected!!");
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[266];
          uint16_t code = rest.waitResponse(response, 266);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
             Serial.println("ARDUINO: GET successful:");
             Serial.println(response);
            } 
          else 
            {
             Serial.print("ARDUINO: GET failed: ");
             Serial.println(code);
            }
          
        }
        
    } 

//function to send POST request to Maker channel    
void logToMaker()
    {
           // process any callbacks coming from esp_link
      esp.Process();
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {  Serial.println("wifi connected!!");
          // Request  from the previously set-up server
          rest.get((const char*)buff);

          char response[266];
          uint16_t code = rest.waitResponse(response, 266);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
             Serial.println("ARDUINO: GET successful:");
             Serial.println(response);
            } 
          else 
            {
             Serial.print("ARDUINO: GET failed: ");
             Serial.println(code);
            }
          
        }
        
    }    
 
 // function to start motor
 void motorStart()
     {
       // increase speed of motor in from min to max in increments of 5 points:
       for(int initialValue = 0 ; initialValue <= 255; initialValue +=5)
          { 
            // sets the value (range from 0 to 255):
            analogWrite(motorPin, initialValue); 
            delay(50);                            
          } 

       // decrease speed of motor from max to min in increments of 5 points:
       for(int initialValue = 255 ; initialValue >= 0; initialValue -=5)
          { 
            // sets the value (range from 0 to 255):
            analogWrite(motorPin, initialValue);             
            delay(50);                            
          } 
      } 

    
