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


#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#define DOT_DURATION   200               // Duration of a Morse Code "dot" (in milliseconds)
#define DASH_DURATION  DOT_DURATION * 3  // Duration of a Morse Code "dash" (in milliseconds)
#define SIGNAL_GAP     DOT_DURATION      // Gap between dots/dashes of a single letter (in ms)
#define LETTER_GAP     DOT_DURATION * 3  // Gap between two letters (in milliseconds)
#define WORD_GAP       DOT_DURATION * 7  // Gap between two words (in milliseconds)

#define BUTTON          5                // The diginal connector port to the button.

#define DOT             1                // DOT identifier
#define DASH            2                // DASH identifier
#define NONE            0                // Neither DOT nor DASH

boolean buttonWasPressed = false;        // Indicator of whether button was pressed in the last cycle
long lastTimestamp = 0;                  // Last recorded timestamp  (used for mesuring duration)
char inputSignal[5]="";                     // Input signal buffer
int inputSignalIndex = 0;                // Index into the input signal buffer
String code = "";
RF24 radio(7,8);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 1;

// Address of the other node
const uint16_t other_node = 0;

boolean send_message = false;
char messageToSend[32] = ""; 

void resetInputSignal()         // Reset the input signal buffer and index
    {         
      inputSignal[0] = NONE;
      inputSignal[1] = NONE; 
      inputSignal[2] = NONE;
      inputSignal[3] = NONE;
      inputSignal[4] = NONE;
      inputSignalIndex = 0;
    }

void setup() 
    {
      SPI.begin();
      radio.begin();
      network.begin(/*channel*/ 90, /*node address*/ this_node);
      pinMode(BUTTON, INPUT);                // Set the button input
      resetInputSignal();                    // Reset input signal buffer
      Serial.begin(57600);
      Serial.println("NRF24L01 MESSENGER");  
    }

void loop() 
    { // loop forever
       network.update();
       if (send_message)   // if there is typed message on serial monitor ready to send
          {
            RF24NetworkHeader header1(/*to node*/ other_node);
            boolean message = network.write(header1, messageToSend, sizeof(messageToSend));   // send message to other user
            if (message)
               { 
                 Serial.println("sending msg....");
                 Serial.print("Sender: ");   // print message on serial monitor
                 Serial.println(messageToSend);
                 send_message = false; 
                 resetInputSignal(); // reset the input signal buffer
               }
          
            else 
            
               Serial.println("could not write....\n"); // if it is failed to send message prompt error 
          }  
            long currentTimestamp  = millis(); // get the current timestamp
            long duration = currentTimestamp - lastTimestamp; // get elapsed time
            if (digitalRead(BUTTON) == HIGH) 
               { 
                 // if the button is pressed
                 if (!buttonWasPressed) 
                    { 
                      //  if the button was previously not pressed
                      buttonWasPressed = true; // remember the button press
                      lastTimestamp = currentTimestamp; // record the time of the button press
                      if (duration > LETTER_GAP) 
                         {
                           Serial.print(' ');
                         }
                    } // end of if (button was not pressed)
               } 
            else 
               { 
                 // the button is not pressed
                 if (buttonWasPressed) 
                    { 
                      // the button was just released
                      if (duration < DOT_DURATION)
                         { 
                           // if the button was pressed for up to DOT cutoff
                           inputSignal[inputSignalIndex] = DOT; // remember the current signal as a DOT
                         } 
                      else 
                         { 
                           // if the button was pressed for longer than DOT cutoff
                           inputSignal[inputSignalIndex] = DASH; // remember the current signal as a DASH
                         }
                     inputSignalIndex++; // advance the index to the input signal buffer
                     buttonWasPressed = false; // consume previous button press
                     lastTimestamp = currentTimestamp; // record the time the button was released
                   } 
                else 
                   { 
                     // the button was not just released
                     if (inputSignalIndex > 0) 
                        {
                          // if there is data in the input signal buffer
                          if (duration > SIGNAL_GAP || inputSignalIndex >= 4)
                             {
                               // if we have a complete letter
                               currentInputSignalToLetter();
                               send_message = true;
                               Serial.print(messageToSend); // parse the letter and send it via serial
                               Serial.println(currentInputSignalToLetter());
                               send_message = true;
                             }
                        }
                   } // end of else (button was not previously pressed)
            } // end of else (button is not pressed)
     } // end of loop

// return true if s0-s4 match input signal
boolean matchInputSignal(byte s0, byte s1, byte s2, byte s3, byte s4) 
       {
         return ((inputSignal[0] == s0) && 
                 (inputSignal[1] == s1) && 
                 (inputSignal[2] == s2) && 
                 (inputSignal[3] == s3) &&  
                 (inputSignal[4] == s4));
       }

