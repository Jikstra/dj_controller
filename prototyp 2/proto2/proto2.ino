#include <Rotary.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>

const bool DEBUG = true;


Rotary rotaries[] = {
  Rotary(1, 2),
  Rotary(3, 4),
  Rotary(5, 6),
};

const int count_rotaries = 3;

int counters[count_rotaries] = {0};

struct Button {
 int pin; 
 bool is_pressed;
 Button(int pin) : pin(pin), is_pressed(false) {}
};


Button buttons[] = {
  { 12 }
};



const int count_buttons = 1;

//int startTime;
//int endTime;

MIDI_CREATE_INSTANCE(HardwareSerial,Serial, midiOut); // create a MIDI object called midiOut


void setup() {
  Serial.begin(9600);
  pinMode(12, INPUT_PULLUP);
}

void loop() {
  //startTime = millis();


  int i;
  for(i = 0; i < count_rotaries; i++) {
    int result = rotaries[i].process();
    
    if (result == DIR_NONE) {
      continue;
    }
    
    if (result == DIR_CW) {
      counters[i]++;
    } else {
      counters[i]--;
    }
    
    rotaryTurned(i);
  }

  for(i = 0; i < count_buttons; i++) {
    int button = digitalRead(buttons[i].pin);
    if(button == LOW && buttons[i].is_pressed == false) {
      Serial.println("Pressed!");
      buttons[i].is_pressed = true;
    } else if(button == HIGH && buttons[i].is_pressed == true) {
      Serial.println("Released!");
      buttons[i].is_pressed = false;
    }
  }
  //endTime = millis();
  //Serial.println(endTime - startTime);

}

void buttonChanged(int i) {
  
}

void rotaryTurned(int i) {
  if(DEBUG == false) {
    midiOut.sendControlChange(i,counters[i],1); // send a MIDI CC -- 56 = note, 127 = velocity, 1 = channel
  } else {
    Serial.println(String(i) + " " + counters[i]);  
  }
}
