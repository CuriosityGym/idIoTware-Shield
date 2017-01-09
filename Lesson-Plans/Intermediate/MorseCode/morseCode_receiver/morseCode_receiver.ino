/*
    MORSE CODE
    
    Morse code is a method of sending text messages by keying in a series of electronic pulses,
    usually represented as a short pulse (called a "dot") and a long pulse (a "dash"). The code 
    was devised by Samuel F. B. Morse in the 1840s
    
    In this example, we are using two NRF24L01 radio modules to transmit and receive Morse Code.
    On Transmitter side, Interpret presses on button (connected to digital pin 7) as Morse Code
    inputs and send it to receiver side using NRF24L01 radio.
    On Receiver side, NRF24L01 radio will receive the message and arduino will convert the signal 
    pattern consist of DOTs and DASHes to that particular letter and also print that letter to 
    OLED with its morse code. 
    
    For Example, to send letter A (DOT,DASH),
    press pushbutton two times, for DOT press pushbutton for 200 milliseconds,
                                for DASH press pushbutton for 600 milliseconds,
                                time gap between DOT and DASH should be less than 200 milliseconds.
    To send HELLO word
    
     H --> (DOT,DOT,DOT,DOT)      // press button for 200 millisconds four times then wait for 600 milliseconds which 
                                  // is gap between two letters (in milliseconds), then Arduino will recognize it as letter H.
     E --> (DOT)                  // then again press button for 200 milliseconds and wiat for 600 milliseconds ,then Arduino will recognize it as letter E 
     L --> (DOT,DASH,DOT,DOT)     // press button for 200,600,200,200 milliseconds four times respectively and wiat for 600 milliseconds,
                                  //then Arduino will recognize it as letter L
     L --> (DOT,DASH,DOT,DOT)     // press button for 200,600,200,200 milliseconds four times respectively and wiat for 600 milliseconds,
                                  //then Arduino will recognize it as letter L
     O --> (DASH,DASH,DASH)       //press button for 600,600,600 milliseconds three times and wiat for 600 milliseconds ,then Arduino will recognize it as letter 0
 
  now to send the new word wait for 1400 milliseconds (1.4 seconds) and send particular signlas for that word         
         
*/


#include "U8glib.h"
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#define WordGap 1400
// Hardware configuration
// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI
RF24 radio(7,8);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t node_1 = 0;

// Address of the master node
const uint16_t master_node = 1;

boolean send_message = false;
char messageToSend[32] = "";
String receivedWord = "";
unsigned long lastTimestamp =0;
void setup()
    {
      Serial.begin(57600);
      
      // u8g.setFont(u8g_font_unifont);
      u8g.setColorIndex(1); // Indataucts the display to draw with a pixel on. 
      Serial.println("MORSE CODE");
      showMessageOnLcd(27,17,"MORSE",35,45,"CODE");
      delay(1000);
      SPI.begin();
      radio.begin();
      network.begin(/*channel*/ 90, /*node address*/ node_1); 
    }

//function to print received data on OLED
void showMessageOnLcd(int x,int y, String message1,int a,int b, String message2)
    { 
      u8g.firstPage();
      do { u8g.setFont(u8g_font_timB14);
           u8g.drawStr( x, y, message1.c_str());
           u8g.drawStr( a, b, message2.c_str());
           u8g.setFont(u8g_font_timB12);
           u8g.drawStr( 0,40, receivedWord.c_str());    // print received word on OLED
         } while( u8g.nextPage() );
    }

void loop() 
    {  
       unsigned long currentTimestamp  = millis(); // get the current timestamp
       unsigned long duration = currentTimestamp - lastTimestamp; // get elapsed time
       // Pump the network regularly
       network.update();
        if (send_message)   // if there is typed message on serial monitor ready to send
           {
             RF24NetworkHeader header1(/*to node*/ master_node);
             boolean message = network.write(header1, messageToSend, 32);   // send message to other user
             if (message)
                {
                  //Serial.print("Sender: ");   // print message on serial monitor
                  //Serial.println(messageToSend);
                  send_message = false;
                }
          
             else
             
                 Serial.println("could not write \n"); // if it is failed to send message prompt error 
                 showMessageOnLcd(1,15,"could not write",5,45,"");  
           }  
      
        // Is there anything ready for us?
        while (network.available() )
              { // If so, grab it and print it out
                RF24NetworkHeader header;
                char messageToRecieve[32] = "";
                boolean recieve = false;
                while (!recieve)
                      {
                        recieve = network.read(header, messageToRecieve ,32);
                        Serial.print("Reciver: ");   // print recived data on serial monitor
                        Serial.println(messageToRecieve);
                        lastTimestamp = currentTimestamp; 
                      }
                String MorseCode(messageToRecieve);    // convert char array of received message to string
                Serial.println(showLetter(MorseCode)); //and print letter on serial monitor
                receivedWord += showLetter(MorseCode);
                Serial.print(receivedWord);  //print received message on serial monitor  
                showLetter(MorseCode);       // show letter on OLED
                if (duration > WordGap)      //if delay between received messages is greater than WordGap(1400)
                   {                         // add space between words
                     receivedWord += " ";
                   }
                if (receivedWord.length()>=13)  // if stringlenth is greater than 13, empty the string
                   { 
                     receivedWord = "";
                   }   
          }  
  
     }
 
