/*
              Weather Gadget
   In this example we are using ESP8266 wifi module and OLED display
 to display weather conditinos of particular city from WeatherDataApi.
              
 WeatherDataApi: Heroku Deployment Application that reduces data 
                 downloaded for OpenWeatherMap Api
  
 To get data from OpenWeatherMap Api, you need to create openweathermap 
 account and then generate API key and use that API Key in this code.                
 In this example we have used ELClient.h library to get data from WeatherDataApi. 
 Here we are sending request for location,current temprature,humnidity and 
 weather descrition. This code allows you to detect your location automatically
 by IP address or to get weather conditions of particular city just change  the 
 city ID in the following code and change autoDetectLocation to false. OLED is 
 used to display this information.
 We are sending request to "idiotware.herokuapp.com" after every one hour to get
 weather information.To change this interval, change value of refreshRate. 
               
*/



#include "U8glib.h"
#include <ELClient.h>
#include <ELClientRest.h>
#include "icons.h"    // Bitmap weather icons 
#define autoDetectLocation true
#define refreshRate 15   //send request after ever 60 minutes(one hour) 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI 
#define tempPin A0  // LM35 is connected to A0 pin
#define pass "Arduino Get Succesful"
#define fail "Arduno Get Failed"
char buff[90];
//variables to store recieved data
float temperature;
int humidity;
String location = "";
String weatherDescription = "";
int tempInC;
//boolean firstRequest = true;
// replace with your channel's thingspeak API key
String API_KEY = "15373f8c0b06b6e66e6372db065c4e46";
String CityID = "1275339"; //Mumbai, India

// varibles for cursor positions
//int yPos = 0;
//int xPos = 0;
//int zPos = 0;
int tempInCelcius = 0;
//char buff[90];
String sendData = "";
unsigned long  elapsedTime;
unsigned long time=0;
unsigned long samplingTime = 180;   //this variable is interval(in Seconds) at which you want to log the data to thingspeak.
unsigned long duration = 60;     //this variable is duration(in Minutes) which is the total time for which you want to log data.
unsigned long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long interval1 = 5000;           // interval at which to blink (milliseconds)
unsigned long interval2 = 10000;
unsigned long interval3 = 15000;
boolean frame1 = true;
boolean frame2 = true;
boolean frame3 = true;

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
               Serial.println("WiFi Connected");  //Wifi gets connected at this place
               u8g.firstPage();
               do 
                 {  
                  u8g.drawXBMP( 110, 0, wifi_logo_width, wifi_logo_height,wifi_logo_bits);
                 // u8g.setFont(u8g_font_timB14);  
                u8g.drawXBMP( 15, 15, cg_logo_width, cg_logo_height,cg_logo_bits);
                u8g.drawStr(45,27,"curiosity"); u8g.drawStr(45,40,"gym"); 
                 } while( u8g.nextPage() );
               wifiConnected = true;
             } 
           else 
             {  
               u8g.firstPage();
               do 
                 {  
                  u8g.drawXBMP( 110, 0, no_wifi_logo_width, no_wifi_logo_height,no_wifi_logo_bits);
                 // u8g.setFont(u8g_font_timB14);  
                u8g.drawXBMP( 15, 15, cg_logo_width, cg_logo_height,cg_logo_bits);
                u8g.drawStr(45,27,"curiosity"); u8g.drawStr(45,40,"gym"); 
                 } while( u8g.nextPage() );
               Serial.print("WIFI NOT READY: "); //Wifi not connected,check connection
               Serial.println(status);
               wifiConnected = false;
             }
         }
    }

