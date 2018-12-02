#include <Rotary.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
#include <stdarg.h>

const bool DEBUG = true;


struct Knob {
  int control_number;
  int channel;
  int button_pin;
  bool button_is_pressed;  
  Rotary rotary;
  int rotary_counter;
  Knob(int rotary_pin_a, int rotary_pin_b, int button_pin, int control_number, int channel) : 
    button_pin(button_pin),
    button_is_pressed(false),
    rotary(Rotary(rotary_pin_a, rotary_pin_b)),
    rotary_counter(0),
    control_number(control_number),
    channel(channel) {}
};

Knob knobs[] = {
  {9, 10, 11, 1, 1},
  {1, 2, 3, 2, 1},
};

int count_knobs = 2;

//int startTime;
//int endTime;

MIDI_CREATE_INSTANCE(HardwareSerial,Serial, midiOut); // create a MIDI object called midiOut


void setup() {
  Serial.begin(9600);
  for(int i = 0; i < count_knobs; i++) {
    Knob* knob = &knobs[i];
    
    pinMode(knob->button_pin, INPUT_PULLUP);
  }
  
}

void loop() {
  //startTime = millis();

  int i;
  for(i = 0; i < count_knobs; i++) {
    // Rotation
    Knob* knob = &knobs[i];
    
    knobProcessRotary(knob);
    knobProcessButton(knob);
  }
  //endTime = millis();
  //Serial.println(endTime - startTime);

}

void knobProcessRotary(Knob* knob) {
  int result = knob->rotary.process();

  if (result == DIR_NONE) return;

  if (result == DIR_CW) {
    knob->rotary_counter++;
  } else {
    knob->rotary_counter--;
  }
  
  if(DEBUG == false) {
    // send a MIDI CC -- 56 = note, 127 = velocity, 1 = channel
    midiOut.sendControlChange(
      knob->control_number,
      knob->rotary_counter,
      knob->channel
    );
  } else {
    p("Rotation: %i:%i %i", knob->control_number, knob->channel, knob->rotary_counter);  
  }
}

void knobProcessButton(Knob* knob) {
  int button = digitalRead(knob->button_pin);
  
  if(button == LOW && knob->button_is_pressed == false) {
    knob->button_is_pressed = true;
  } else if(button == HIGH && knob->button_is_pressed == true) {
    knob->button_is_pressed = false;
  } else {
    return;
  }

  if(DEBUG == false) {
    // TODO: Implement midi 
  } else {
    p("Button: %i:%i %s", knob->control_number, knob->channel, knob->button_is_pressed ? "Pressed" : "Released");
  }
}


void p(char *fmt, ... ){
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.println(buf);
}
