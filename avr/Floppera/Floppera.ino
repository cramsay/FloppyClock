#include "SongPlayer.h"
#include "UI.h"
#include "TimeStruct.h"
#include <TimerOne.h> //These 2 includes are not needed in this file - BUT the arduino ide wont compile 
#include <pgmspace.h> //if they are only included in a .cpp file so they are in the here as a workaround.
#include <LiquidCrystal.h>

//Declare globals
SongPlayer _songPlayer;
struct TimeStruct timeMain;
struct TimeStruct timeAlarm;

//Setup function called upon boot
void setup(){
	//Begin the serial connection for debugging
	Serial.begin(9600);

	//Initialise time structs
	timeMain.lastMinute=millis();
	timeAlarm.hour=7;
	timeAlarm.minute=30;
	
	//Initialise the LCD screen
	initialiseLCD();
	updateStatusLCD(&timeMain);

	//Set control pins to inputs
	pinMode(A0,INPUT);
	pinMode(A1,INPUT);
	pinMode(A2,INPUT);
}

//Main loop
void loop(){

	//Update the floppy drives and song data
	_songPlayer.updateSong();
	
	//Update the time structure
	updateTime(&timeMain, &timeAlarm);
	
	//Get user input and perform corresponding functions
	int control = getUserInput();
	
	switch (control){
		case 0:
			getNewTime(0,&timeAlarm);
			updateStatusLCD(&timeMain);
			break;
		case 1: 
			getNewTime(1,&timeMain);
			updateStatusLCD(&timeMain);
			break;
		case 2:
			_songPlayer.songStart();
			break;
		case 3:
			_songPlayer.songStop();
			break;
		default:
			break; 
	}

	//Small delay
	delay(1);
}

void updateTime(struct TimeStruct *timeMain, struct TimeStruct *timeAlarm){

	//If 60 seconds have passed since last update then
	if (millis()>=(timeMain->lastMinute+60000)){
		//Increment the lastMinute var
		timeMain->lastMinute+=60000; //NOTE: not millis()+60000 to avoid compound errors accumulating
		//Increment the minute var
		timeMain->minute++;
		
		//Roll over hour if required
		if (timeMain->minute>=60){
			//Reset minuteCounter
			timeMain->minute=0;
			//Increment hour
			timeMain->hour++;

			//Roll over day if required
			if (timeMain->hour>=24){
				//Reset hour to 0
				timeMain->hour=0;
			}
		}
		
		//Update the time on the screen
		updateStatusLCD(timeMain);

		//Check if it is time for the alarm
		if (timeMain->minute==timeAlarm->minute&&timeMain->hour==timeAlarm->hour){
			//Play the song
			_songPlayer.songStart();	
		}

	}

	//Check for millis() rollover
	if (timeMain->lastMinute>millis()){
		//Set lastminute to current time - 1 minute
		timeMain->lastMinute=millis()-60000;
		
		//If that has kept lastMinute to before rollover just set it to 0
		if (timeMain->lastMinute>millis()){
			//(slight loss in time accuracy here)
			timeMain-> lastMinute=0;	
		}
	}
}

void toggleSong(){
	_songPlayer.toggleSongState();
}

