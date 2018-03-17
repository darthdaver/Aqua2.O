
/*
 * Authors: Emanuele Falzone, Davide Molinelli
 * 
 * Digital Pin 2: Water-flow Sensor Cold Water
 * Digital Pin 3: Water-flow Sensor Warm Water
 * 
 */
 
#include <WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <SD.h>

#define SENSOR_CSV "remote.csv"

int status = WL_IDLE_STATUS;
char ssid[] = "AndroidAP";
char pass[] = "ciaociao1";

unsigned int localPort = 5000;

File sensorData;

byte sdPin = 4;

byte sensorInterruptCold = 0;  // 0 = digital pin 2
byte sensorInterruptWarm = 1;  // 1 = digital pin 3

volatile byte pulseCountCold = 0;
volatile byte pulseCountWarm = 0;

volatile float flowRateCold = 0;
volatile float flowRateWarm = 0;

unsigned long oldTime = 0;

char packet[50];

volatile long cycle = 1;
volatile int days = 0;
String arduinoId = "6";
String content = "";

WiFiUDP Udp;
IPAddress server(52,11,174,171);

void setup() {
    
    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
  
    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
      
        Serial.println(F("WiFi shield not present"));
        
        // don't continue:
        while (true);
    }
  
    // check firmware version
    String fv = WiFi.firmwareVersion();
    if (fv != "1.1.0") {
        Serial.println(F("Please upgrade the firmware"));
    }
  
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print(F("Attempting to connect to SSID: "));
        Serial.println(ssid);
        
        // Connect to WPA/WPA2 network
        status = WiFi.begin(ssid,pass);
        
        // wait 10 seconds for connection:
        delay(10000);
    }
    
    Serial.println(F("Connected to wifi"));
    printWifiStatus();
  
    Serial.println(F("\nStarting connection to server..."));
    // if you get a connection, report back via serial:
    Udp.begin(localPort);
  
    Serial.print(F("Initializing SD card..."));
  
    if (!SD.begin(sdPin)) {
        Serial.println(F("initialization failed!"));
    }
    
    Serial.println(F("initialization done."));
  
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    if(!SD.exists(SENSOR_CSV)){
   
        sensorData = SD.open(SENSOR_CSV, FILE_WRITE);
        if(sensorData){
            sensorData.println(F("ArduinoId,Day,Second,FlowRateCold(L/min),FlowRateWarm(L/min)"));
            sensorData.close();
            Serial.println(F("Created"));
        }
    } else {
      
        Serial.println(F("Open file"));
    }
  
    // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
    // Configured to trigger on a FALLING state change (transition from HIGH
    // state to LOW state)
    attachInterrupt(sensorInterruptCold, pulseCounterCold, FALLING);
    attachInterrupt(sensorInterruptWarm, pulseCounterWarm, FALLING);
}

void loop() {

    if((millis() - oldTime) > 1000){
        // Disable the interrupt while calculating flow rate and sending the value to
        // the host
        detachInterrupt(sensorInterruptCold);
        detachInterrupt(sensorInterruptWarm);
    
        // Because this loop may not complete in exactly 1 second intervals we calculate
        // the number of milliseconds that have passed since the last execution and use
        // that to scale the output. We also apply the calibrationFactor to scale the output
        // based on the number of pulses per second per units of measure (litres/minute in
        // this case) coming from the sensor.
        flowRateCold = (1000.0 / (millis() - oldTime)) * pulseCountCold * 0.1765;
        flowRateWarm = (1000.0 / (millis() - oldTime)) * pulseCountWarm * 0.1765;
        
        if(flowRateCold != 0 || flowRateWarm != 0){
    
            Serial.print(flowRateCold);
            Serial.print(" - ");
            Serial.println(flowRateWarm);
            String content = arduinoId + "-" + String(flowRateCold) + "-" + String(flowRateWarm);
          
            //"arduinoId-flowRateCold-flowRateWarm";
            strcpy(packet, content.c_str());
          
            // send a reply, to the IP address and port that sent us the packet we received
            Udp.beginPacket(server, localPort);
            Udp.write(packet);
            Udp.endPacket();
      
            // convert to CSV
            saveData(); // save to SD card
        }
        
        // Reset the pulse counter so we can start incrementing again
        pulseCountCold = 0;
        pulseCountWarm = 0;
        
        oldTime = millis();
    
        cycle++;
    
        if(cycle == 86401){
            days++;
            cycle = 1;    
        }
    
        // Enable the interrupt again now that we've finished sending output
        attachInterrupt(sensorInterruptCold, pulseCounterCold, FALLING);
        attachInterrupt(sensorInterruptWarm, pulseCounterWarm, FALLING);
    }
}

void saveData(){
  
    if(SD.exists(SENSOR_CSV)) { // check the card is still there
      
        // now append new data file
        sensorData = SD.open(SENSOR_CSV, FILE_WRITE);
        
        if (sensorData){
          
            unsigned int frac;
      
            // Print the arduinoId
            sensorData.print(arduinoId);
            sensorData.print(",");
      
            // Print the days passed from the start of the measurements
            sensorData.print(days);
            sensorData.print(",");
      
            // Print the second passed from the start of the day
            sensorData.print(cycle);
            sensorData.print(",");
            
            // Print the flow rate for this second in litres / minute
            sensorData.print(int(flowRateCold));  // Print the integer part of the variable
            sensorData.print(".");             // Print the decimal point
            // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
            frac = (flowRateCold - int(flowRateCold)) * 10;
            sensorData.print(frac, DEC) ;
            sensorData.print(",");

            // Print the flow rate for this second in litres / minute
            sensorData.print(int(flowRateWarm));  // Print the integer part of the variable
            sensorData.print(".");             // Print the decimal point
            
            // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
            frac = (flowRateWarm - int(flowRateWarm)) * 10;
            sensorData.print(frac, DEC) ;
            sensorData.print("\n");
            
            sensorData.close(); // close the file
        }
    } else {
        Serial.println(F("Save Data Failed"));
    }
}

void printWifiStatus() {
  
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
  
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print(F("IP Address: "));
    Serial.println(ip);
  
    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print(F("signal strength (RSSI):"));
    Serial.print(rssi);
    Serial.println(F(" dBm"));
}


/*
Insterrupt Service Routines
 */
void pulseCounterCold() {
    // Increment the pulse counter of cold water
    pulseCountCold++;
}

void pulseCounterWarm() {
    // Increment the pulse counter of warm water
    pulseCountWarm++;
}