void setup() 
    {
      Serial.begin(9600);   // the baud rate here needs to match the esp-link config
       u8g.setFont(u8g_font_timB12);
      u8g.setColorIndex(1); // Instructs the display to draw with a pixel on. 
      u8g.firstPage();
             do 
               {  
                //u8g.setFont(u8g_font_timB14);  
                u8g.drawXBMP( 15, 15, cg_logo_width, cg_logo_height,cg_logo_bits);
                u8g.drawStr(45,27,"curiosity"); u8g.drawStr(45,40,"gym"); 
               } while( u8g.nextPage() );
           delay(3000);
      Serial.println("EL-Client starting!");
      
      
      // Sync-up with esp-link, this is required at the start of any sketch and initializes the
      // callbacks to the wifi status change callback. The callback gets called with the initial
      // status right after Sync() below completes.
      esp.wifiCb.attach(wifiCb); // wifi status change callback, optional (delete if not desired)
      bool ok;
      do 
       {
         ok = esp.Sync();      // sync up with esp-link, blocks for up to 2 seconds
         if (!ok) 
             u8g.firstPage();
             do 
               {  
                u8g.drawStr(0,15,"sync failed!");
              //  u8g.drawXBMP( 110, 0, wifi_logo_width, wifi_logo_height,wifi_logo_bits);
               } while( u8g.nextPage() );
             Serial.println("EL-Client sync failed!");
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

      // Set up the REST client to talk to www.idiotware.herokuapp.com, this doesn't connect to that server,
      // it just sets-up stuff on the esp-link side
     
      Serial.println("EL-REST ready");  
      
      int err = rest.begin("idiotware.herokuapp.com");
      if(err != 0) 
        {
          Serial.print("REST begin failed: ");
                       u8g.firstPage();
             do 
               {  
                u8g.drawStr(0,15,"REST begin failed:");
               } while( u8g.nextPage() );
          Serial.println(err);
          while(1) ;
        }
      get_location();
      get_Temperature();
      get_Humidity();
      get_weatherDescription();
      duration = 2800 * 60;
    }

void loop() 
    { 
      elapsedTime = millis()/1000;   // this variable will keep track of elapsed time
      while(((millis()/1000)-elapsedTime) < 1);    // this loop will do nothing until a second has passed 
      time++;                                       //increment time after each second.

      weatherData();
      dataSamples();
  
      frameScroll();
  
    }      

int dataSamples()
   { 
     // here we are logging data at interval of 16 seconds for 30 mintutes, i.e, 112 samples.
     // if you want to log data for 2 hours then simply multiply 2 by 60 which will give 
     // you value of 120 minutes then change the varible duration to 120. 

      int reading1 = analogRead(tempPin); delay(10);
      int reading2 = analogRead(tempPin); delay(10);
      int reading3 = analogRead(tempPin); delay(10);
      int reading4 = analogRead(tempPin); delay(10);
      
      int averageReading = ((reading2 +  reading3 + reading4 )/ 3); 
      tempInCelcius = round(( 5.0 * averageReading * 100.0) / 1024.0);
      
      int light_value1 = analogRead(A3); delay(10);
      int light_value = analogRead(A3);delay(10);
     
      if((duration >= time) && (time % samplingTime == 0))
        { 
          int err = rest.begin("api.thingspeak.com");
          if(err != 0) 
            {
             Serial.print("REST begin failed: ");
             Serial.println(err);
             while(1) ;
            }
          Serial.println("EL-REST ready");
          char str_light[6]; 
          char str_temp[6];
          dtostrf(tempInCelcius, 4, 0, str_temp);
          dtostrf(light_value, 4, 0, str_light);
          sprintf(buff, "/update?api_key=8094ZFO33L6CS7YE&field1=%s&field2=%s", str_temp,str_light);
          // uncomment following line to get temperature values in Farehniet
          //tempInFarenheit = ((tempC*9)/5) + 32;            //convert celcius to farenheit
          logToThingspeak();  //Log to thingspeak using commands under void LogToThingspeak()
          // print to the serial port too:              
          //Serial.print("Temperature: ");
          //Serial.print(tempInCelcius);
          //Serial.print(char(176)); 
          //Serial.println("C"); 
          
          
                
        }
      
    
   } 
 
    
boolean logToThingspeak()
    {
           // process any callbacks coming from esp_link
      esp.Process();

     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {  Serial.println("wifi connected!!");
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[64];
          uint16_t code = rest.waitResponse(response, 128);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
             Serial.println(pass);
           //  Serial.println(response);
            } 
          else 
            {
             Serial.print(fail);
           //  Serial.println(code);
           return false;
            }
          
        }
        
    } 
     
