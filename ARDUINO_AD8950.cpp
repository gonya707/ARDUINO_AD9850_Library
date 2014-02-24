#include "ARDUINO_AD9850.h"
#include "Arduino.h"

//Constructor for the class AD9850
AD9850::AD9850(int givenW_CLK, int givenFQ_UD, int givenDATA, int givenRESET){
	W_CLK = givenW_CLK;
   	FQ_UD = givenFQ_UD;
   	DATA = givenDATA;
   	RESET = givenRESET;
	Serial.println("AD9850 instance created");
}

void AD9850::init(){
	digitalWrite(W_CLK, LOW);
	digitalWrite(FQ_UD, LOW);
	digitalWrite(DATA, LOW);
	digitalWrite(RESET, LOW);
	Serial.println("All the pins lowered.");
}

/* Reset operation for the AD9850. This function must be called before using AD9850_Osc
 * first time in the execution (check page 12 on datasheet)
 * */
void AD9850::doReset(){
	digitalWrite(W_CLK, LOW);
	digitalWrite(W_CLK, HIGH);
	digitalWrite(W_CLK, LOW);
	
	digitalWrite(RESET, LOW);
	digitalWrite(RESET, HIGH);
	digitalWrite(RESET, LOW);
	
	digitalWrite(FQ_UD, LOW);
	digitalWrite(FQ_UD, HIGH);
	digitalWrite(FQ_UD, LOW);
	
	osc(0, 0);
	
	Serial.println("Reset succesfull.");
}

/* Sets the DDS sine and square oscillator to the detailed "frequency" and "phase" variables.
 * "frequency" will be turned into a 32 bit word, so the frequency resolution of 0.0291 Hz
 * with a 125 MHz reference clock. "phase" will be a 5 bit word instead so the resolution is
 * 11.5 degrees, or pi/32 radians.
 */
void AD9850::osc(double frequency, double phase){
  
    Serial.println("Changing frequency to:");
    Serial.println(frequency);

	long y = (long)frequency * FREQ_FACTOR / XTAL_MHZ;
	while(phase > 360)
		phase -= 360;
	long z = phase / 11.5;
        
    Serial.println("Frequency word:");
    Serial.println(y);
	
	int i;
        
	//Frequency 32-bit word
	for (i = 0; i < 32; i++){
	        digitalWrite(DATA, (y >> i) & 0x01);
		digitalWrite(W_CLK, HIGH);
		digitalWrite(W_CLK, LOW);
	}
	
	//control bit #1, control bit #2 and Power off, all to low
	digitalWrite(DATA, LOW);
	digitalWrite(W_CLK, HIGH);
	digitalWrite(W_CLK, LOW);
	digitalWrite(W_CLK, HIGH);
	digitalWrite(W_CLK, LOW);
	digitalWrite(W_CLK, HIGH);
	digitalWrite(W_CLK, LOW);
	
	//phase 5-bit word
	for (i = 0; i < 5; i++){
                digitalWrite(DATA, (z >> i) & 0x01);
		digitalWrite(W_CLK, HIGH);
		digitalWrite(W_CLK, LOW);
	}

	digitalWrite(FQ_UD, HIGH);
	digitalWrite(FQ_UD, LOW);
	
	Serial.println("Done.");
}

/* Enables power down mode. This method is used for a quick "all outputs" disable.
 * The effect is the same as AD9850_Osc(0,0), but it takes less clock cycles
 */
void AD9850::powerDown(){
	Serial.println("Changing mode to power down.");
	int PDword = 0x04;
	int i;
	digitalWrite(FQ_UD, HIGH);
	digitalWrite(FQ_UD, LOW);

	for (i = 0; i < 8; i++){
		if((PDword >> i) & 0x01)
			digitalWrite(DATA, HIGH);
		else
			digitalWrite(DATA, LOW);
		digitalWrite(W_CLK, HIGH);
		digitalWrite(W_CLK, LOW);
	}

	digitalWrite(FQ_UD, HIGH);
	digitalWrite(FQ_UD, LOW);
	
	Serial.println("Done.");
}

/* Performs a frequency sweep increased in "inc"Hz steps. The frequency order is from low to high
 * and resets to the lower frequency when maximum frequency is reached.
 */
void AD9850::sweepUp(double minFreq, double maxFreq, double inc, int microsPerDelay){
   Serial.println("Shutting down serial communication.");
   Serial.end();
   double f = minFreq;
   int i;
   while (1){
      osc(f, 0);
      f += inc;
      if (f > maxFreq)
         f = minFreq;
   }
}

/* Performs a frequency sweep decreased in "inc"Hz steps. The frequency order is from high to low
 * and resets to the higher frequency when minimum frequency is reached.
 */
void AD9850::sweepDown(double minFreq, double maxFreq, double inc, int microsPerDelay){
   double f = maxFreq;
   int i;
   while (1){
      osc(f, 0);
      f -= inc;
      if (f < minFreq)
         f = maxFreq;
      delayMicroseconds(microsPerDelay);
   }
}

/* Performs a frequency sweep increased in "inc"Hz steps. The frequency order is from low to high,
 * but it changes to from high to low when the maximum frequency is reached and so on.
 */
void AD9850::sweepLoop(double minFreq, double maxFreq, double inc, int microsPerDelay){
   double f = minFreq;
   int i;
   while (1){
      osc(f, 0);
      f += inc;
      if (f > maxFreq || f < minFreq)
         inc =- inc;
      delayMicroseconds(microsPerDelay);
   }
}



