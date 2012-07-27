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

//Defining statements for pin manipulation
#define MOTOR_HIGH PORTB|=_bitmaskMotor
#define MOTOR_LOW PORTB&=~_bitmaskMotor
#define DIR_HIGH PORTB|=_bitmaskDir
#define DIR_LOW PORTB&=~_bitmaskDir



Floppy::Floppy(byte motorPin, byte directionPin){
	
	//Set up bitmask for pin control
	//with 1 marking the pin status bit
	_bitmaskMotor = 1<<motorPin;
	_bitmaskDir = 1<<directionPin;

	//Set direction and motor pins to ouputs
	DDRB|=(_bitmaskDir|_bitmaskMotor);

	//Set both pins to low
	PORTB&=~(_bitmaskDir|_bitmaskMotor);

	//Initialise tracking variables
	_ticks = 0;
	_position = 0;
	_period = 0;
	_stateMotor= 0;
	_stateDir = 1;

	//Make sure the floppy starts from the start!
	reset();
}


void Floppy::incrementState() {

	//Only really increment if the floppy actually
	//making a note
	if (_period!=0){

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


void Floppy::changeState() {
	
	//Change direction of steps if needed
	
	//If at end...	
	if (_position>=158){
		//Set dir pin to high and set state
		DIR_HIGH;
		_stateDir=0;
	}
	
	//Or if at start...
	//Note: comparator is "<=" as _position is byte and unsigned!
	else if (_position<=0){
		//Set dir pin to low and set state
		DIR_LOW;
		_stateDir=1;
	}

	//Change the position tracking variable
	//(with direction considered)
	_position+=(int(_stateDir)*2)-1;
	
	//Actually toggle stepper motor pin and state var
	if (_stateMotor){
		MOTOR_HIGH;
		_stateMotor=0;	
	}
	else {
		MOTOR_LOW;
		_stateMotor=1;
	}
}	

void Floppy::setPeriod(unsigned int period) {
	_period = period;
}

void Floppy::reset(){

	//Set backwards (set dir to HIGH)
	PORTD|=_bitmaskDir;
	_stateDir=1;

	//Repeatedly pulse motor control
	for (int i=0;i<80;i++){
		MOTOR_HIGH;
		//delay(1);
		MOTOR_LOW;
	}

	//Update motor state just in case
	_stateMotor=0;
}
