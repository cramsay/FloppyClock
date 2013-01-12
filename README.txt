This project is split in to two parts:

	1) The Arduino-side code to drive the floppy drive motors, handle 
user interaction and the alarm clock functionality

	2) A java desktop app to parse a midi file in to a suitable format 
for the Arduino to handle with minimal overhead

These are divided into the "avr" and "midi_parser" dirs respectively.


Arduino-side Notes:
	
**Harware info in here and desc of each class/module**	



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
