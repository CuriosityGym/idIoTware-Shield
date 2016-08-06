/*
idIoTWare Shield Weather Gadget
**Attention: Please install all libraries from our Github Repository to enable the example to run.**

In this example we are using ESP8266-01 Wifi Module with esp-link firmware and an I2C OLED display
to show weather conditinos of particular city, fetched from OpenWeatherMaps http://www.openweathermap.org/appid

WeatherDataApi: Heroku Deployment Application that reduces data downloaded for OpenWeatherMap Api
Due to memory limits on the Arduino, an intermediate server filters data recieved from OpenWeatherMaps.
The python code can be accessed here https://github.com/CuriosityGym/WeatherDataApi



To get data from OpenWeatherMap Api, you need to create openweathermaps account and then generate API key, and use that API Key in this code.
You can read on how to do it here http://www.openweathermap.org/appid                

In this example we have used ELClient.h library to get data from WeatherDataApi. 
We are sending requests for location, current temprature, humnidity and 
weather description. 
This code allows you to detect your location
1)Automatically by IP address 
2)To get weather conditions of particular city just change  the cityID in the following code and change autoDetectLocation variable to false. 

OLED is used to display this information.

We are sending request after every one hour to get weather information.To change this interval, change value of variable refreshRate. 
               
*/



#include "U8glib.h"
#include <ELClient.h>
#include <ELClientRest.h>
#include "icons.h"    // Bitmap weather icons 
#define autoDetectLocation false
#define refreshRate 60   //send request after ever 60 minutes(one hour) 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI 


char buff[128];
//variables to store recieved data
float temperature;
int humidity;
String location = "";
String weatherDescription = "";
int tempInCelsius;

// replace with your channel's thingspeak API key
String API_KEY = "15373f8c0b06b6e66e6372db065c4e46";
String CityID = "1275339"; //Mumbai, India

// varibles for cursor positions
int yPos = 0;
int xPos = 0;

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
               u8g.firstPage();
               do 
                 {  
                  u8g.drawStr(0,12,"WIFI Disconnected");
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
      u8g.setFont(u8g_font_timB10);
      u8g.setColorIndex(1); // Instructs the display to draw with a pixel on. 
         
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
                u8g.drawStr(0,12,"WIFI Disconnected");
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
      int err = rest.begin("idiotware.herokuapp.com");
      if(err != 0) 
        {
          Serial.print("REST begin failed: ");
          Serial.println(err);
          while(1) ;
        }
      Serial.println("EL-REST ready");  
     
    }

void loop() 
    { 
      get_location();
      get_Temperature();
      get_Humidity();
      get_weatherDescription();
      for (int i=0; i<= (refreshRate*60)/10; i++)
          {
            frameScroll();
          }
    }      
 
// function to get humidity from 
void get_location()
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
             Serial.println("ARDUINO: GET successful:");
             location = response;  
            } 
          else 
            {
             Serial.print("ARDUINO: GET failed: ");
             Serial.println(code);
            }
          delay(1000);
        }
        
    }   
    
void get_Temperature()
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
             Serial.println("ARDUINO: GET successful:");
             temperature = atoi(response);    //convert recieved string to integer
             tempInCelsius = (temperature - 273.15); // temperature values recieved from WeatherDataApi
                                                     // are in Kelvin convert it to Celcius
            } 
          else 
            {
             Serial.print("ARDUINO: GET failed: ");
             Serial.println(code);
            }
          delay(1000);
        }
        
    }   
    
     
void get_Humidity()
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
             Serial.println("ARDUINO: GET successful:");
             Serial.print("Response: ");
             Serial.println(response);
             int hum = atoi(response);  // convert recieved string to integer
             if (hum > 100)
                { hum = hum/10;
                  humidity = hum;
                } 
             else 
                {
                  humidity = hum; 
                }  
            }    
          else 
            {
             Serial.print("ARDUINO: GET failed: ");
             Serial.println(code);
            }
          delay(1000);
        }
        
    }   
         
void get_weatherDescription()
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
             Serial.println("ARDUINO: GET successful:");
             weatherDescription = response;
            } 
          else 
            {
             Serial.print("ARDUINO: GET failed: ");
             Serial.println(code);
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
       u8g.setPrintPos( yPos + xPos, 11);
       u8g.print(location);
       u8g.drawXBMP( yPos + xPos + 40, 12, icon_width, icon_height, bitmap);
       u8g.setPrintPos( yPos + xPos + 10, 61);
       u8g.print(weatherDescription);
     
    }
//print thermometer icon and tmeperature and humidity values on OLED
void drawFrame2() 
    { 
       u8g.setPrintPos(yPos + xPos - 128, 11);
       u8g.print(location);
       u8g.setPrintPos(yPos - 67 + xPos, 31);
       u8g.print(tempInCelsius);
       u8g.drawXBMP( yPos - 53 + xPos, 15, icon_width, icon_height, temperature_icon);
       u8g.drawStr( yPos - 128 + xPos, 45, "Hum:");
       u8g.setPrintPos( yPos - 128 + xPos, 60);
       u8g.print(humidity);
       if(humidity == 100)
         {
           u8g.drawXBMP( yPos - 105 + xPos, 50, percentage_sign_WIDTH,percentage_sign_HEIGHT, percentage_sign);
         }
       else
         {
           u8g.drawXBMP( yPos - 110 + xPos, 50, percentage_sign_WIDTH,percentage_sign_HEIGHT, percentage_sign);
         }  
         
    }

// function to scroll frames on OLED after 5 seconds  
void frameScroll()
    {
       u8g.firstPage();
     do 
       {  
         displayIcon();
         drawFrame2();
       } while( u8g.nextPage() );
     if(yPos == 0)
       {
         delay(5000);
       }
     // If its too fast, you could add a delay
     if(yPos < 128)
       {
         // if it's too slow, you could increment y by a greater number
         yPos+=32;
       }
  
     else 
       {
         if(xPos == 0)
           {
             delay(5000);
           }
         if(xPos <128)
           {
             xPos+=32;
           }
         else
           { // When the yPos is off the screen, reset to 0.
             xPos=0;
             yPos=0;
           }
       }  
   }
