/*

  Clock using 12 LEDs

 In this example 12 LEDs are used to show current time.  
 12 LEDs are connected in circualr pattern. All 12 LEDs represent 
 number 1 to 12 of clock respectively. Depending upon the current 
 time, LED of that position will glow.
 If current time is 4:15 then 4th and 3rd LED will glow.
 4th LED will indicate hour and 3rd LED will indicate minutes.
 LED which shows hours will glow conitinuously. 
 LED which shows minutes will blink with delay of 1 second.

*/

int ledPins[] = {1,2,3,4,5,6,7,8,9,10,11,12};   //an array of pin numbers to which LEDs are attached
int pinCount = 12;  // the number of pins (i.e. the length of the array)

void setup() 
    { 
       for (int thisPin = 0; thisPin < pinCount; thisPin++)  // the array elements are numbered from 0 to (pinCount - 1).
          {
            pinMode(ledPins[thisPin],OUTPUT);   // use a for loop to initialize each pin as an output:
          }
    }
  
  //thhis is a list of int variables used in this clock program
unsigned long elapsedSeconds=0;
unsigned long hour=0;
unsigned long minute=0;
unsigned long initialHours =12;      //variable to initiate hours
unsigned long initialMinutes = 59;    //variable to initiate minutes
unsigned long initialSeconds = 50;    //variable to initiate seconds


int ledState = LOW;                   // ledState used to set the LED
unsigned long previousMillis = 0; // will store last time LED was updated
    // constants won't change :
const long interval_1 = 1000;     // interval at which to blink (milliseconds)
const long interval_2 = 250;
   
 //this method is for elapsed time in seconds
 unsigned long seconds()
    {  
       elapsedSeconds = initialHours * 3600;
       elapsedSeconds = elapsedSeconds + (initialMinutes*60);
       elapsedSeconds = elapsedSeconds + initialSeconds; 
       elapsedSeconds = elapsedSeconds + (millis()/1000);
       return elapsedSeconds;
    }
//this method is for hours
int hours()
   {
        hour = seconds();
        hour = hour/3600;
        hour = hour % 12;
        return hour;
   }
//this method is for minutes
int minutes()
   {
       minute = seconds();
       minute = minute/60;
       minute = minute%60;
       return minute;
   }


//this loop will conintue to keep looping so that the time can go as follows
void loop()
   {
     digitalClockDisplay(); //by calling this function particular LED will turn ON and OFF depending upon time
    
   }

//this method is for blink LED withot delay
unsigned long blinkWithoutDelay(const long interval)
   {
     unsigned long currentMillis = millis();

     if (currentMillis - previousMillis >= interval) 
        {
         // save the last time you blinked the LED
         previousMillis = currentMillis;

         // if the LED is off turn it on and vice-versa:
         if (ledState == LOW) 
            {
              ledState = HIGH;
            } 
          else 
            {
              ledState = LOW;
            }
         }
   }

// this method is to turn ON and turn OFF particular LED according to time
void digitalClockDisplay()
   {

    int minuteLed = minutes() / 5; //dividing minutes by 5 will give the value between 1 to 12
    int hourLed = hours();         // houeLed varible will store the value of currnet hour

    if (minuteLed == hourLed)                     // if current hours and minutes are equal 
       {                                          // then LED which will indicate hours and minutes                                                      
           blinkWithoutDelay(interval_2);         // which is same will blink with delay of 250 miliseconds.
                                                  // for example: 2:10, 3:15, 4:20, 6:30, 9:45
           digitalWrite(ledPins[hourLed - 1 ], ledState); 
           digitalWrite(ledPins[hourLed], LOW); 
       }
       
           
    else                                            // if current hours and minutes are diffrent 
       {                                            // then LED which will indicate hour will
          blinkWithoutDelay(interval_1);            // glow continously and LED which will indicate 
                                                    // minutes will blink with delay of one second.
          digitalWrite(ledPins[hourLed - 1], HIGH);     
          digitalWrite(ledPins[hourLed ], LOW);   
          digitalWrite(ledPins[minuteLed], ledState); 
          digitalWrite(ledPins[minuteLed - 1], LOW);
           
        }  
        
   }
   