// convert input signal to letter or ? if not found
char currentInputSignalToLetter() 
    {  
      if (matchInputSignal(DOT, DASH, NONE, NONE, NONE))  { code ="12000"; strcpy(messageToSend, code.c_str()); return 'A'; }//A
      if (matchInputSignal(DASH, DOT, DOT, DOT, NONE))    { code ="21110"; strcpy(messageToSend, code.c_str()); return 'B'; }//B
      if (matchInputSignal(DASH, DOT, DASH, DOT, NONE))   { code ="21210"; strcpy(messageToSend, code.c_str()); return 'C'; }//C
      if (matchInputSignal(DASH, DOT, DOT, NONE, NONE))   { code ="21100"; strcpy(messageToSend, code.c_str()); return 'D'; }//D
      if (matchInputSignal(DOT, NONE, NONE, NONE, NONE))  { code ="10000"; strcpy(messageToSend, code.c_str()); return 'E'; }//E
      if (matchInputSignal(DOT, DOT, DASH, DOT, NONE))    { code ="11210"; strcpy(messageToSend, code.c_str()); return 'F'; }//F
      if (matchInputSignal(DASH, DASH, DOT, NONE, NONE))  { code ="22100"; strcpy(messageToSend, code.c_str()); return 'G'; }//G
      if (matchInputSignal(DOT, DOT, DOT, DOT, NONE))     { code ="11110"; strcpy(messageToSend, code.c_str()); return 'H'; }//H
      if (matchInputSignal(DOT, DOT, NONE, NONE, NONE))   { code ="11000"; strcpy(messageToSend, code.c_str()); return 'I'; }//I
      if (matchInputSignal(DOT, DASH, DASH, DASH, NONE))  { code ="12220"; strcpy(messageToSend, code.c_str()); return 'J'; }//J
      if (matchInputSignal(DASH, DOT, DASH, NONE, NONE))  { code ="21200"; strcpy(messageToSend, code.c_str()); return 'K'; }//K
      if (matchInputSignal(DOT, DASH, DOT, DOT, NONE))    { code ="12110"; strcpy(messageToSend, code.c_str()); return 'L'; }//L
      if (matchInputSignal(DASH, DASH, NONE, NONE, NONE)) { code ="22000"; strcpy(messageToSend, code.c_str()); return 'M'; }//M
      if (matchInputSignal(DASH, DOT, NONE, NONE, NONE))  { code ="21000"; strcpy(messageToSend, code.c_str()); return 'N'; }//N
      if (matchInputSignal(DASH, DASH, DASH, NONE, NONE)) { code ="22200"; strcpy(messageToSend, code.c_str()); return 'O'; }//O
      if (matchInputSignal(DOT, DASH, DASH, DOT, NONE))   { code ="12210"; strcpy(messageToSend, code.c_str()); return 'P'; }//P
      if (matchInputSignal(DASH, DASH, DOT, DASH, NONE))  { code ="22120"; strcpy(messageToSend, code.c_str()); return 'Q'; }//Q
      if (matchInputSignal(DOT, DASH, DOT, NONE, NONE))   { code ="12100"; strcpy(messageToSend, code.c_str()); return 'R'; }//R
      if (matchInputSignal(DOT, DOT, DOT, NONE, NONE))    { code ="11100"; strcpy(messageToSend, code.c_str()); return 'S'; }//S
      if (matchInputSignal(DASH, NONE, NONE, NONE, NONE)) { code ="20000"; strcpy(messageToSend, code.c_str()); return 'T'; }//T
      if (matchInputSignal(DOT, DOT, DASH, NONE, NONE))   { code ="11200"; strcpy(messageToSend, code.c_str()); return 'U'; }//U
      if (matchInputSignal(DOT, DOT, DOT, DASH, NONE))    { code ="11120"; strcpy(messageToSend, code.c_str()); return 'V'; }//V
      if (matchInputSignal(DOT, DASH, DASH, NONE, NONE))  { code ="12200"; strcpy(messageToSend, code.c_str()); return 'W'; }//W
      if (matchInputSignal(DASH, DOT, DOT, DASH, NONE))   { code ="21120"; strcpy(messageToSend, code.c_str()); return 'X'; }//X
      if (matchInputSignal(DASH, DOT, DASH, DASH, NONE))  { code ="21220"; strcpy(messageToSend, code.c_str()); return 'Y'; }//Y
      if (matchInputSignal(DASH, DASH, DOT, DOT, NONE))   { code ="22110"; strcpy(messageToSend, code.c_str()); return 'Z'; }//Z
      if (matchInputSignal(DOT, DASH, DASH, DASH, DASH))  { code ="12222"; strcpy(messageToSend, code.c_str()); return '1'; }//1
      if (matchInputSignal(DOT, DOT, DASH, DASH, DASH))   { code ="11222"; strcpy(messageToSend, code.c_str()); return '2'; }//2
      if (matchInputSignal(DOT, DOT, DOT, DASH, DASH))    { code ="11122"; strcpy(messageToSend, code.c_str()); return '3'; }//3
      if (matchInputSignal(DOT, DOT, DOT, DOT, DASH))     { code ="11112"; strcpy(messageToSend, code.c_str()); return '4'; }//4
      if (matchInputSignal(DOT, DOT, DOT, DOT, DOT))      { code ="11111"; strcpy(messageToSend, code.c_str()); return '5'; }//5
      if (matchInputSignal(DASH, DOT, DOT, DOT, DOT))     { code ="21111"; strcpy(messageToSend, code.c_str()); return '6'; }//6
      if (matchInputSignal(DASH, DASH, DOT, DOT, DOT))    { code ="22111"; strcpy(messageToSend, code.c_str()); return '7'; }//7
      if (matchInputSignal(DASH, DASH, DASH, DOT, DOT))   { code ="22211"; strcpy(messageToSend, code.c_str()); return '8'; }//8
      if (matchInputSignal(DASH, DASH, DASH, DASH, DOT))  { code ="22221"; strcpy(messageToSend, code.c_str()); return '9'; }//9
      if (matchInputSignal(DASH, DASH, DASH, DASH, DASH)) { code ="22222"; strcpy(messageToSend, code.c_str()); return '0'; }//0
      return '?';
    }
