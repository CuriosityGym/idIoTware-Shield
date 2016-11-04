/*
 SD Card Temperature Log
 This example demonstrates how to log and save data from idIoTware Shield Integrated Temperature Sensor to a SD card using the SDFAT library.
 In this example we have created a CSV file with the Temperature Values for every one minute for 15 minutes. This interval rate and duration is configurable.
 If there is existing file, then it will create new file with different name.
 
  
 The circuit:
 * LM35 sensor on analog pin A3
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 6
 */

/* Include Necessary Libraries*/
#include <SPI.h>
#include <SdFat.h>

SdFat sd; //Initialise an Object of SDFat Type.
const uint8_t chipSelect = 6;

//const int chipSelect = 4;   //cs pin of SD card shield
int tempPin = A0;      // LM35 is connected to A0 pin.
int buzzerPin = A1;    // buzzer is connected to A1 pin

File dataFile;   // a handle to the logging file
char filename[] = "Temp000.CSV"; // First file that will be created.
float temperatureValue;
boolean logCelsius=true;
unsigned long timeCount=0;
int samplingTime = 60;  //this variable is interval(in Seconds) at which you want to log the data to SD card.
int duration = 15;     //this variable is duration(in Minutes) which is the total time for which you want to log data.


void setup() 
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
      {
        ; // wait for serial port to connect. Needed for native USB port only
      }

  Serial.print("Initializing SD card...");

 // see if the card is present and can be initialized:
 if (!sd.begin(chipSelect))  
    { 
      sd.initErrorHalt();
      Serial.println("Card failed, or not present");
      // don't do anything more:
      return;
    }
  Serial.println("card initialized.");

 // create a new file
 for(unsigned int i = 0; i < 1000; i++) 
{
      filename[4] = i/100 + '0';
      filename[5] = ((i%100)/10) + '0';
      filename[6] = i%10 + '0';
      if(!sd.exists(filename)) 
        {
          // only open a new file if it doesn't exist
          dataFile = sd.open(filename, FILE_WRITE); // Open the File to write to it. 
          break;  // leave the loop!
        }
}

if(!dataFile) // Program could not create the file, hence it alerts the user. 
{ 
  //alert user
  Serial.println("Couldnt Create File");
  int alertAlarm1[] = { 1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
                   1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,};
  int noteDurations[] = { 5,5,5,5,5,5,5,5,5,5,5,
                          5,5,5,5,5,5,5,5,5,5,5 };
  for (int thisNote = 0; thisNote < 20; thisNote++) 
  {
     // Duration = 1 second / note type
     // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
     int noteDuration = 2500 / noteDurations[thisNote];
     tone(A1,alertAlarm1[thisNote], noteDuration);
     //pause for the note's duration plus 100 ms:
     delay(noteDuration + 100);
  }
}


  // check availble space on SD Card
  uint32_t freeKB = sd.vol()->freeClusterCount();
  freeKB *= sd.vol()->blocksPerCluster()/2;
  Serial.print("Free space KB: ");
  Serial.println(freeKB);
  uint32_t freeMB= freeKB/1024;
  Serial.print("Free space in MB: ");
  Serial.println(freeMB); 
  
  if(freeKB <= 500)
  {
    Serial.println("LOW SPACE!!!");
    int alertAlarm2[] = { 1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
                         1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
    int noteDurations[] = { 15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
                            15,15,15,15,15,15,15,15,15,15,15,15,15,15,15  };
    for (int thisNote = 0; thisNote < 20; thisNote++) 
   {
     // Duration = 1 second / note type
     // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
     int noteDuration = 2500 / noteDurations[thisNote];
     tone(A1,alertAlarm2[thisNote], noteDuration);
     //pause for the note's duration plus 50 ms:
     delay(noteDuration + 50);
   }
  }
 duration *= 60;   //convert durartion in minutes to seconds   

 Serial.print("Logging to: ");
 Serial.println(filename);
 
dataFile = sd.open(filename, FILE_WRITE);
 // if the file is available, write to it:
  if (dataFile) 
  { 
     dataFile.print("Logging Temperature for ");
     dataFile.print(duration);
     dataFile.print(" minutes at interval of ");
     dataFile.print(samplingTime);
     dataFile.println(" seconds.");     
     
  }
dataFile.close();
}

void loop() 
{
  dataSamples();    
}

// this method will log data to SD card at particular interval and for paricular duration
int dataSamples()
   { 
     // here we are logging data at interval of 1 minute for 15 mintutes, i.e, 15 samples.
     // if you want to save data for 2 hours then simply multiply 2 by 60 which will give 
     // you value of 120 minutes then change the variable duration to 120. 

      
      
      unsigned long  elapsedTime = millis()/1000; // this variable will keep track of elapsed time
      while(((millis()/1000)-elapsedTime) < 1);   // this loop will do nothing until a second has passed 
      timeCount++;                                     //increment time after each second.
    // This conditions means that the duration is greater than timecount, and timecount is a multiple of sampling time
      if((duration >= timeCount) && (timeCount % samplingTime == 0))
        {
          if(logCelsius)
          {
            temperatureValue = ( 5.0 * analogRead(tempPin) * 100.0) / 1024.0;
          }
      
          if(!logCelsius)
          {
            temperatureValue = ((temperatureValue*9)/5) + 32;
          }
       
        
          LogToSDcard();  //Log to SD using commands under void LogToSD()  
          // print to the serial port too:              
          Serial.print("Temperature: ");
          Serial.print(temperatureValue);
          Serial.print(char(176));
          if(logCelsius)
          {
            Serial.println("C");
          }   
          if(!logCelsius)
          {
            Serial.println("F");
          }       
        }
           
   }
   
void LogToSDcard()
   {
     // open the file. note that only one file can be open at a time,
     // so you have to close this one before opening another.
     dataFile = sd.open(filename, FILE_WRITE);
     // if the file is available, write to it:
     if (dataFile) 
       { 
         dataFile.print(temperatureValue);   
         if(logCelsius)
          {
            dataFile.println("°C");
          }   
          if(!logCelsius)
          {
            dataFile.println("°F");
          }   
         
         dataFile.println(",");
         dataFile.close();
         
     
       } 
  
      //if the file isn't open, pop up an error:
     else 
        {
          Serial.println("Error opening Temp000.CSV");
          delay(2000);
        }
   }   
 