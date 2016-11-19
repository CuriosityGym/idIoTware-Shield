/*  

    In this example we are using NRF24L01 radio and Arduino to
    communicate with other Arduino and NRF24L01 using Serial Monitor.
    Here we are creating a local mesh between two radios(NRF24L01)
    using <RF24.h> and <RF24Network.h library>.  


*/

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "U8glib.h"
//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(7,8);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 0;

// Address of the other node
const uint16_t other_node = 1;

boolean send_message = false;
char messageToSend[32] = "";

int melody[]= {2000,2000,2000,2000};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 8,8,8,8 };

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI 

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void setup(void)
    { 
      u8g.setFont(u8g_font_unifont);
      Serial.begin(57600);
      Serial.println("NRF24L01 MESSENGER");
 
      SPI.begin();
      radio.begin();
      network.begin(/*channel*/ 90, /*node address*/ this_node);
      u8g.firstPage();  
      do 
       {
         u8g.setPrintPos(0, 10); 
         u8g.print("Organizer " );
         u8g.setPrintPos(45, 10); 
       //  u8g.print(this_node);
       } while( u8g.nextPage() );   
    }

void loop() 
    {
       // Pump the network regularly
       network.update();
       if (stringComplete)   // if there is typed message on serial monitor ready to send
        { 
          inputString.replace("\r","");
          inputString.replace("\n","");
          RF24NetworkHeader header1(/*to node*/ other_node);
          boolean message = network.write(header1,  inputString.c_str(), 32);   // send message to other user
          if (message)
            {
              Serial.print("Organizer: ");   // print message on serial monitor
              Serial.println( inputString);
              send_message = false;
            }
          
          else
            
              Serial.println("could not write....\n"); // if it is failed to send message prompt error 
               
              stringComplete=false;
              inputString="";   
      }  
    
     //// Is there anything ready for us?
     while (network.available() )
       { // If so, grab it and print it out
         RF24NetworkHeader header;
         char messageToRecieve[32] = "";
         boolean recieve = false;
         while (!recieve)
            {
              recieve = network.read(header, messageToRecieve ,32);
              Serial.print("Organizer: ");   // print recived data on serial monitor
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
                    u8g.drawStr(0,10, "participant :"); 
                    u8g.setPrintPos(0, 30);  
                    u8g.print(messageToRecieve); 
                  } while( u8g.nextPage() ); 
                 
            }
 
        }
   
   }
  
   void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}
   
