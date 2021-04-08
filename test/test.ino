#include <SoftwareSerial.h>


static const int RXPin = 13, TXPin = 15;
static const uint32_t GPSBaud = 9600;

SoftwareSerial ss(RXPin, TXPin);

void setup(){
    Serial.begin(115200);
    delay(1000);
    ss.begin(9600);


}

void loop(){
    if (ss.available()){
        Serial.write(ss.read());
    } 

}