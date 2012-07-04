package floppera;

public class NoteEvent {
	int trackNo;
	int period;
	long tick;
	
	public NoteEvent(int trackNo, int period, long tick){
		this.trackNo = trackNo;
		this.period = period;
		this.tick = tick;
	}
	
	public long getTick(){
		return tick;
	}
	
	public int getPeriod(){
		return period;
	}
	
	public int getTrackNo(){
		return trackNo;
	}
	
	public void setTick(long tick){
		this.tick=tick;
	}
	
	public void setPeriod(int period){
		this.period = period;
	}
	
	public void setTrackNo(int trackNo){
		this.trackNo = trackNo;
	}
}
