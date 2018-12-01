#include <Rotary.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>


Rotary rotaries[] = {
  Rotary(1, 2),
  Rotary(3, 4),
  Rotary(5, 6),
};

const int count_rotaries = 3;

int counters[count_rotaries] = {0};

//int startTime;
//int endTime;

MIDI_CREATE_INSTANCE(HardwareSerial,Serial, midiOut); // create a MIDI object called midiOut


void setup() {
  Serial.begin(9600);
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
    
    log(i);
  }
  //endTime = millis();
  //Serial.println(endTime - startTime);

}

void log(int i) {
  midiOut.sendControlChange(i,counters[i],1); // send a MIDI CC -- 56 = note, 127 = velocity, 1 = channel
}
