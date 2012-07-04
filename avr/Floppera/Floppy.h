/* 
Floppy.h
Craig Ramsay

Used to track the state of a pair of
control pins for the floppy drive and
provide a clean looking way of pulsing
the stepper motor from the main code
*/

#ifndef FLOPPY_H_
#define FLOPPY_H_

#include "Arduino.h"

class Floppy {
	private:
		byte _motorPin;
		byte _directionPin;
		byte _position;
		byte _bitmaskMotor;
		byte _bitmaskDir;
		bool stateMotor;
		bool stateDir;
		unsigned int _period;
		unsigned int _ticks;
		void changeState();
	public:
		Floppy(byte motorPin, byte directionPin);
		void incrementState();
		void setPeriod(unsigned int period)
		void reset();
		//void print();
};

#endif /* FLOPPY_H_ */
