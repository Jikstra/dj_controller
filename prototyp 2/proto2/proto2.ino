#include <Rotary.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
#include <stdarg.h>

const bool DEBUG = false;
const bool BENCHMARK = false;

int BENCH_START_TIME = 0;
int BENCH_TOTAL_TIME = 0;
int BENCH_MAX_TOTAL = 0;

struct Knob {
  int control_number_value;
  int control_number_mute;
  
  int channel;
  int button_pin;
  int step_size;
  bool button_is_pressed;
  bool button_was_pressed;
  Rotary rotary;
  int rotary_counter;
  Knob(int rotary_pin_a, int button_pin, int rotary_pin_b, int control_number_value, int control_number_mute, int channel, int step_size) : 
    button_pin(button_pin),
    button_is_pressed(false),
    button_was_pressed(false),
    rotary(Rotary(rotary_pin_a, rotary_pin_b)),
    rotary_counter(0),
    control_number_value(control_number_value),
    control_number_mute(control_number_mute),
    channel(channel),
    step_size(step_size) {}
};

Knob knobs[] = {
  { 1,  2,  3,  1, 2, 1, 4 },
  { 4,  5,  6,  3, 4, 1, 4 },
};

const int count_knobs = sizeof(knobs) / sizeof(Knob);
//int count_knobs = 2;

int max_time = 0;

MIDI_CREATE_INSTANCE(HardwareSerial,Serial, midiOut); // create a MIDI object called midiOut


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

void knobProcessRotary(Knob* knob) {
  int result = knob->rotary.process();

  if (result == DIR_NONE) return;

  if (result == DIR_CCW && knob->rotary_counter < 127) {
    knob->rotary_counter = knob->rotary_counter + knob->step_size;
  } else if(result == DIR_CW && knob->rotary_counter > 0)  {
    knob->rotary_counter = knob->rotary_counter - knob->step_size;
  } else {
    return;
  }

  int value_to_send = _knobGetValueToSend(knob);
  
  if(DEBUG == false) {
    // send a MIDI CC -- 56 = note, 127 = velocity, 1 = channel
    midiOut.sendControlChange(
      knob->control_number_value,
      value_to_send,
      knob->channel
    );
  } else {
    p("Rotation: %i:%i %i", knob->control_number_value, knob->channel, value_to_send);  
  }
}

void knobProcessButton(Knob* knob) {
  int button = digitalRead(knob->button_pin);
  
  /*if(button == LOW && knob->button_is_pressed == false) {
    knob->button_is_pressed = true;
  } else if(button == HIGH && knob->button_is_pressed == true) {
    knob->button_is_pressed = false;
  } else {
    return;
  }*/
  if(button == LOW && knob->button_was_pressed == false) {
    knob->button_is_pressed = !knob->button_is_pressed;
    knob->button_was_pressed = true;
  } else if(button == HIGH && knob->button_was_pressed == true){
    knob->button_was_pressed = false;
    return;
  } else {
    return;
  }
  
  int value_to_send = knob->button_is_pressed ? 0 : 1;

  if(DEBUG == false) {
    midiOut.sendNoteOn(knob->control_number_mute, value_to_send, knob->channel);
  } else {
    p("Button: %i:%i %s %i", knob->control_number_mute, knob->channel, knob->button_is_pressed ? "Pressed" : "Released", value_to_send);
  }
}

int _knobGetValueToSend(Knob* knob) {
  int value_to_send = knob->rotary_counter;
  if(value_to_send > 127) {
    value_to_send = 127;
  } else if(value_to_send < 0) {
    value_to_send = 0;
  }
  return value_to_send;  
}

void p(char *fmt, ... ){
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.println(buf);
}
