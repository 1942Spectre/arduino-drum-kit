#include <MIDI.h>



int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal(rs, en, d4, d5, d6, d7)
/*
  35 ACOUSTIC BASS DRUM
  36 BASS DRUM 1
  37 SIDE KICK
  38 ACOUSTIC SNARE
  39 HAND CLAP
  40 ELECTRIC SNARE
  41 LOW FLOOR TOM
  42 CLOSED HI HAT
  43 HIGH FLOOR TOM
  44 PEDAL HI HAT
  45 LOW TOM
  46 OPEN HI HAT
  47 LOW MID TOM
  48 HI MID TOM
  49 CRASH CYMBAL 1
  50 HIGH TOM
  51 RIDE CYMBAL 1
  52 CHINEESE CYMBAL
  53 RIDE BELL
  54 TAMBOURINE
  55 SPLASH CYMBAL

*/


int hi_hat_note = 46;
int tom_note = 45;
int tom2_note = 47;
int snare_note = 38;
int snarebuff = 0;
int tom_buff = 00;
int hi_hat_buff = 0;
int crash_note = 49;

int threshold = 0;

int UPDOWN = 0;
int NEXTBACK = 0;

void setup() {
  MIDI.begin();
  Serial.begin(38400); /* You need to use the same baudrate in the HAIRLESS MIDI  */
}

void loop() {

  /////////////////////////////////// CIRCUIT ///////////////////////////////////////
  /*
    int snare = map(analogRead(A0),0,500,0,128); //snare head
    int tom  = map(analogRead(A1),0,500,0,128); //snare rim
    int hi_hat = map(analogRead(A3),0,500,0,128) ; //hi-hat
    int tom2 = map(analogRead(A2),0,500,0,128);
    int crash = map(analogRead(A4),0,500,0,128);
  */
  int snare = analogRead(A0);
  int tom = analogRead(A1);
  int crash = analogRead(A4);
  int hi_hat = analogRead(A3);
  int tom2 = analogRead(A2);

  if (snare > threshold) {
    MIDI.sendNoteOn(snare_note, snare, 5);
  }
  if (tom > threshold) {
    MIDI.sendNoteOn(tom_note, tom, 1);
  }
  if (crash > threshold) {
    MIDI.sendNoteOn(crash_note, crash, 2);
  }
  if (hi_hat > threshold) {
    MIDI.sendNoteOn(hi_hat_note, hi_hat, 3);
  }
  if (tom2 > threshold) {
    MIDI.sendNoteOn(tom2_note, tom2, 4);
  };
}
