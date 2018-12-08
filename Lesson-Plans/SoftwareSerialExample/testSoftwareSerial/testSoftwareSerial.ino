#include <SoftwareSerial.h>
const byte rxPin = 10;
const byte txPin = 9; 
SoftwareSerial dbg(rxPin,txPin);
void setup() {
  
  dbg.begin(38400);
}

void loop() {  
  dbg.println("Hello");
  delay(1000); 

}