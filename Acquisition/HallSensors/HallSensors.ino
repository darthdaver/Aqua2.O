/*
 * Authors: Emanuele Falzone, Davide Molinelli
 * 
 * Digital Pin 2: Water-flow Sensor
 * Analog Pin 0:  Additional Hall Sensor
 * Analog Pin 1:  Additional Hall Sensor
 * 
 * With serial monitor you can see the values read every second.
 * The same values are stored in a file in the SD card.
 */

#include <SPI.h>
#include <SD.h>

#define SENSOR_CSV "hall.csv"

File sensorData;

byte sdPin = 4;

byte sensorInterrupt = 0;  // 0 = digital pin 2
byte analogReadS2 = 0;
byte analogReadS3 = 1;

int pulseCountS1 = 0;
int pulseCountS2 = 0;
int pulseCountS3 = 0;

bool acceso2 = 0;
bool acceso3 = 0;

unsigned long oldTime;

void setup() {
  
    Serial.begin(9600);
    
    Serial.println(F("Initializing SD card..."));
  
    if (!SD.begin(sdPin)) {
        Serial.println(F("initialization failed!"));
    }
    
    Serial.println(F("initialization done."));
  
    if(!SD.exists(SENSOR_CSV)){
      
        sensorData = SD.open(SENSOR_CSV, FILE_WRITE);
        
        if(sensorData){
            sensorData.println(F("S1,S2,S3"));
            sensorData.close();
            Serial.println(F("Created"));
        }
    } else {
        Serial.println(F("File already exists"));
    }
    
    attachInterrupt(sensorInterrupt, sensorPulseCounter, FALLING);
}

void DoMeasurement() {
  
    int raw2 = analogRead(analogReadS2);
    
    if (raw2 > 520 && acceso2 == false) {
          acceso2 = true;    
    } else if (raw2 < 480 && acceso2 == true) {
          acceso2 = false;
          pulseCountS2++;
    }
  
    int raw3 = analogRead(analogReadS3);
    
    if (raw3 > 520 && acceso3 == false) {
          acceso3 = true;    
    } else if (raw3 < 480 && acceso3 == true) {
          acceso3 = false;
          pulseCountS3++;
    }
}

void loop() 
{
    DoMeasurement();

    if((millis() - oldTime) > 1000){
      
        if(SD.exists(SENSOR_CSV)) { 
     
            sensorData = SD.open(SENSOR_CSV, FILE_WRITE);
            
            if (sensorData){
             
                sensorData.print(pulseCountS1);
                sensorData.print(",");
                
                sensorData.print(pulseCountS2);
                sensorData.print(",");
                
                sensorData.print(pulseCountS3);
                sensorData.print("\n");

                Serial.print(pulseCountS1);
                Serial.print("    ");
                
                Serial.print(pulseCountS2);
                Serial.print("    ");
                
                Serial.println(pulseCountS3);
           

                Serial.println(F("Data Saved!"));
            }

            sensorData.close();
            
        } else {
            Serial.println(F("Save Data Failed"));
        }
        
        pulseCountS1 = 0;
        pulseCountS2 = 0;
        pulseCountS3 = 0;

        oldTime = millis();
    }
}

void sensorPulseCounter()
{
  pulseCountS1++;
}

