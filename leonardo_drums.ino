// I'M USING A ARDUINO LEONARDO INSTEAD OF UNO BECAUSE LEONARDO HAS A BUILT IN USB INTERFACE THAT SUPPORTS MIDI
// MEANING IT WON'T NEED A THIRD PARTY SOFTWARE TO MAKE DEVICES RECOGNIZE ITS SIGNALS AS MIDI SIGNALS.

// TESTED WITH WINDOWS PC, LINUX (FEDORA 34) AND APPLE IOS(IPHONE,IPAD ETC WITH GARAGEBAND)



// LIBRARY NEEDED FOR TRANSMITTING MIDI SIGNALS
#include <MIDIUSB.h>


// DEFINITION OF PIEZOSENSOR PINS.
#define pad1 A0
#define pad2 A1
#define pad3 A2
#define pad4 A3
#define pad5 A4

// CREATING AN ARRAY FOR PIEZOSENSORS AND THE NOTES SET TO THEM.
byte pads[] = {pad1,pad2,pad3,pad4,pad5};
int sounds[] = {40,45,41,44,49};


// THRESHOLD FOR ACTIVATING THE DRUM SIGNALS. I'M USING THIS BECAUSE I DON'T WANT USERS TO BE ABLE TO
// PRODUCE OUTPUT SIGNALS WITH TOO LOW PRESSURE. WHEN THE THRESHOLD IS LESS THAN 10, IF USER
// JUST PUTS HIS/HER HAND ON A DRUM PAD, IT CONTINIOUSLY PRODUCE SIGNAL WHICH IS NOT WANTED
int threshold = 10;


void setup() {

  
  // ENABLING SERIAL (MIDI) AND LCD)
  Serial.begin(115200);
}

// THE LOOP FUNCTION READS THE VALUES FROM DRUMS AND SENDS THEM THROUGHT MIDI
void loop() {

  // READINGS
  int red_vel = analogRead(A0);
  int blue_vel = analogRead(A1);
  int green_vel = analogRead(A2);
  int yellow_vel = analogRead(A3);
  int orange_vel = analogRead(A4);

  // STATEMENTS BELOW SENDS THE SIGNALS OF DRUMS WITH BIGGER VALUE THAN THRESHOLD
  if(red_vel > threshold)
  {
  noteOn(0,sounds[0],red_vel);
  }

  if(blue_vel > threshold)
  {
  noteOn(1,sounds[1],blue_vel);
  }

  if(green_vel > threshold){
  noteOn(2,sounds[2],green_vel);
  }

  if(yellow_vel > threshold){
  noteOn(3,sounds[3],yellow_vel);
  }

  if(orange_vel > threshold){
  noteOn(4,sounds[4],orange_vel);
  }  
}


// THIS FUNCTION IS USED FOR SENDING NOTES.
// PITCH STANDS FOR NOTE , CHANNEL STANDS FOR CHANNEL AND VELOCITY IS THE POWER OF THAT NOTE.
void noteOn(byte channel, byte pitch, byte velocity) {
  // THIS FUNCTION IS NOT CREATED BY ME. I COPIED IT FROM THE REFERENCE DOCUMENTATION OF
  // THE LIBRARY I'm USING.
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}
