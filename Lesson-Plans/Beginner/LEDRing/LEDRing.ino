/*
The LED’s on the ring are connected from Digital Pin 1 to Digital pin 12 of the Arduino.
First of all we need to include the idiotware library to our sketch.
We need to initialize this leds in void setup() function of arduino sketch.
The void loop() runs over and over again, repeatedly till power is removed. 
We will use the function for example the sweep(1). The “1” means how many repetitions you wish 
to do for that animation.This value can only be whole numbers, and not with decimal points.
*/
const int led_Pin_Count = 13; // // the number of leds (i.e. the length of the array)
int led_Pins[] = {1,1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,12}; //an array of pin numbers to which LEDs are attached

void setup()
    {
     for(int i = 0; i < led_Pin_Count; i++)   // set all LEDs to output
        {
          pinMode(led_Pins[i], OUTPUT);
          digitalWrite(led_Pins[i], LOW);
        } 
    }

void loop()
     {
       sweep(2);       //2 replicates of the sweep sequence
       circle(2);      //2 replicates of the circle sequence
       halfCircle(2);  //4 replicates of the halfCircle sequence
       curtain(1);    
       fastSlow(1);   
     }



void curtain(int Reps)
    {
     for(int k=1; k<(Reps+1); k++)
        {
          for(int j=1; j<257; j=j+j)
             {
               int LEDCounter=1;
               int LEDSwitch=1;
               for(int i=1; i>0 && i<14; i=i+LEDCounter)
                  {
                    if(LEDSwitch)
                      {
                        digitalWrite(led_Pins[i], HIGH);
                        delay(j);
                      }
                    else
                      {
                        digitalWrite(led_Pins[i], LOW);
                        delay(j);
                      }
                    if(i>12)
                      {
                        LEDCounter*=-1;
                        LEDSwitch=0;
                        digitalWrite(13, LOW);
                        delay(j);
                      }
                  }
             }
        }
    }

void fastSlow(int Reps)
     {
       for(int k=1; k<(Reps+1); k++)
          {
            for(int j=1; j<257; j=j+j)
               {
                 int LEDCounter=1;
                 for(int i=1; i>0 && i<14; i=i+LEDCounter)
                    {
                      blink(i, j, 10); //Call the blink method below
                      if(i>12)
                        {
                          LEDCounter*=-1;
                        }
                    }
               }
          }
      }


void blink(int LEDPin, int LEDOnTime, int LEDOffTime)
          {
            digitalWrite(led_Pins[LEDPin], HIGH);
            delay(LEDOnTime);
            digitalWrite(led_Pins[LEDPin], LOW);
            delay(LEDOffTime);
          }
 
 
void sweep(int Reps)
    {
      for(int k=1; k<(Reps+1); k++)
         {
           for(int i=0; i<=led_Pin_Count; i++)
              {
                digitalWrite(led_Pins[i],HIGH);
                delay(100);
              }
           for(int i=0; i<led_Pin_Count; i++)
              {
                digitalWrite(led_Pins[i],LOW);
                delay(100);
              }  
         }
       for(int k=1; k<(Reps+1); k++)
         {
           for(int i=12; i>=0; i--)
              {
                digitalWrite(led_Pins[i],HIGH);
                delay(100);
              }
           for(int i=12; i>=0; i--)
              {
                digitalWrite(led_Pins[i],LOW);
                delay(100);
              }  
         }  
    }  
  
void circle(int Reps)
    { 
      for(int k=1; k<(Reps+1); k++)
         {
           for(int i= 0,j=12; i<6,j>=6; i++,j--)
              {
                digitalWrite(led_Pins[i],HIGH); 
                digitalWrite(led_Pins[j],HIGH);
                delay(100);
              }
           for(int i= 12,j=0; i>6,j<=led_Pin_Count; i--,j++)
              {
                digitalWrite(led_Pins[i],LOW); 
                digitalWrite(led_Pins[j],LOW);
                delay(100);
              }
              
            for(int i= 0,j=12; i<6,j>=6; i++,j--)
              {
                digitalWrite(led_Pins[i],HIGH); 
                digitalWrite(led_Pins[j],HIGH);
                delay(100);
              }
           for(int i= 6,j=6; i>6,j<=led_Pin_Count; i--,j++)
              {
                digitalWrite(led_Pins[i],LOW); 
                digitalWrite(led_Pins[j],LOW);
                delay(100);
              }  
            
        }    
    }  
 
 void halfCircle(int Reps)
    { 
      for(int k=1; k<(Reps+1); k++)
         {
           for(int i=0,j=6; i<=6,j<=12;i++,j++)
              {
                digitalWrite(led_Pins[i],HIGH); 
                digitalWrite(led_Pins[j],HIGH);
                delay(100);
              }
           for(int i=6,j=0; i<=12,j<=6;i++,j++)
              {
                digitalWrite(led_Pins[i],LOW); 
                digitalWrite(led_Pins[j],LOW);
                delay(100);
              } 

           for(int i=12,j=6; i>=6,j>=0; i--,j--)
              {
                digitalWrite(led_Pins[i],HIGH); 
                digitalWrite(led_Pins[j],HIGH);
                delay(100);
              }
           for(int i=12,j=6; i>=6,j>=0;i--,j--)
              {
                digitalWrite(led_Pins[i],LOW); 
                digitalWrite(led_Pins[j],LOW);
                delay(100);
              }      
          } 
    }    
        
