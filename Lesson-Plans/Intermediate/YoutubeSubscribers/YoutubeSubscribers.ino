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
String subscribers = "";
String channelName = "";
 String channel_Name = "";
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
   
     get_ChannelName();
     get_SubscriberCount();
    
         
      
    }

void loop() 
    {
   
    }
 
    
void get_ChannelName()
    { 
      sprintf(buff, "/apps/thinghttp/send_request?api_key=IP8NMS42UUM10Z2O");
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
             
             /// int data = Serial.readBytesUntil('\n',response,32);
             // response[data] = '\0';
              Serial.println(response);
              channelName = response;
              channel_Name = channelName.substring(0,9);
              Serial.println(channel_Name);
              u8g.firstPage();
              do { u8g.setFont(u8g_font_timB10);
                   u8g.drawStr(1,10,"Channel Name:");
                   u8g.setFont(u8g_font_timB14); 
                   u8g.setPrintPos(5, 30);
                   u8g.print(channel_Name);
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
    
void get_SubscriberCount()
    { 
      sprintf(buff, "/apps/thinghttp/send_request?api_key=2J2UYWIDFW1SAID2");
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
              subscribers = response;
              String subscribers_count = subscribers.substring(0,9);
             // Serial.println(subscribers);
              //int data = Serial.readBytesUntil('\n',response,32);
              //response[data] = '\0';
              //Serial.println(response);
              //subscribers = response;
              Serial.println(subscribers_count);
              u8g.firstPage();
              do {
                   u8g.setFont(u8g_font_timB10);
                   u8g.drawStr(1,10,"Channel Name:");
                   u8g.setFont(u8g_font_timB14); 
                   u8g.setPrintPos(5, 30);
                   u8g.print(channel_Name);
                   u8g.setFont(u8g_font_timB10);
                   u8g.drawStr(1,45,"Subscribers:");
                   u8g.setFont(u8g_font_timB14); 
                   u8g.setPrintPos(5, 62);
                   u8g.print(subscribers_count);
                 } while( u8g.nextPage() );
            } 
          else 
            {
              Serial.print("ARDUINO: GET failed: ");
              Serial.println(code);
            }
          delay(5000);
        }
        
    }       
   
