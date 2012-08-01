/* 
SongPlayer.h
Craig Ramsay

Used to control the playing of the song (using Floppy.h)
to add an extra layer of abstraction between the raw pin
manipulation of Floppy.h and the main code, making it neat 
enough to also hold alarm clock funtionality
*/

#ifndef SONGPLAYER_H
#define SONGPLAYER_H

#include "Arduino.h"
#include "Floppy.h"

class SongPlayer {
	private:
		unsigned long _startTime;
		volatile bool _playSong;
		static void updateFloppys();
		void parseNextNote(unsigned char &noteTrack, unsigned int &notePeriod, unsigned long &noteTime, unsigned int &noteIndex);
		void setFloppyNote(int index, unsigned int period);
	public:
		SongPlayer();
		int updateSong();
		void toggleSongState();
		void songStart();
		void songStop();
};

#endif /* SONGPLAYER_H */
