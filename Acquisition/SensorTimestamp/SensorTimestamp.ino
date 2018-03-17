#include <SPI.h>
#include "SdFat.h"
SdFat SD;

#define SENSOR_CSV "time.csv"

File sensorData;

byte sdPin = 4;
byte sensorInterrupt = 0;  // 0 = digital pin 2

volatile unsigned long oldTime;

volatile unsigned long buffer_timestamp[50]; 
volatile byte pos = 0;

volatile boolean turn = true;

void setup() {

    Serial.begin(38400);
  
    Serial.println(F("Initializing SD card..."));
  
    while(!Serial){}
    
    if (!SD.begin(sdPin)) {
        Serial.println(F("initialization failed!"));
    }
    
    Serial.println(F("initialization done."));

    // SD.remove(SENSOR_CSV);
    if(!SD.exists(SENSOR_CSV)){
       
        sensorData = SD.open(SENSOR_CSV, FILE_WRITE);
        if(sensorData){
            sensorData.println(F("Milliseconds"));
            Serial.println(F("File created"));
        }
    } else {
      
        Serial.println(F("File opened"));
    }
  
    attachInterrupt(sensorInterrupt, interruptRoutine, FALLING);
}

void loop() {

    if(millis() - oldTime > 1000){
      
        turn = false;
        sensorData.flush();
   
        for (byte i = 0; i < pos; i++){
            sensorData.println(buffer_timestamp[i]);
        }
        
        pos = 0;
        turn = true;
        oldTime = millis();
        Serial.print(F("."));
    }
}

void interruptRoutine() {
  
    if (turn == true) {
        sensorData.println(micros());
    } else {
        buffer_timestamp[pos] = micros();
        pos++;
    }
}

