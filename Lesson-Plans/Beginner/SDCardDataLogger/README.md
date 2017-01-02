
 SD card temperature logger
 This example shows how to log data from LM35 temperature sensor
 to an SD card using the SdFat library.
 In this example we have created a CSV file with the temperature values
 every one minute for 15 minutes. This interval rate and duration is configurable.
 If there is existing file, then it will create new file with different name.
 
  
 The circuit:
 LM35 sensor on analog pin A3
 
 SD card attached to SPI bus as follows:
 MOSI - pin 11
 MISO - pin 12
 CLK - pin 13
 CS - pin 4
