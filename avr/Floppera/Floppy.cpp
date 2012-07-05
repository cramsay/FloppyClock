/*
Floppy.cpp
Craig Ramsay

Class for storing and controlling the state
of a floppy drive's stepper motor

NOTE: Pins must be from 0-7 as this deals directly
with the pin registers for speed, however there is no
validation on this as of yet...
*/

//Includes here
#include "Arduino.h"
#include "Floppy.h"

//Declaring class scope variables
byte _motorPin=0;
byte _directionPin=1;
byte _position=0;
byte _bitmaskMotor=0;
byte _bitmaskDir=0;
bool _stateMotor=0;
bool _stateDir=0;
unsigned int _period=0;
unsigned int _ticks=0;

Floppy::Floppy(byte motorPin, byte directionPin){

	//Set pin variables from args
	_motorPin = motorPin;
	_directionPin = directionPin;
	
	//Set up bitmask for pin control
	//with 1 marking the pin status bit
	_bitmaskMotor = 1<<motorPin;
	_bitmaskDir = 1<<directionPin;

	//Set direction and motor pins to ouputs
	DDRD|=(_bitmaskDir|_bitmaskMotor);

	//Set both pins to low
	PORTD&=~(_bitmaskDir|_bitmaskMotor);

	//Initialise tracking variables
	_ticks = 0;
	_position = 0;
	_stateMotor= 0;
	_stateDir = 0;

	//Make sure the floppy starts from the start!
	reset();
}


void Floppy::incrementState(){

	//Only really increment if the floppy actually
	//making a note
	if (_period>0){

		//Increment the tick count
		_ticks++;

		//If the period has been elapsed, change state
		//and reset tick counter
		if (_ticks>=_period){
			_ticks=0;
			changeState();		
		}	
	}
}


void Floppy::changeState(){
	
	//Change direction of steps if needed
	
	//If at end...	
	if (_position>=158){
		//Set dir pin to high and set state
		PORTD|=_bitmaskDir;
		_stateDir=1;
	}
	
	//Or if at start...
	else if (_position<=0){
		//Set dir pin to low and set state
		PORTD&=~_bitmaskDir;
		_stateDir=0;
	}

	//Change the position tracking variable
	//(with direction considered)
	_position+=(int(_stateDir)*2)-1;

	//Actually toggle stepper motor pin
	_stateMotor=~_stateMotor;
	if (_stateMotor)
		PORTD|=_bitmaskMotor;
	else
		PORTD&=~_bitmaskMotor;
}	

void Floppy::setPeriod(unsigned int period){
	_period = period;
}

void Floppy::reset(){

	//Set backwards (set dir to HIGH)
	PORTD|=_bitmaskDir;
	_stateDir=1;

	//Repeatedly pulse motor control
	for (int i=0;i<80;i++){
		PORTD|=_bitmaskMotor;
		delay(1);
		PORTD&=~_bitmaskMotor;
		delay(1);
	}

	//Update motor state just in case
	_stateMotor=0;
}