// function to get humidity from 
boolean get_location()
    { 
      if(autoDetectLocation)
        {
           sprintf(buff, "/getCityCountryByIP");
           // process any callbacks coming from esp_link
           esp.Process();
        }
      else
        {
           sprintf(buff, "/getCityCountry?id=%s&appid=%s",CityID.c_str(),API_KEY.c_str());
           // process any callbacks coming from esp_link
           esp.Process();
         }  
     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[20]="";
          uint16_t code = rest.waitResponse(response, 20);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
             Serial.println(pass);
             location = response;  
            } 
          else 
            {
             Serial.print(fail);
           //  Serial.println(code);
             return false;
            }
          delay(1000);
        }
        
    }   
    
boolean get_Temperature()
    { 
      if(autoDetectLocation)
        {
           sprintf(buff, "/temperature?appid=%s",API_KEY.c_str());
           // process any callbacks coming from esp_link
           esp.Process();
        }
      else
        {
           sprintf(buff, "/temperature?id=%s&appid=%s",CityID.c_str(),API_KEY.c_str());
           // process any callbacks coming from esp_link
           esp.Process();
         }  
     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[]="";
          uint16_t code = rest.waitResponse(response, 3);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
             Serial.println(pass);
             temperature = atoi(response);    //convert recieved string to integer
             tempInC = (temperature - 273.15); // temperature values recieved from WeatherDataApi
                                                     // are in Kelvin convert it to Celcius
            } 
          else 
            {
             Serial.print(fail);
            // Serial.println(code);
             return false;
            }
          delay(1000);
        }
        
    }   
    
     
boolean get_Humidity()
    { 
       if(autoDetectLocation)
        {
           sprintf(buff, "/humidity?appid=%s",API_KEY.c_str());
           // process any callbacks coming from esp_link
           esp.Process();
        }
       else
         {
           sprintf(buff, "/humidity?id=%s&appid=%s",CityID.c_str(),API_KEY.c_str());
           // process any callbacks coming from esp_link
           esp.Process();
         }  
     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[]="";
          uint16_t code = rest.waitResponse(response, 2);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
             Serial.println(pass);
             Serial.print("Response: ");
             Serial.println(response);
             int hum = atoi(response);  // convert recieved string to integer
             if (hum >=101 && hum<200)
                { hum = 100;
                  humidity = hum;
                } 
             else if(hum >199) 
                {
                  hum = hum/10;
                  humidity = hum; 
                }  
             else
                {
                  humidity = hum; 
                }  
            }    
          else 
            {
             Serial.print(fail);
            // Serial.println(code);
             return false;
            }
          delay(1000);
        }
        
    }   
         
boolean get_weatherDescription()
    { 
       if(autoDetectLocation)
        {
           sprintf(buff, "/weatherDescription?appid=%s",API_KEY.c_str());
           // process any callbacks coming from esp_link
           esp.Process();
        }
      else
        {
           sprintf(buff, "/weatherDescription?id=%s&appid=%s",CityID.c_str(),API_KEY.c_str());
           // process any callbacks coming from esp_link
           esp.Process();
         }
     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[20]="";
          uint16_t code = rest.waitResponse(response, 20);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
             Serial.println(pass);
             weatherDescription = response;
            } 
          else 
            {
              
             Serial.print(fail);
         //    Serial.println(code);
             return false;
            }
          delay(1000);
        }
        
    }  
   
    
