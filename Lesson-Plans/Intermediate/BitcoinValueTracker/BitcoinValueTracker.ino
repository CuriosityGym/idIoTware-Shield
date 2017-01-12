/*
              Location Display
   In this example we are using ESP8266 wifi module and OLED display
 to display location using your IP address.
                         
*/



#include "U8glib.h"
#include <ELClient.h>
#include <ELClientRest.h>
char buff[32];
char buff2[32];
String bitcoin_value = "";
unsigned long time=0;
unsigned long samplingTime = 3600;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);
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
              do { u8g.setFont(u8g_font_timB10); 
                   u8g.drawStr(1,13,"WIFI DISCONNECTED!!");
                 } while( u8g.nextPage() );
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
      
      u8g.setColorIndex(1);
      // Sync-up with esp-link, this is required at the start of any sketch and initializes the
      // callbacks to the wifi status change callback. The callback gets called with the initial
      // status right after Sync() below completes.
      esp.wifiCb.attach(wifiCb); // wifi status change callback, optional (delete if not desired)
      bool ok;
      do 
       {
         ok = esp.Sync();      // sync up with esp-link, blocks for up to 2 seconds
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

      // Set up the REST client to talk to idiotware.herokuapp.com, this doesn't connect to that server,
      // it just sets-up stuff on the esp-link side
      int err = rest.begin("api.thingspeak.com");
      if(err != 0) 
        {
          Serial.print("REST begin failed: ");
          Serial.println(err);
          while(1) ;
        }
      Serial.println("EL-REST ready");
    
     get_bitcoinValue();
   
    
         
      
    }

void loop() 
    {
      unsigned long  elapsedTime = millis()/1000;   // this variable will keep track of elapsed time
      while(((millis()/1000)-elapsedTime) < 1);    // this loop will do nothing until a second has passed 
      time++;                                       //increment time after each second.
      
      if((time % samplingTime == 0))
        { 
          get_bitcoinValue();
        }  
   
    }
 
    
void get_bitcoinValue()
    { 
      sprintf(buff, "/apps/thinghttp/send_request?api_key=558W30NPHWSXVP3J");
           // process any callbacks coming from esp_link
      esp.Process();

     
      // if we're connected make an HTTP request
      if(wifiConnected) 
        {
          // Request /utc/now from the previously set-up server
          rest.get((const char*)buff);

          char response[20];
          uint16_t code = rest.waitResponse(response, 20);
          if(code == HTTP_STATUS_OK)     //check for response for HTTP request  
            {
              Serial.println("ARDUINO: GET successful:");
              bitcoin_value = response;
              String bitcoinValue = bitcoin_value.substring(0,8);
              u8g.firstPage();
              do { u8g.setFont(u8g_font_timB10);
                   u8g.drawStr(1,10,"Bitcoin value in USD:");
                   u8g.setFont(u8g_font_timB24); 
                   u8g.setPrintPos(5, 50);
                   u8g.print(bitcoinValue);
                 } while( u8g.nextPage() );
            } 
          else 
            {
              Serial.print("ARDUINO: GET failed: ");
              Serial.println(code);
            }
          //delay(5000);
        }
        
    }   
   
