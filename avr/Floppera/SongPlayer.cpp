
/*
songData.cpp
Craig Ramsay

Class for controlling the playing of the song (used with Floppy.h)
*/

#include "Arduino.h"
#include "SongPlayer.h"
#include "Floppy.h"
#include "songData.h"
#include <TimerOne.h>
#include <pgmspace.h>

//Constant for interval of "ticks" from TimerOne lib
#define RESOLUTION 40

//This will break when there is more than one instance (class level the noo)
Floppy _drives[]={Floppy(0,1),Floppy(2,3),Floppy(4,5)};


SongPlayer::SongPlayer(){
	
	//Set up TimerOne library to make function calls at a set interval
  	Timer1.initialize(RESOLUTION); 

	//Instantiate object varialbes 
	_startTime=0;
	_playSong=false;

}

int SongPlayer::updateSong(){
	
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
		
		//Return 1 to indicate a song in progress
		return 1;	
	}
	
	//Else if this the song has just ended/stopped prematurely (playSong is still true)
	else if (_playSong){
			
			//Stop the song from playing
			songStop();
		}

	//Else (the song does not need played) ...
	else {
	
		//Reset the noteIndex to zero (clears up local variable that the stop/toggle
		//function can't reach)
		noteIndex=0;
	}

	//Return 0 to indicate no song playing
	return 0;
}


void SongPlayer::updateFloppys() {

	//Loop through all floppy objects in array and call the increment
	//state function
	for (int i=0;i<(sizeof(_drives)/sizeof(Floppy));i++){
		_drives[i].incrementState();
	}
	
}


void SongPlayer::toggleSongState(){
	
	//If the song is not currently being played
	if (!_playSong)	{
		songStart();
	}

	//else (the song is currently being played)
	else {
		//Stop the song from being played
		songStop();
	}
	
}

void SongPlayer::songStart(){
	
	//Set start time for the song
	_startTime=millis();
		
	//Attach the timerOne timer to the update floppys function
	Timer1.attachInterrupt(SongPlayer::updateFloppys,RESOLUTION);

	//Set the playSong flag to true to start the song
	_playSong=true;
}

void SongPlayer::songStop(){

	//Set playSong to false
	_playSong=false;
		
	//Set all drives to a period of zero
	for (int i=0;i<(sizeof(_drives)/sizeof(Floppy));i++){
		_drives[i].setPeriod(0);
	}
			
	updateFloppys();
			
	//Also, detach the timerOne lib from the update floppy 
	//function to save power
	Timer1.detachInterrupt();
}

void SongPlayer::parseNextNote(unsigned char &noteTrack, unsigned int &notePeriod, unsigned long &noteTime, unsigned int &noteIndex){
	
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

void SongPlayer::setFloppyNote(int index, unsigned int period){

	//Call setter function for period of floppy object
	_drives[index].setPeriod(period); 	
}



