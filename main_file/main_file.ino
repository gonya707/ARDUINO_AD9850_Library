#include "Arduino.h"
#include "ARDUINO_AD9850.h"

void setup(){
  
    Serial.begin(9600);
    while(!Serial) {}
    Serial.println("Starting...");
	
    pinMode(clk, OUTPUT);
    pinMode(fqud, OUTPUT);
    pinMode(data, OUTPUT);
    pinMode(reset, OUTPUT);
  
	AD9850 device(clk, fqud, data, reset);
	device.init();
	device.doReset();
        device.osc(12345, 0);
}

void loop(){} //do nothing
