//Includes
#include "TimerOne.h"
#include "pgmspace.h"
#include "Floppy.h"
#include "songData.h"

//Constant for interval of "ticks" from TimerOne lib
#define RESOLUTION 40

//Declare floppy array 
//NOTE: These pin numbers are relative to the "B" port registers
//so 0->5 maps to pins 8->13 as marked on the arduino board
Floppy _drives[]={Floppy(0,1),Floppy(2,3),Floppy(4,5)};

//Set up some variables to track the next note properties
unsigned long _startTime=0;
bool _playSong=true;

//Setup function called upon boot
void setup(){

	Serial.begin(9600);

  	//Set up TimerOne library to call "updateFloppys" at a set interval
  	Timer1.initialize(RESOLUTION); 
  	Timer1.attachInterrupt(updateFloppys);

  	//Set start time to now!
  	_startTime = millis();

}

//Main loop
void loop(){
	
	static unsigned int noteIndex=0;
	static unsigned char noteTrack=0;
	static unsigned int notePeriod=0;
	static unsigned long noteTime=0;
	
	//If the song is not finished and is set to play then...
	if ((noteIndex<_songDataLength)&&(_playSong)){
		
		//While notes still need to be processed...
		while (millis() >=noteTime){	
			//Set the floppy period from the pasred data
			setFloppyNote(noteTrack,notePeriod);
		
			//Parse the next note's data from array
			parseNextNote(noteTrack,notePeriod,noteTime,noteIndex);
		}
	
		//Wait until the next note is due
		delay(noteTime-millis());
	}
	
	//If this the song has just ended (playSong is still true)
	else if ((noteIndex>=_songDataLength)&&(_playSong)){
		Serial.print("ending branch");		

		//Set playSong to false and reset the noteIndex to 0
		_playSong=false;
		noteIndex=0;
		
		//Set all drives to a period of zero
		for (int i=0;i<(sizeof(_drives)/sizeof(Floppy));i++){
			_drives[i].setPeriod(0);
		}
			
		updateFloppys();
			
		//Also, detach the timerOne lib from the update floppy 
		//function to save power
		Timer1.detachInterrupt();
		}

	//Else (the song does not need played) ...
	else {
	
		//Wait longer than usual to minimize power consumption
		delay(2000);
	}
	
}


void updateFloppys() {

	//Loop through all floppy objects in array and call the increment
	//state function
	for (int i=0;i<(sizeof(_drives)/sizeof(Floppy));i++){
		_drives[i].incrementState();
	}
	
}

void parseNextNote(unsigned char &noteTrack, unsigned int &notePeriod, unsigned long &noteTime, unsigned int &noteIndex){
	
	//Parse data from song array
	//NOTE: pgm_read_dword is a function from pgmspace.h as _songData
	//currently resides in the program space in flash memory. The address
	//of each element is then passed, hence the "&"
	noteTrack = pgm_read_dword_near(&_songData[noteIndex])&3;
	notePeriod = pgm_read_dword_near(&_songData[noteIndex])>> 24;
	noteTime = ((pgm_read_dword_near(&_songData[noteIndex])>> 2)&0x003FFFFF)+_startTime;
	
	//Increment index for next function call
	noteIndex++;
}

void setFloppyNote(int index, unsigned int period){

	//Call setter function for period of floppy object
	_drives[index].setPeriod(period); 	
}


