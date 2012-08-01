#include "Arduino.h"
#include "UI.h"
#include "TimeStruct.h"
#include <LiquidCrystal.h>

//Prototype definitions of "private" functions
int getHour(int exitControl);
int getHourData();
int getMinute(int exitControl);
int getMinuteData();
void printLCD(String message);

//Declare file-wide vars

LiquidCrystal _lcd(7,6,5,4,3,2);
static byte custChars[][8]={
	{
	B11000,
	B11110,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	},
	{
	B00000,
	B00000,
	B00000,
	B11000,
	B11110,
	B11111,
	B11111,
	B11111,
	},
	{
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B11000,
	B11110,
	},
	{
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000,
	B00011,
	B01111,
	},
	{
	B00000,
	B00000,
	B00000,
	B00011,
	B01111,
	B11111,
	B11111,
	B11111,
	},
	{
	B00011,
	B01111,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	B11111,
	},
	
};

//Initialises the LCD object for use
void initialiseLCD(){
	_lcd.begin(8,2);
	_lcd.createChar(5,custChars[0]);
	_lcd.createChar(6,custChars[1]);
	_lcd.createChar(7,custChars[2]);
	_lcd.createChar(8,custChars[3]);
	_lcd.createChar(9,custChars[4]);
	_lcd.createChar(10,custChars[5]);
	_lcd.clear();
	_lcd.setCursor(0,0);
}

//Updates the LCD with the current time and alarm time
//NOTE: passed pointer to reduce memory usage
void updateStatusLCD(struct TimeStruct *curTime){

	//Reset the cursor to the start	
	_lcd.setCursor(0,0);

	//Print out some custom characters
	_lcd.write(5);
	_lcd.write(6);
	_lcd.write(7);
	_lcd.print("  ");

	//Get the ascii version of the passed time
	String t_H= String(curTime->hour);
	String t_M= String(curTime->minute);

	//If either the hour or minute count
	//is only 1 char long, append a "0" to the start
	if (t_H.length()==1)
		t_H = "0" + String(curTime->hour);

	if (t_M.length()==1)
		t_M = "0" + String(curTime->minute);
	
	//Print time
	_lcd.print(t_H);
	_lcd.print(":");
	_lcd.setCursor(0,1);
	_lcd.print(t_M);

	//Print out some more custom characters
	_lcd.print("  ");
	_lcd.write(8);
	_lcd.write(9);
	_lcd.write(10);

}

void printLCD(String message){	
  _lcd.setCursor(0,0);
  _lcd.print(message.substring(0,8));
  _lcd.setCursor(0,1);
  _lcd.print(message.substring(8,16));
}

int getUserInput(){

	/*Return  data is as follows:
	down : 0
	up : 1
	left : 2
	right : 3
	middle : 4 */ 

	static int x=0; 
	static int y =0;
	int x2=x;
	int y2=y;

	y=(analogRead(A0)/100)-5;
	x=(analogRead(A1)/100)-5;

	if (!(x==x2&&y==y2)){

		if (abs(y)>abs(x)){
			if (y>0) //use catch for 2 parses in a row(?)
				return 0;
			else
				return 1;
		}
		else if (abs(y)<abs(x)){
			if (x>0)
				return 2;
			else 
				return 3;
		}
		else
			return 4;
	}
	else 
		return 4;
}

void getNewTime(int exitControl, struct TimeStruct *newTime){
	delay(400);
	newTime->hour = getHour(exitControl);
	delay(400);
	_lcd.clear();
	newTime->minute = getMinute(exitControl);
	delay(400);
	_lcd.clear();
}

int getHour(int exitControl){
	
	String message="Set new hour: ";
	String newVal;
	do{		
		newVal=String(getHourData());
		if (newVal.length()==1)
			newVal="0"+String(getHourData());		
		printLCD(message+newVal);
	}while(getUserInput()!=exitControl);
	
	return getHourData();
}

int getHourData(){
	return map(analogRead(A2),0,1023,0,23);
}

int getMinute(int exitControl){
	
	String message="Set new min: ";
	String newVal;
	do{
		newVal=String(getMinuteData());
		if (newVal.length()==1)
			newVal="0"+String(getMinuteData());
		printLCD(message+newVal);
	}while(getUserInput()!=exitControl);
	
	return getMinuteData();
}

int getMinuteData(){
	return map(analogRead(A2),0,1023,0,59);
}


