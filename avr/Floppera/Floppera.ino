//Includes
#include  <TimerOne.h>
#include <Floppy.h>
//#include <songData.h>

//Constant for interval of "ticks" from TimerOne lib
#define RESOLUTION 40

//Declare floppy array 
Floppy drives[3]={Floppy(2,3),Floppy(4,5),Floppy(6,7)};

//Set up some variables to track the next note properties
unsigned long _startTime;
unsigned int _noteIndex=0;
byte _noteTrack=0;
byte _notePeriod=0;
unsigned long _noteTime=0;

//Function called upon boot
void setup(){
	
	//Only used for debugging (check for issues when disabling, another strange issue)
  	Serial.begin(9600);
  	
  	//Set up TimerOne library to call "updateFloppys" at a set interval
  	Timer1.initialize(RESOLUTION); 
  	Timer1.attachInterrupt(updateFloppys);

  	//Set start time to now!
  	_startTime = millis();
  	//Parse the first note
  	//parseNextNoteData();

	//DEBUGGING: set initial periods for floppys  	
  	drives[0].setPeriod(100);
  	drives[1].setPeriod(200);
  	drives[2].setPeriod(400);
}

//Main loop
void loop(){
	
	//While notes still need to be processed...
	//while (millis()<=_noteTime){
		
		//Serial.println("inside note loop!");
		//Set the floppy period from the pasred data
	
		//Parse the next note's data from array
		//parseNextNoteData();
	//}
	
	delay(1000);
}


void updateFloppys() {
	//Serial.println("updating floppys");
	//Loop through all floppy objects in array and call the increment
	//state function

	//Using for loop to generate index does strange things (NEED FIX)	
	drives[0].incrementState();
	drives[1].incrementState();
	drives[2].incrementState();
	
}

/* Commented to allow song data header to be excluded 
    for much faster upload times while debugging
void parseNextNoteData(){
	
	//Parse data from song array
	_notePeriod = song[_noteIndex]>> 24;
	_noteTime = ((song[_noteIndex]>> 2)&0x003FFFFF)+_startTime;
	_noteTrack = song[_noteIndex]&3;
	
	//Increment index
	_noteIndex++;
}
*/

