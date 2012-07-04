#include <TimerOne.h>
#include <ShiftLCD.h>

boolean firstRun = true; // Used for one-run-only stuffs;

//First pin being used for floppies, and the last pin.  Used for looping over all pins.
const byte FIRST_PIN = 14;
const byte PIN_MAX = 18;
#define RESOLUTION 40 //Microsecond resolution for notes

byte MAX_POSITION[] = {
  158,0,158,0,158,0,158,0,158,0,158,0,158,0,158,0,158,0,158,0,158,0,158,0};
  
//Array to track the current position of each floppy head.  (Only even indexes (i.e. 2,4,6...) are used)
byte currentPosition[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/*Array to keep track of state of each pin.  Even indexes track the control-pins for toggle purposes.  Odd indexes
track direction-pins.  LOW = forward, HIGH=reverse
*/
int currentState[] = {
  0,0,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW
};
  
//Current period assigned to each pin.  0 = off.  Each period is of the length specified by the RESOLUTION
//variable above.  i.e. A period of 10 is (RESOLUTION x 10) microseconds long.
unsigned int currentPeriod[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

//Current tick
unsigned int currentTick[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

ShiftLCD lcd(6, 8, 7);

int curs=0;
boolean firstSerial=true;
//Setup pins (Even-odd pairs for step control and direction
void setup(){
  lcd.begin(8,2);
  
  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(18,OUTPUT);
  pinMode(19,OUTPUT);
  
  Timer1.initialize(RESOLUTION); // Set up a timer at the defined resolution
  Timer1.attachInterrupt(tick); // Attach the tick function

  Serial.begin(115200);
 
  curs=0;
  lcd.clear();
  lcd.setCursor(0,0);
  char* message="Waiting for midi";
  for (int i=0;i<17;i++)
  lcdPrint(message[i]);
  
  curs=0; 
 
}


void loop(){
  
  //The first loop, reset all the drives, and wait 2 seconds...
  if (firstRun)
  {
    firstRun = false;
    resetAll();
    delay(2000);
  }

  //Only read if we have 
  if (Serial.available() > 2){
    if (firstSerial){
      firstSerial=false;
      curs=0;
      lcd.clear();
      lcd.setCursor(0,0);
      char letter;
      for(int i=0;i<16;i++){
        letter = Serial.read(); 
        if (letter=='.')
          break;  
        lcdPrint(letter);
      };
      Serial.read();
      Serial.read();
      Serial.read();
      Serial.read();
      curs=0; 
    }
    
    //Watch for special 100-message to reset the drives
    if (Serial.peek() == 100) {
      curs=0;
      lcd.clear();
      lcd.setCursor(0,0);
      char* message="Waiting for midi";
      for (int i=0;i<17;i++)
        lcdPrint(message[i]);
  
      curs=0; 
      resetAll();
      
      while(Serial.available()>0)
        Serial.read();
        
      firstSerial=true;
      delay(1000);
    } 
    else{
         currentPeriod[Serial.read()] = (Serial.read() << 8) + Serial.read();
    }
  }
}

  void lcdPrint(byte serChar){
     curs++;
     
     if (curs>8)
       lcd.setCursor(curs-9,1);  
     else
        lcd.setCursor(curs-1,0);  
    
      lcd.write(serChar);
  }

/*
Called by the timer inturrupt at the specified resolution.
*/
void tick()
{
  /* 
  If there is a period set for control pin 2, count the number of
  ticks that pass, and toggle the pin if the current period is reached.
  */
  if (currentPeriod[14]>0){
    currentTick[14]++;
    if (currentTick[14] >= currentPeriod[14]){
      togglePin(14,15);
      currentTick[14]=0;
    }
  }
  if (currentPeriod[16]>0){
    currentTick[16]++;
    if (currentTick[16] >= currentPeriod[16]){
      togglePin(16,17);
      currentTick[16]=0;
    }
  }
  if (currentPeriod[18]>0){
    currentTick[18]++;
    if (currentTick[18] >= currentPeriod[18]){
      togglePin(18,19);
      currentTick[18]=0;
    }
  }
  
}

void togglePin(byte pin, byte direction_pin) {
  
  //Switch directions if end has been reached
  if (currentPosition[pin] >= MAX_POSITION[pin]) {
    currentState[direction_pin] = HIGH;
    digitalWrite(direction_pin,HIGH);
  } 
  else if (currentPosition[pin] <= 0) {
    currentState[direction_pin] = LOW;
    digitalWrite(direction_pin,LOW);
  }
  
    //Update currentPosition
  if (currentState[direction_pin] == HIGH){
    currentPosition[pin]--;
  } 
  else {
    currentPosition[pin]++;
  }
  
  //Pulse the control pin
  digitalWrite(pin,currentState[pin]);
  currentState[pin] = ~currentState[pin];
}


//
//// UTILITY FUNCTIONS
//

//For a given controller pin, runs the read-head all the way back to 0
void reset(byte pin)
{
  digitalWrite(pin+1,HIGH); // Go in reverse
  for (byte s=0;s<MAX_POSITION[pin];s+=2){ //Half max because we're stepping directly (no toggle)
    digitalWrite(pin,HIGH);
    digitalWrite(pin,LOW);
    delay(5);
  }
  currentPosition[pin] = 0; // We're reset.
  digitalWrite(pin+1,LOW);
  currentPosition[pin+1] = 0; // Ready to go forward.
}

//Resets all the pins
void resetAll(){
  
  // Old one-at-a-time reset
  //for (byte p=FIRST_PIN;p<=PIN_MAX;p+=2){
  //  reset(p);
  //}
  
  // New all-at-once reset
  for (byte s=0;s<80;s++){ // For max drive's position
    for (byte p=FIRST_PIN;p<=PIN_MAX;p+=2){
      digitalWrite(p+1,HIGH); // Go in reverse
      digitalWrite(p,HIGH);
      digitalWrite(p,LOW);
    }
    delay(5);
  }
  
  for (byte p=FIRST_PIN;p<=PIN_MAX;p+=2){
    currentPosition[p] = 0; // We're reset.
    digitalWrite(p+1,LOW);
    currentState[p+1] = 0; // Ready to go forward.
  }

}



