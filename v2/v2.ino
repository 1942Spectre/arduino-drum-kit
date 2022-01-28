// MIDI DRUM KIT WITH ARDUINO LEONARDO

// I'M USING A ARDUINO LEONARDO INSTEAD OF UNO BECAUSE LEONARDO HAS A BUILT IN USB INTERFACE THAT SUPPORTS MIDI
// MEANING IT WON'T NEED A THIRD PARTY SOFTWARE TO MAKE DEVICES RECOGNIZE ITS SIGNALS AS MIDI SIGNALS.


// BECAUSE  THE SD CARD ADAPTER I BOUGHT RECENTLY IS NOT WORKING, I COULD NOT TEST IT AND I'M NOT SURE IF IT WORKS BUT THE REST IS WORKING
// ALSO, I GOT A PROBLEM WITH SPI LIBRARY AND LCD LIBRARY AND I DON'T KNOW ANYTHING ABOUT SPI SO I CAN'T SOLVE IT. IN THE CONFIGURATION FUNCTION,
// THERE IS A STATEMENT THAT BYPASSES THE PART THAT READS AND WRITES DATA TO SD CARD. IF YOU KNOW HOW TO SOLVE THIS YOU CAN CHANGE THE STATEMENT TO USE IT.


// NORMALLY, SD CARD ADAPTER WAS PLANNED TO BE CONNECTED TO FOLLOWING PINS
// ChipSelect: 10
// MOSI: 11
// MISO: 12
// SCK 13

// I NEVER HAD THE CHANGE TO TRY THIS BECAUSE AS I SAID, I COULD NOT MAKE THE ADAPTER I BOUGHT WORK.
// I'M LEAVING THE CODE HERE BECAUSE IT ALREADY CHECKS IF THERE'S A SD CARD CONNECTION AND WRITES A MESSAGE ON
// THE LCD SCREEN THAT TELLS THE USER SD CONNECTION IS NOT AVAILABLE.

// IF YOU ARE ABLE TO SOLVE THE SPI PROBLEM AND SHARE THE SOLUTION WITH ME, IT WOULD BE GREAT.

// LIBRARIES NEEDED FOR SD CARD TO SAVE USER CONFIGURATIONS
#include <SPI.h>
#include <SD.h>
#include <Arduino_JSON.h>

// LIBRARY NEEDED FOR LCD SCREEN
#include <LiquidCrystal.h>

// LIBRARY NEEDED FOR TRANSMITTING MIDI SIGNALS
#include <MIDIUSB.h>


// DEFINITION OF PIEZOSENSOR PINS.
#define pad1 A0
#define pad2 A1
#define pad3 A2
#define pad4 A3
#define pad5 A4

// CREATING AN ARRAY FOR PIEZOSENSORS AND THE NOTES SET TO THEM.
// THOSE NOTES CAN BE CHANGED FROM THE SIMPLE INTERFACE WITH THE BUTTONS AND LCD SCREEN. I INITIALIZED THEM WITH THOSE BECAUSE I USE THEM.
byte pads[] = {pad1,pad2,pad3,pad4,pad5};
int sounds[] = {40,45,41,44,49};

// PINS OF LCD SCREEN
int rs=9,en=8,d4=7,d5=6,d6=5,d7=4;

// PINS OF BUTTONS
int next=3,back=2,ok=1,Exit=0;

// ChipSelect Pin For SD Card
int cs = 10;

// SETTING THE LCD SCREEN
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);


// THRESHOLD FOR ACTIVATING THE DRUM SIGNALS. I'M USING THIS BECAUSE I DON'T WANT USERS TO BE ABLE TO
// PRODUCE OUTPUT SIGNALS WITH TOO LOW PRESSURE. WHEN THE THRESHOLD IS LESS THAN 10, IF USER
// JUST PUTS HIS/HER HAND ON A DRUM PAD, IT CONTINIOUSLY PRODUCE SIGNAL WHICH IS NOT WANTED
int threshold = 10;

// LCD WRITE FUNCTION, FIRST PARAMETER GETS WRITTEN IN THE FIRST LINE AND SECOND PARAMETER IN THE SECOND LINE
void lcdwrite(String text , String text2 = " "){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(text);
  lcd.setCursor(0,1);
  lcd.print(text2);
}


