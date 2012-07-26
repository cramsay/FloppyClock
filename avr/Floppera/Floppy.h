/* 
Floppy.h
Craig Ramsay

Used to track the state of a pair of
control pins for the floppy drive and
provide a clean looking way of pulsing
the stepper motor from the main code
*/

#ifndef FLOPPYH
#define FLOPPYH

#include "Arduino.h"

class Floppy {
	private:
		byte _position;
		byte _bitmaskMotor;
		byte _bitmaskDir;
		bool _stateMotor;
		bool _stateDir;
		volatile unsigned int _period;
		unsigned int _ticks;
		void changeState();
	public:
		Floppy(byte motorPin, byte directionPin);
		void incrementState();
		void setPeriod(unsigned int period);
		void reset();
};

#endif /* FLOPPYH */
