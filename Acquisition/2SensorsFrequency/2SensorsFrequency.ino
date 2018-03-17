/*
 * Authors: Emanuele Falzone, Davide Molinelli
 * 
 * Digital Pin 2: Water-flow Sensor
 * Digital Pin 3: Water-flow Sensor
 * 
 * With serial monitor you can see the pulses per second for each water-flow sensor
 * and the cumulative counter of the pulses.
 */

byte sensorInterrupt_s1 = 0;  // 0 = digital pin 2
byte sensorInterrupt_s2 = 1;  // 1 = digital pin 3

volatile int pulseCount_s1;
volatile int pulseCount_s2;

int pulseSec_s1;
int pulseSec_s2;

unsigned long oldTime;
unsigned long totalTime;

void setup() {

    Serial.begin(38400);
  
    oldTime              = 0;
    totalTime            = 0;
    
    pulseSec_s1          = 0;
    pulseCount_s1        = 0;
  
    pulseSec_s2          = 0;
    pulseCount_s2        = 0;

    attachInterrupt(sensorInterrupt_s1, pulseCounter_s1, FALLING);
    attachInterrupt(sensorInterrupt_s2, pulseCounter_s2, FALLING);
}

void loop() {

    if((millis() - oldTime) > 1000) {
      
        totalTime += (millis() - oldTime)/1000;
        
        if(pulseSec_s1 > 0) {
          
            Serial.print("Pulse/sec_s1: ");
            Serial.print(int(pulseSec_s1));
            
            Serial.print("        PulseCount_s1: ");
            Serial.println(int(pulseCount_s1));
        }
        
        if(pulseSec_s2 > 0) {

            Serial.print("        Pulse/sec_s2: ");
            Serial.print(int(pulseSec_s2));
            Serial.print("        PulseCount_s2: ");
            Serial.println(int(pulseCount_s2));
        }
        
        oldTime = millis();
        
        pulseSec_s1 = 0;
        pulseSec_s2 = 0;
    }
}

void pulseCounter_s1() {
    pulseCount_s1++;
    pulseSec_s1++;
}

void pulseCounter_s2() {
    pulseCount_s2++;
    pulseSec_s2++;
}