void setup() {

  
  // ENABLING SERIAL (MIDI) AND LCD)
  Serial.begin(115200);
  lcd.begin(16,2);

  // WELCOMING THE USER
  lcdwrite( "Welcome to" , "MIDI DRUMS" );
  delay(5000);

  // SETTING THE PINMODES OF BUTTONS AS INPUT
  pinMode(next,INPUT);
  pinMode(back,INPUT);
  pinMode(ok,INPUT);
  pinMode(Exit,INPUT);

  pinMode(cs,OUTPUT);

 
  // INVOKING THE CONFIGURATION FUNCTION HERE FOR THE INITIAL CONFIGURATIONS FROM THE USER.
  configuration();
}

// THE LOOP FUNCTION READS THE FALUES FROM DRUMS AND EXIT BUTTON.
// IF THE EXIT BUTTON IS NOT PRESSED, LOOP GOES ON.
// WHEN ITS PRESSED, DEVICE PAUSES AND PROMPTS THE USER TO CONTINUE OR CONFIGURE THE DEVICE.
void loop() {

  // READINGS
  int red_vel = analogRead(A0);
  int blue_vel = analogRead(A1);
  int green_vel = analogRead(A2);
  int yellow_vel = analogRead(A3);
  int orange_vel = analogRead(A4);
  int ExitStatus = digitalRead(Exit);

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



  // CHECKS THE STATUS OF EXIT BUTTON. IF ITS PRESSED, IT PAUSES AND WAITS FOR THE USER INPUT.
  // USER CAN CONTINUE PLAYING WITH BACK BUTTON OR START CONFIGURATION WITH NEXT BUTTON
  if(ExitStatus==1){
    lcdwrite("Back = Continue","Next = Configure");
    while(true){
      if(digitalRead(back)>0){
        lcd.clear();
        break;
      }
      else if(digitalRead(next)>0){
        configuration();
      }
      }
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


// THIS FUNCTION SAVES THE CURRENT CONFIGURATION AS A JSON ARRAY TO THE SD CARD.
void saveConfig(int notes[]){

  //JSON ARRAY FORMAT
  // Creating a JSON file for saving the settings.
  String config = "[";
  for (int i = 0; i < sizeof(notes)/sizeof(notes[0]); i++)
  {
    config = config + notes[i];
    if(i < sizeof(notes)/sizeof(notes[0]) - 1)
      config = config + ",";
  } 
  
  config = config + "]";

  File configFile = SD.open("config.jsn",FILE_WRITE);

  if(configFile){
    configFile.print(config);
    configFile.close();
    return;
  }
  else{
    lcdwrite("Error","File Error");
    delay(3000);
    return;
  }
  
}


// THIS FUNCTION READS THE CONFIGURATION OF NOTES FROM THE JSON ARRAY CREATED IN SD CARD.
bool loadConfig(){
  File configFile = SD.open("config.jsn");
  int noteArray[sizeof(sounds)/sizeof(1)];
  String filetext = "";
  if(configFile){

    // While there are chars to read, it will keep reading and keep adding them into variable
    while(configFile.available()){
      char c = configFile.read();
      filetext = filetext + c;
      }
      configFile.close();

      // Using toCharArray to convert the string to a char array which is the datatype this
      // Structure supports
      char text[1024];
      filetext.toCharArray(text,filetext.length());
      
    JSONVar noteArray = JSON.parse(text);

    // If its failed, tell it to the user and return false.
    if(JSON.typeof(noteArray) == "undefined"){
      lcdwrite("reading failed" , " ");
      return false;
    }
    // If its succesfull, 
    else{
      for(int i = 0 ; i < sizeof(sounds) / sizeof(0) ; i ++)
        sounds[i] = (int) noteArray[i];
    }
    lcdwrite("Success","Config Loaded");
    return true;
  }
  else{
    lcdwrite("Error","File Error");
    delay(3000);
    return;
  }
}


// THIS FUNCTION CREATES A CONFIGURATION SESSION TO CONFIGURE THE NOTES OF THE DRUMS.
// USER CAN ADD ANY VOICE IN THE LIST TO ANY DRUM HE/SHE WANTS.
// FOR THE MAIN CONFIGURATION I PROVIDE AS A DEFAULT IN THE BEGINNING OF THE FILE HAS TWO CYMBALS , ONE SNARE AND
// TWO TOMS. USER CAN SET ANY OF THOSE DRUMS TO ANY INSTRUMENT THEY WANT FROM HERE. I KEPT THE ARRAYS SHORT BECAUSE
// I THINK ITS ENOUGH BUT IF WANTED, HAND CLAPS, METALIC NOISES OR ELSE CAN BE ADDED TO THOSE ARRAYS EASILY.
// https://www.zendrum.com/resource-site/drumnotes.htm

void configuration(){

  // THE ARRAYS CONTAINING THE NOTES AND THE SOUNDS THEY CORRESPOND TO.
  int notes[] = {49,55,51,48,47,45,43,41,40,38,46,44,37,36,42,35,54};
  String instruments[] = {"Crash","Splash","Ride","Open Hi-Hat","Hi-Tom","Mid-Tom","Lo-Tom","Floor-Tom","Snare Rim","Snare Hit","Half Hi-Hat","Pedal Hi-Hat","Snare Side","Kick 1","Closed Hi-Hat","Kick 2","Tambourine"};
  // IF A NEW DRUM IS ADDED, IT CAN BE EASILY ADDED HERE TOO.
  String colors[] = {"red","blue","green","yellow","orange"};

  
  int selected_drum = 0;
  int i = 0;
  int selected_instrument;
  int selected_note;
  lcdwrite("Load config?" , "Back=n/Next = y");
  delay(1000);
  while(true)
  {
    if(digitalRead(next) > 0){
       if(1){// Normally, there should be SD.begin(cs) instead of 1, but because of SPI, it disables the lcd screen when sd library gets activated.
        // So I'm just putting a dummy here. That's all I can do, I can not change the SD library, I don't understand what is SPI or how to manage it. I can't solve this problem.
          lcdwrite("SD card error" , "Not available");
          delay(3000);
          break;
        }
        else{
          loadConfig();
          return;
        }}

    if(digitalRead(back) > 0){
      break;
    }
  }
// MAIN CONFIGURATION LOOP THAT ALLOWS THE USER TO NAVIGATE ACROSS THE MENUS
  while(true)
  {
  // SELECTING A DRUM HERE TO CONFIGURE
  while(true){
    if(digitalRead(next) == 1){
      if(selected_drum < 4)
        selected_drum++;
        delay(100);
    }
    else if(digitalRead(back) == 1){
      if(selected_drum > 0)
        selected_drum = selected_drum - 1;
      delay(100);
     }

    else if(digitalRead(ok) == 1){
      delay(100);
      break;
    }

    else if(digitalRead(Exit) == 1){
      lcd.clear();
      return;
    }
  
    lcdwrite("Select a drum" , colors[selected_drum]);
    delay(100);
  }

  // SELECTING A SOUND HERE 
  while(true){
        if(digitalRead(next) == 1){
        if(i < sizeof(notes)/sizeof(notes[0]))
          i++;
        delay(100);
    }
    else if(digitalRead(back) == 1){
      if(i > 0)
        i = i - 1;
      delay(100);
     }

    else if(digitalRead(ok) == 1){
      delay(100);
      sounds[selected_drum] = notes[i];
      break;
      }
    else if(digitalRead(Exit) == 1){
      lcd.clear();
      return;
    }
    lcdwrite("Select a sound" , instruments[i] );
    delay(100);
  }
  
  
  lcdwrite("Back=Exit" , "Next=Configure");
  while(true){
    if(digitalRead(next) == 1){
       break;
       delay(100);
     }
    else if(digitalRead(back) == 1){
      lcdwrite("Save Config ?","Next=y/Back=n");
      while(true){
        delay(500);
        while(true)
        {
        if(digitalRead(next) > 0){
            if(1){// Normally, there should be SD.begin(cs) instead of 1, but because of SPI, it disables the lcd screen when sd library gets activated.
            // So I'm just putting a dummy here. That's all I can do, I can not change the SD library, I don't understand what is SPI or how to manage it. I couldn't solve this problem.
            lcdwrite("SD card error" , "Not available");
            delay(3000);
            lcdwrite("Back=Exit" , "Next=Configure");
            break;
        }
        else{
          saveConfig(sounds);
          return;
        }}

        if(digitalRead(back) > 0){
          lcd.clear();
            return;
        }
        else if(digitalRead(Exit) == 1){
            lcd.clear();
            return;
        }
      }
    }
  }
}
}
}