// function to decode the received morse code to respective letter and print that letter on OLED  
char showLetter(String data)
    {
        if (data == "12000") { showMessageOnLcd(5,13,"A",25,15,"._");       return 'A';}  
        if (data == "21110") { showMessageOnLcd(5,13,"B",25,15,"_...");     return 'B';}
        if (data == "21210") { showMessageOnLcd(5,13,"C",25,15,"_._.");     return 'C';}
        if (data == "21100") { showMessageOnLcd(5,13,"D",25,15,"_..");      return 'D';}
        if (data == "10000") { showMessageOnLcd(5,13,"E",25,15,".");        return 'E';}    
        if (data == "11210") { showMessageOnLcd(5,13,"F",25,15,".._.");     return 'F';}
        if (data == "22100") { showMessageOnLcd(5,13,"G",25,15,"_ _.");     return 'G';}
        if (data == "11110") { showMessageOnLcd(5,13,"H",25,15,"....");     return 'H';}  
        if (data == "11000") { showMessageOnLcd(5,13,"I",25,15,"..");       return 'I';}  
        if (data == "12220") { showMessageOnLcd(5,13,"J",25,15,"._ _ _");   return 'I';}
        if (data == "21200") { showMessageOnLcd(5,13,"K",25,15,"_._");      return 'K';}
        if (data == "12110") { showMessageOnLcd(5,13,"L",25,15,"._..");     return 'L';}                 
        if (data == "22000") { showMessageOnLcd(5,13,"M",25,15,"_ _");      return 'M';} 
        if (data == "21000") { showMessageOnLcd(5,13,"N",25,15,"_.");       return 'N';}  
        if (data == "22200") { showMessageOnLcd(5,13,"O",25,15,"_ _ _");    return 'O';}          
        if (data == "12210") { showMessageOnLcd(5,13,"P",25,15,"._ _.");    return 'P';}
        if (data == "22120") { showMessageOnLcd(5,13,"Q",25,15,"_ _._");    return 'Q';}
        if (data == "12100") { showMessageOnLcd(5,13,"R",25,15,"._.");      return 'R';}  
        if (data == "11100") { showMessageOnLcd(5,13,"S",25,15,"...");      return 'S';}  
        if (data == "20000") { showMessageOnLcd(5,13,"T",25,15,"_");        return 'T';}
        if (data == "11200") { showMessageOnLcd(5,13,"U",25,15,".._");      return 'U';}
        if (data == "11120") { showMessageOnLcd(5,13,"V",25,15,"..._");     return 'V';}
        if (data == "12200") { showMessageOnLcd(5,13,"W",25,15,"._ _");     return 'W';}
        if (data == "21120") { showMessageOnLcd(5,13,"X",25,15,"_.._");     return 'X';}
        if (data == "21220") { showMessageOnLcd(5,13,"Y",25,15,"_._ _");    return 'Y';}
        if (data == "22110") { showMessageOnLcd(5,13,"Z",25,15,"_ _..");    return 'Z';} 
        if (data == "12222") { showMessageOnLcd(5,13,"1",25,15,"._ _ _ _"); return '1';}
        if (data == "11222") { showMessageOnLcd(5,13,"2",25,15,".._ _ _ "); return '2';}
        if (data == "11122") { showMessageOnLcd(5,13,"3",25,15,"..._ _");   return '3';}
        if (data == "11112") { showMessageOnLcd(5,13,"4",25,15,"...._");    return '4';}
        if (data == "11111") { showMessageOnLcd(5,13,"5",25,15,".....");    return '5';}
        if (data == "21111") { showMessageOnLcd(5,13,"6",25,15,"_....");    return '6';}
        if (data == "22111") { showMessageOnLcd(5,13,"7",25,15,"_ _...");   return '7';}
        if (data == "22211") { showMessageOnLcd(5,13,"8",25,15,"_ _ _..");  return '8';}
        if (data == "22221") { showMessageOnLcd(5,13,"9",25,15,"_ _ _ _."); return '9';}
        if (data == "22222") { showMessageOnLcd(5,13,"0",25,15,"_ _ _ _ _");return '0';} 
      }     