void displayIcon() 
    {
      //"clear-day, clear-night, rain, snow, sleet, wind, fog, cloudy, partly-cloudy-day, or partly-cloudy-night"
      if (weatherDescription == "Clear") 
         {
          drawFrame1(clear_sky);
         } 
      else if (weatherDescription == "Clouds") 
         {
          drawFrame1(few_clouds);
         } 
      else if (weatherDescription == "Scattered clouds") 
         {
          drawFrame1(scattered_clouds);
         }
      else if (weatherDescription == "Broken clouds") 
         {
          drawFrame1(broken_clouds);
         } 
      else if (weatherDescription == "Shower rain") 
         {
          drawFrame1(shower_rain);
         }
      else if (weatherDescription == "Rain") 
         {
          drawFrame1(rain);
         } 
      else if (weatherDescription == "Thunderstorm") 
         {
          drawFrame1(thunderstorm);
         } 
      else if (weatherDescription == "Snow") 
         {
          drawFrame1(snow);
         } 
      else if (weatherDescription == "Mist") 
         {
          drawFrame1(mist);
         } 
      else 
         {
          drawFrame1(no_icon);
         } 
    } 

//print weather icon and weather description on OLED
void drawFrame1(const uint8_t *bitmap) 
    { 
      
      
       u8g.setPrintPos( 0, 11);
       u8g.print(location);
       u8g.drawXBMP(  40, 12, icon_width, icon_height, bitmap);
       u8g.setPrintPos(  10, 61);
       u8g.print(weatherDescription);
     
    }
//print thermometer icon and tmeperature and humidity values on OLED
void drawFrame2() 
    {  
      
       u8g.setPrintPos(0, 11);
       u8g.print(location);
       u8g.setPrintPos( 59 , 31);
       u8g.print(tempInC);
       u8g.drawXBMP( 75 , 15, icon_width, icon_height, temperature_icon);
       u8g.drawStr(  0, 45, "Hum:");
       u8g.setPrintPos(  0, 60);
       u8g.print(humidity);
       if(humidity == 100)
         {
           u8g.drawXBMP( 25, 50, percentage_sign_WIDTH,percentage_sign_HEIGHT, percentage_sign);
         }
       else
         {
           u8g.drawXBMP( 20 , 50, percentage_sign_WIDTH,percentage_sign_HEIGHT, percentage_sign);
         }  
      
    }

void drawFrame3() 
    {
     
     // u8g.setFont(u8g_font_timB14);
      u8g.drawStr(0,15, "Room Temp: ");
    //  u8g.setPrintPos( 0, 11);
    //   u8g.print("Room Temp:");
     //u8g.setFont(u8g_font_timB14);
     
      
       u8g.setPrintPos(45, 40); 
      u8g.print(tempInCelcius );
      u8g.setPrintPos(67, 40);
      u8g.print(char(176));
      u8g.drawStr( 75, 40, "C");
     //  u8g.setFont(u8g_font_timB12);
      
     // u8g.drawXBMP( 55 , 25, icon_width, icon_height, temperature_icon);
          
    }

   
// function to scroll frames on OLED after 5 seconds  
void frameScroll()
    { unsigned long currentMillis = millis();
      if(currentMillis - previousMillis > interval1 && frame1 == true)
        {
          u8g.firstPage();
          do 
            {  
              displayIcon();
              frame1 = false;
            } while( u8g.nextPage() );}
 
      if(currentMillis - previousMillis > interval2 && frame2 == true) 
        {
          u8g.firstPage();
          do 
            {  
              drawFrame2();
              frame2 = false;
            } while( u8g.nextPage() );}
      
      if(currentMillis - previousMillis > interval3 && frame3==true) 
        {
          u8g.firstPage();
          do 
            {  
              drawFrame3();
              frame3 = false;
            } while( u8g.nextPage() );
     
       previousMillis = currentMillis;
       frame1 = true;
       frame2 = true;
       frame3 = true;
     }
   
   }
   

 void weatherData()
     {
      if(time % (refreshRate*60) == 0 )
        {
          int err = rest.begin("idiotware.herokuapp.com");
          if(err != 0) 
            {
              Serial.print("REST begin failed: ");
              u8g.firstPage();
              do 
               {  
                u8g.drawStr(0,15,"REST begin failed:");
               } while( u8g.nextPage() );
              Serial.println(err);
              while(1) ;
            }
          get_location();
          get_Temperature();
          get_Humidity();
          get_weatherDescription();
        }
     }     
