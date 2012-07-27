package floppera;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Vector;

import javax.sound.midi.InvalidMidiDataException;
import javax.sound.midi.MidiSystem;
import javax.sound.midi.Sequence;
import javax.sound.midi.Track;

public class Main {

	String _midiName="mm2metal.mid";
	//String _filePath="sounds/"+_midiName;
	String _filePath="sounds/"+_midiName;
	int _pitchOffset=7;
	double _tempoSkew=0.4;
	
	OutputStream output;
	
	public Main(){
			
			//Setup for midi parse
			Sequence midiSe = null;
			try {
				midiSe = MidiSystem.getSequence(new File(this.getClass().getResource(_filePath).getFile()));
				//midiSe = MidiSystem.getSequence(new File(_filePath));
			} catch (InvalidMidiDataException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			Track tk;
			Vector<NoteEvent> songVect = new Vector<NoteEvent>();
			
			int noTracks = midiSe.getTracks().length;
			if (noTracks>4)
				noTracks=4;
			
			//Parse all note change events on all midi tracks
			for (int track=0; track<noTracks;track++){
				tk = midiSe.getTracks()[track];
				
				for (int t=1; t<tk.size();t++){
					
					/*Note: keypress event is 1001nnnn where nnnn is meant to be the track number but some midi files
					 use this rule differently (use range for wider compatibility for <=8 tracks) */
					if (tk.get(t).getMessage().getStatus()>=144&&tk.get(t).getMessage().getStatus()<=160){
						songVect.add(new NoteEvent(
								track, 
								getTfromMidiNo(tk.get(t).getMessage().getMessage()[1]), 
								tk.get(t).getTick()
						));
					}
					else if (tk.get(t).getMessage().getStatus()>=128&&tk.get(t).getMessage().getStatus()<=144){
						songVect.add(new NoteEvent(
								track, 
								0, 
								tk.get(t).getTick()
						));
					}
				}
			}
			
			
			//Sort events by tick time
			boolean swapped=false;
			NoteEvent temp;
			NoteEvent[] parsedSong = new NoteEvent[songVect.size()];
			for (int i=0;i<parsedSong.length;i++){
				parsedSong[i]=songVect.elementAt(i);
			}
			
			do{
				swapped=false;
				for (int i=0; i<parsedSong.length-1;i++){
					if(parsedSong[i].getTick()>parsedSong[i+1].getTick()){
						temp=parsedSong[i];
						parsedSong[i]=parsedSong[i+1];
						parsedSong[i+1]=temp;
						swapped=true;
					}
				}
			}while(swapped==true);
			
			
			/*Print out a heade structure similar to : 
			 
			  #include "pgmspace.h"
			  #define _songDataLength 1397
              //PROGMEM modifier and special "unsigned long" data type for storage in flash memory on avr chip
              prog_uint32_t _songData[] PROGMEM= {....*/
			  
			System.out.println("#include \"pgmspace.h\"");
			System.out.print("#define _songDataLength ");
			System.out.println(parsedSong.length);
			System.out.println("\n//PROGMEM modifier and special \"unsigned long\" data type for storage in flash memory on avr chip");
			System.out.println("prog_uint32_t _songData[] PROGMEM = {");
			//Output the events in parsedSong
			for (NoteEvent note: parsedSong){
				//System.out.println("Tick: " + note.getTick()*_tempoSkew);
				//System.out.println("Track: " + note.getTrackNo());
				//System.out.println("Period:  " + note.getPeriod());
				//System.out.println("-");
				long data = 0;
				data = (byte)note.getPeriod()<<24;
				long tick = ((long)(note.getTick()*_tempoSkew)&0x003FFFFF)<<2;
				data |= tick;
				data |= ((byte)note.getTrackNo()-1);
				
				System.out.println(data+",");
			}
		
			System.out.println("};");
			//System.out.println(parsedSong.length);
		System.exit(0);
	 }
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		new Main();
	}

	private int getTfromMidiNo(int midiNo){
		return getT((getF(midiNo)));
	}
	
	private double getF(double midiNo){
		return (Math.pow(2, (midiNo-(69-_pitchOffset))/12)*440);
	}
	
	private int getT(double freq){
		double t = 1/freq/40*1000000;
		return (int)t;
	}
}
