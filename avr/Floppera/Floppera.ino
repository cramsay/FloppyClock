//Includes
#include  <TimerOne.h>
#include <Floppy.h>
#include <songData.h>

//Constant for interval of "ticks"
#define RESOLUTION 40 

//Declare floppy array 
Floppy drives[3]={Floppy(2,3),Floppy(4,5),Floppy(6,7)};
Floppy test(2,3);
//Set up some variables to track the next note properties
unsigned long _startTime;
unsigned int _noteIndex=0;
byte _noteTrack=0;
byte _notePeriod=0;
unsigned long _noteTime=0;

//Function called upon boot
void setup(){
	
  	//Setup the serial communications for debuging
  	Serial.begin(9600);
  	
  	//Set up timer library to call "updateFloppys" at a set interval
  	Timer1.initialize(RESOLUTION); 
  	Timer1.attachInterrupt(updateFloppys);

  	
  	//Set start time to now!
  	_startTime = millis();
  	//Parse the first note
  	parseNextNoteData();
}

//Main loop
void loop(){
	
	//While notes still need to be processed...
	//while (millis()<=_noteTime){
		
		//Serial.println("inside note loop!");
		//Set the floppy period from the pasred data
		test.setPeriod(80);
	
		//Parse the next note's data from array
		//parseNextNoteData();
	//}
	
	delay(500);
}


void updateFloppys() {
	//Serial.println("updating floppys");
	//Loop through all floppy objects in array and call the increment
	//state function
	//for (int i=0;i<sizeof(drives);i++){
		test.incrementState();
	//}
}

void parseNextNoteData(){
	
	//Parse data from song array
	_notePeriod = song[_noteIndex]>> 24;
	_noteTime = ((song[_noteIndex]>> 2)&0x003FFFFF)+_startTime;
	_noteTrack = song[_noteIndex]&3;
	
	//Increment index
	_noteIndex++;
}


