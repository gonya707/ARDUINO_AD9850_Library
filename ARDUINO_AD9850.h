/*   Library for Arduino Uno board and compatible devices. (probably all the Arduino boards)
 *
 *    This library contains several functions for using with a DDS generator
 *    AD9850 module. All the functions are intended to work with this device
 *    in SERIAL mode, so you need to connect the parallel pins D0 and D1 to VCC
 *    and D2 GND, as written in the AD9850 datasheet. For further information
 *    check the Analog Devices website.
 *
 *    Coded by Gonzalo Recio
 *
 *	  http://grsynth.com
 */


#ifndef ARDUINO_AD9850_H
	#define ARDUINO_AD9850_H

	//Frequency of your crystal oscillator (CLKIN input pin 9 in datasheet), measured in MHz.
	// This reference frequency must be higher than 1MHz.
	#define XTAL_MHZ 125

	//Relationship value between actual frequency and 32-bit word sent in the serial streaming
	#define FREQ_FACTOR 4294.967295
	
	//interchangeable pins
	#define clk 4
	#define fqud 7
	#define data 8
	#define reset 12

	class AD9850{
		 public:
			AD9850(int givenW_CLK, int givenFQ_UD, int givenDATA, int givenRESET);
			void init();
			void doReset();
			void osc(double Freq,double phase);
			void sweepUp(double minFreq, double maxFreq, double inc, int cyclesPerDelay);
			void sweepDown(double minFreq, double maxFreq, double inc, int cyclesPerDelay);
			void sweepLoop(double minFreq, double maxFreq, double inc, int cyclesPerDelay);
			void powerDown();
		 private:
			int W_CLK;
			int FQ_UD;
			int DATA;
			int RESET;
	};

#endif
