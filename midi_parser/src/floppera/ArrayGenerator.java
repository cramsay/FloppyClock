package floppera;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Vector;

import javax.sound.midi.MidiSystem;
import javax.sound.midi.Sequence;
import javax.sound.midi.Track;

public class ArrayGenerator {

	File arr = new File("/tmp/arr.txt");
	PrintWriter output;
	String _midiName="bwv539.mid";
	String _filePath="sounds/"+_midiName;
	int _trackOffset=-6;
	int _pitchOffset=0;
	double _tempoSkew=01.2;

	public ArrayGenerator(){

		try{
			arr.createNewFile();
			output = new PrintWriter(new FileWriter(arr),true);
			output.print(new String(_midiName));
			
			//Setup for midi parse
			Sequence midiSe = MidiSystem.getSequence(new File(this.getClass().getResource(_filePath).getFile()));
			Track tk;
			Vector<NoteEvent> songVect = new Vector<NoteEvent>();
			
			//Parse all note change events on all midi tracks
			for (int track=0; track<midiSe.getTracks().length;track++){
				tk = midiSe.getTracks()[track];
				
				for (int t=1; t<tk.size();t++){
					System.out.println(track +": "+ tk.get(t).getMessage().getStatus());
					
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
			
			//Output the events to arduino in real time  and output arrays to file
			
			long startT = System.currentTimeMillis();
			long tempT = startT;
			long sleepT = 0;
			output.print("{");
			for (NoteEvent note: parsedSong){
				sleepT=(long)(startT+(note.getTick()*_tempoSkew));
				
				printEvent((note.getTrackNo()-_trackOffset)*2, note.getPeriod(),sleepT);
			}
			
			//output.write(new byte[]{(byte)100});
			//output.write(new byte[]{(byte)100});
			//output.write(new byte[]{(byte)100});
			output.print("}");
			output.close();
			tempT=sleepT;
			}
		catch(Exception e){System.out.println("Uh oh...");e.printStackTrace();}
		
		System.exit(0);
	 }
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		new ArrayGenerator();
	}
	
	private void printEvent(int floppy, int period, long tSleep) throws IOException{
		output.print("{");
		output.print(floppy);
		output.print(",");
		output.print(period);
		output.print(",");
		output.print((int)tSleep);
		output.print("},");
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
