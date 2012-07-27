//Includes
#include "TimerOne.h"
#include "pgmspace.h"
#include "Floppy.h"
#include "songData.h"

//Constant for interval of "ticks" from TimerOne lib
#define RESOLUTION 40

//Declare floppy array 
Floppy _drives[]={Floppy(0,1),Floppy(2,3),Floppy(4,5),Floppy(6,7)};

//Set up some variables to track the next note properties
unsigned long _startTime=0;
unsigned int _noteIndex=0;
unsigned char _noteTrack=0;
unsigned int _notePeriod=0;
unsigned long _noteTime=0;
bool _playSong=true;

//Setup function called upon boot
void setup(){

  	//Set up TimerOne library to call "updateFloppys" at a set interval
  	Timer1.initialize(RESOLUTION); 
  	Timer1.attachInterrupt(updateFloppys);

  	//Set start time to now!
  	_startTime = millis();

  	//Parse the first note
  	parseNextNote();

}

//Main loop
void loop(){
	
	//If the song is not finished and is set to play then...
	if ((_noteIndex<_songDataLength)&&(_playSong)){
		
		//While notes still need to be processed...
		while (millis() >=_noteTime){	
			//Set the floppy period from the pasred data
			setFloppyNote(_noteTrack,_notePeriod);
		
			//Parse the next note's data from array
			parseNextNote();
		}
	
		//Wait until the next note is due
		delay(_noteTime-millis());
	}
	
	//Else (the song does not need played) ...
	else {
	
		//Wait longer than usual to minimize power consumption
		delay(1000);
		//Set the play song flag to false (may have entered the branch by exceeding 
		//length of array)
		_playSong=false;
	}
	
}


void updateFloppys() {

	//Loop through all floppy objects in array and call the increment
	//state function
	for (int i=0;i<(sizeof(_drives)/sizeof(Floppy));i++){
		_drives[i].incrementState();
	}
	
}

void parseNextNote(){
	
	//Parse data from song array
	_noteTrack = pgm_read_dword_near(&_songData[_noteIndex])&3;
	_notePeriod = pgm_read_dword_near(&_songData[_noteIndex])>> 24;
	_noteTime = ((pgm_read_dword_near(&_songData[_noteIndex])>> 2)&0x003FFFFF)+_startTime;
	
	//Increment index for next function call
	_noteIndex++;
}

void setFloppyNote(int index, unsigned int period){

	//Call setter function for period of floppy object
	_drives[index].setPeriod(period); 	
}


