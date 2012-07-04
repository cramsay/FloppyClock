/* 
* Floppy.h
* Written: Craig Ramsay
*/

#ifndef FLOPPY_H_
#define FLOPPY_H_

using namespace std;

class Floppy {
	private:
		byte _motorPin;
		byte _directionPin;
		byte _position;
		unsigned int period;
		unsigned int ticks;
		void changeState();
	public:
		Floppy();
		void incrementState();
		void reset();
		//void print();
};

#endif /* FLOPPY_H_ */
