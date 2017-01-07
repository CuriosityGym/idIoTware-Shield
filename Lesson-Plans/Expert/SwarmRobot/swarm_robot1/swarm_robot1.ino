#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "U8glib.h"

// Hardware configuration
// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(7,8);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 0;

// Address of the other node
const uint16_t other_node = 1;

boolean send_message = false;
char messageToSend[32] = "";  //

int melody[]= {2000,2000,2000,2000};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 8,8,8,8 };

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);	// I2C / TWI 

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

int motor_left[] = {2, 3};
int motor_right[] = {4, 5};
#define trigPin 9
#define echoPin 6
long previousMillis = 0;        // will store last time LED was updated
long interval = 1000;           // interval at which to blink (milliseconds)
int duration, distance;
int x, y;
boolean msg=false;
// --------------------------------------------------------------------------- Setup
void setup() 
     { 
       Serial.begin(57600);
       pinMode(trigPin, OUTPUT);
       pinMode(echoPin, INPUT);
       // Setup motors
       for(byte i = 0; i < 2; i++)
          {
            pinMode(motor_left[i], OUTPUT);
            pinMode(motor_right[i], OUTPUT);
          }
       u8g.setFont(u8g_font_unifont);
       
 
       SPI.begin();
       radio.begin();
       network.begin(/*channel*/ 90, /*node address*/ this_node);
       u8g.firstPage();  
       do 
        {
          u8g.setPrintPos(0, 10); 
         u8g.print("Robot " );
         u8g.setPrintPos(45, 10); 
         u8g.print(this_node);
        } while( u8g.nextPage() );   
     

      }


// --------------------------------------------------------------------------- Loop
void loop() 
    {  
      // Pump the network regularly
       network.update();
       
       unsigned long currentMillis = millis();
       
       if(currentMillis - previousMillis > interval)
         {
           // save the last time you blinked the LED 
           previousMillis = currentMillis;   
           digitalWrite(trigPin, HIGH);
           digitalWrite(trigPin, LOW);
           duration = pulseIn(echoPin, HIGH);
           distance = (duration/2) / 29.1;
           x = (x + distance)%100;
           y = (x + y + distance)%100;
           Serial.println(distance);
           
         }  
       if (distance < 20 && msg==true)
          { 
            Serial.println("right");
            motor_stop();
            delay(500);
            turn_right();
            delay(500);
            motor_stop();
            delay(500);
            
            inputString = String(x) + "   " + String(y);
            RF24NetworkHeader header1(/*to node*/ other_node);
            boolean message = network.write(header1, inputString.c_str(), 32); // send message to other user
            if (message)
               {
                 Serial.print("Robot2: ");  // print message on serial monitor
                 Serial.println(inputString);
                 send_message = false;
               }
          
            else
               {
                 Serial.println("could not write....\n");  // if it is failed to send message prompt error
              
               }
          }    
    
       else
          {
            Serial.println("forward");
            drive_forward();
             // Is there anything ready for us?
            while (network.available() )   
                  { 
                    // If so, grab it and print it out
                    RF24NetworkHeader header;
                    char messageToRecieve[32] = "";
                    boolean recieve = false;
                    while (!recieve)
                          {
                            recieve = network.read(header, messageToRecieve , 32 );
                            Serial.print("Robot1: ");   // print recived data on okserial monitor
                            Serial.println(messageToRecieve);
                            for(int thisNote = 0; thisNote < 4; thisNote++) 
                               {
                                // to calculate the note duration, take one second divided by the note type.
                                //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
                                int noteDuration = 1000/noteDurations[thisNote];
                                tone(A1, melody[thisNote],noteDuration);
                                // to distinguish the notes, set a minimum time between them.
                                // the note's duration + 30% seems to work well:
                                int pauseBetweenNotes = noteDuration * 1.60;
                                delay(pauseBetweenNotes);
                                // stop the tone playing:
                                noTone(8);
                               }
                             u8g.firstPage();  
                             do 
                               {
                                 u8g.drawStr(0,10, "Robot2 says: "); 
                                 u8g.drawStr(0,25, "found obstacle: ");
                                 u8g.setPrintPos(0, 44); 
                                 u8g.print(messageToRecieve); 
                               } while( u8g.nextPage() );   
                         }
                 }
          }
    }     


// --------------------------------------------------------------------------- Drive

void motor_stop()
     {
       digitalWrite(motor_left[0], LOW);
       digitalWrite(motor_left[1], LOW);

       digitalWrite(motor_right[0], LOW);
       digitalWrite(motor_right[1], LOW);
       delay(25);
     }


void drive_forward()
     {
       digitalWrite(motor_left[0], HIGH);
       digitalWrite(motor_left[1], LOW);

       digitalWrite(motor_right[0], HIGH);
       digitalWrite(motor_right[1], LOW);
     }

void turn_right()
     {
       digitalWrite(motor_left[0], HIGH);
       digitalWrite(motor_left[1], LOW);

       digitalWrite(motor_right[0], LOW);
       digitalWrite(motor_right[1], HIGH);
     }
 
