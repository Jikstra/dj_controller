#include <Rotary.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
#include <stdarg.h>

const bool DEBUG = true;
const bool BENCHMARK = false;

int BENCH_START_TIME = 0;
int BENCH_TOTAL_TIME = 0;
int BENCH_MAX_TOTAL = 0;

void p(char *fmt, ... ){
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.println(buf);
}

struct Knob {
  int control_number;
  int channel;
  int button_pin;
  bool button_is_pressed;  
  Rotary rotary;
  int rotary_counter;
  Knob(int rotary_pin_a, int button_pin, int rotary_pin_b, int control_number, int channel) : 
    button_pin(button_pin),
    button_is_pressed(false),
    rotary(Rotary(rotary_pin_a, rotary_pin_b)),
    rotary_counter(0),
    control_number(control_number),
    channel(channel) {}
};

// Rotary PIN A, Button PIN, Rotary PIN B, Control number, Channel
Knob knobs[] = {
   { 1, 2, 3 , 1, 1 },
   { 4, 5, 6 , 2, 1 }
};


const int count_knobs = sizeof(knobs) / sizeof(Knob);
//int count_knobs = 2;

int max_time = 0;

MIDI_CREATE_INSTANCE(HardwareSerial,Serial, midiOut); // create a MIDI object called midiOut



void knobProcessRotary(Knob* knob) {
  int result = knob->rotary.process();

  if (result == DIR_NONE) return;

  if (result == DIR_CCW && knob->rotary_counter < 127) {
    knob->rotary_counter++;
  } else if(result == DIR_CW && knob->rotary_counter > 0)  {
    knob->rotary_counter--;
  } else {
    return;
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
    midiOut.sendNoteOn(knob->control_number, knob->button_is_pressed ? 127 : 0, knob->channel);
  } else {
    p("Button: %i:%i %s", knob->control_number, knob->channel, knob->button_is_pressed ? "Pressed" : "Released");
  }
}



void setup() {
  Serial.begin(115200);
  for(int i = 0; i < count_knobs; i++) {
    Knob* knob = &knobs[i];
    
    pinMode(knob->button_pin, INPUT_PULLUP);
  }
  
}

void loop() {
  if(BENCHMARK == true) {
    BENCH_START_TIME = micros();
  }

  for(int i = 0; i < count_knobs; i++) {
    // Rotation
    Knob* knob = &knobs[i];
    
    knobProcessRotary(knob);
    knobProcessButton(knob);
  }

  if(BENCHMARK == true) {
    BENCH_TOTAL_TIME = micros() - BENCH_START_TIME;
    if(BENCH_TOTAL_TIME > BENCH_MAX_TOTAL) {
      BENCH_MAX_TOTAL = BENCH_TOTAL_TIME;
      p("Benchmark: %i micros", BENCH_MAX_TOTAL);
    }  
  }

  delayMicroseconds(1000);
}
