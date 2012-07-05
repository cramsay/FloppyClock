//Includes
#include  <TimerOne.h>
#include <Floppy.h>

//Constant for interval of "ticks"
#define RESOLUTION 40 

//Declare floppy array 
Floppy drives[3]={Floppy(2,3),Floppy(4,5),Floppy(6,7)};

//Function called upon boot
void setup(){

  	//Set up timer library to call "updateFloppys" at a set interval
  	Timer1.initialize(RESOLUTION); 
  	Timer1.attachInterrupt(updateFloppys);

  	//Setup the serial communications for debuging
  	Serial.begin(115200);
}

//Main loop
void loop(){
	//TODO must add in time sensitive update of floppy periods from output of java
	//midi parser
}

/*
Called by the timer inturrupt at the specified resolution.
*/
void updateFloppys() {

	//Loop through all floppy objects in array and call the increment
	//state function
	for (int i=0;i<sizeof(drives);i++){
		drives[i].incrementState();
	}
}




