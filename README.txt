An Arduino based alarm clock system which plays parsed midi files on a stack of floppy drives - original floppy drive code base derived from SammyIAm's "Moppy" project but gets completely reimplemented in an OOP style

This project is split in to two parts:

	1) The Arduino-side code to drive the floppy drive motors, handle 
user interaction and the alarm clock functionality

	2) A java desktop app to parse a midi file in to a suitable format 
for the Arduino to handle with minimal overhead

These are divided into the "avr" and "midi_parser" dirs respectively.


Arduino-side Notes:
	
	The hardware included in this project consists of ...
		Arduino Uno - to control everything else...
		LCD - To display the time and give feedback when setting the time and alarm
		Potentiometer - To let the user quickly set the time and alarm
		Analogue stick - Using basic up/down/left/right flicks, the user can run different commands (hardware scavenged from a gamecube controller). This could be replaced by 2 pots or possibly 4 buttons with some modification to the UI class
		3 Floppy disk drives - To play a parsed midi file using the clicks of the stepper motor inside them

	Pin Layout:

	For the LCD, the pins "rs, enable, d4, d5, d6, d7" align with the digital pins 7,6,5,4,3,2 on the Arduino.
	The output of the pot for adujsting the time is connected to A2.
	The X axis pot from the analogue stick is connected to pin A1 and the y axis to A0
	
	Each connected floppy drive (I have 3) needs 2 pins to be connected: one to step the motor and another to dictate the direction.
	The first drive's motor is connected to pin 8, the direction pin to 9 - and so on for the next 2 floppy drives up to pin 13.
	Also, ensure that you ground the drive select pin on each floppy drive, otherwise they will not respond to the input.
	The ground between the the arduino and floppy drives must be common (it's sometimes easier to power the floppy drives from a spare PC power supply). 


Midi Parsing Notes:

	The main code is held in the main class (yes, really), and requires 
only one supporting class to operate - the Note Event class. This is just 
to provide a convient way of holding all the data associated with a single 
note action.

	Some experimental midi files are provided in the sounds tarball in 
the source dir as well as a single, well tested one residing in the sounds 
folder.

	The main java class require a single parameter - the location of 
the midi file. For Example: java "floppera/Main /tmp/mm2metal.mid". This 
spits out a bunch of text which should then be manually copied to the 
SongData.h header in the AVR side of the code, ready for uploading to the 
Arduino.
