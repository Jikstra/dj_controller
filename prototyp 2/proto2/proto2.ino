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

const bool DECK_A = 0;
const bool DECK_B = 1;

int channel_deck_a = 1;
int channel_deck_b = 2;

struct Knob {
  int control_number_value;
  int control_number_mute;
  
  bool deck;
  int button_pin;
  int step_size;
  bool button_is_pressed;
  bool button_was_pressed;
  unsigned long button_last_flake;
  Rotary rotary;
  int rotary_counter;
  Knob(int rotary_pin_a, int button_pin, int rotary_pin_b, int control_number_value, int control_number_mute, bool deck, int step_size) : 
    button_pin(button_pin),
    button_is_pressed(false),
    button_was_pressed(false),
    button_last_flake(0),
    rotary(Rotary(rotary_pin_a, rotary_pin_b)),
    rotary_counter(63),
    control_number_value(control_number_value),
    control_number_mute(control_number_mute),
    deck(deck),
    step_size(step_size) {}
};

struct Button {
  int control_number;
  const bool deck;
  int pin;
  unsigned long last_flake;
  bool switch_is_up;
  bool switch_was_up;
  Button(int pin, int control_number, bool deck) :
    pin(pin),
    control_number(control_number),
    deck(deck),
    last_flake(0),
    switch_is_up(true),
    switch_was_up(true) {}
  
};


struct PositionLoopKnob {
  int control_number_left;
  int control_number_right;
  int control_number_press;
  int control_number_release;
  
  bool deck ;
  int button_pin;
  bool button_is_pressed;
  bool button_was_pressed;
  unsigned long button_last_flake;
  Rotary rotary;
  int rotary_counter;
  PositionLoopKnob(int rotary_pin_a, int button_pin, int rotary_pin_b, int control_number_left, int control_number_right, int control_number_press, int control_number_release, bool deck) : 
    button_pin(button_pin),
    button_is_pressed(false),
    button_was_pressed(false),
    button_last_flake(0),
    rotary(Rotary(rotary_pin_a, rotary_pin_b)),
    control_number_left(control_number_left),
    control_number_right(control_number_right),
    control_number_press(control_number_press),
    control_number_release(control_number_release),
    deck(deck) {}
};

/*************
 * START PIN LAYOUT
 *************/

Knob knobs[] = {
  // DECK A
  {   8,   9,  10,  5,  6, DECK_A, 4 },
  {  11,  12,  13,  7,  8, DECK_A, 4 },
  {  A0,  A1,  A2,  9, 10, DECK_A, 4 },
  {  A3,  A4,  A5, 11, 12, DECK_A, 4 },
  {  A8,  A9, A10, 13, 14, DECK_A, 4 },
  { A11, A12, A13, 15, 16, DECK_A, 4 },
  {  14,  15,  16, 17, 18, DECK_A, 4 },
  {  17,  18,  19, 19, 20, DECK_A, 4 },
  
  // DECK B
  {  28,  30,  32,  5,  6, DECK_B, 4 },
  {  29,  31,  33,  7,  8, DECK_B, 4 },
  {  34,  36,  38,  9, 10, DECK_B, 4 },
  {  35,  37,  39, 11, 12, DECK_B, 4 },
  {  40,  42,  44, 13, 14, DECK_B, 4 },
  {  41,  43,  45, 15, 16, DECK_B, 4 },
  {  46,  48,  50, 17, 18, DECK_B, 4 },
  {  47,  49,  51, 19, 20, DECK_B, 4 }

};


const int count_knobs = sizeof(knobs) / sizeof(Knob);
//int count_knobs = 2;



/*************
 * END PIN LAYOUT
 *************/

int max_time = 0;

MIDI_CREATE_INSTANCE(HardwareSerial,Serial, midiOut); // create a MIDI object called midiOut

/*************
 *  Knob
 *************/

void setupKnobs() {
    for(int i = 0; i < count_knobs; i++) {
    _setupKnob(&knobs[i]);
  }
}

void _setupKnob(Knob* knob) {
   pinMode(knob->button_pin, INPUT_PULLUP);
}

void loopKnobs() {  
  for(int i = 0; i < count_knobs; i++) {
    // Rotation
    Knob* knob = &knobs[i];
    
    knobProcessRotary(knob);
    knobProcessButton(knob);
  }
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
  int channel = getChannelFromDeck(knob->deck);
  
  if(DEBUG == false) {
    // send a MIDI CC -- 56 = note, 127 = velocity, 1 = channel
    midiOut.sendControlChange(
      knob->control_number_value,
      value_to_send,
      channel
    );
  } else {
    p("Rotation: %i:%i %i", knob->control_number_value, channel, value_to_send);  
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
    if(isBouncing(&knob->button_last_flake)) return;
    knob->button_is_pressed = !knob->button_is_pressed;
    knob->button_was_pressed = true;
  } else if(button == HIGH && knob->button_was_pressed == true){
    if(isBouncing(&knob->button_last_flake)) return;
    knob->button_was_pressed = false;
    return;
  } else {
    return;
  }

  int channel = getChannelFromDeck(knob->deck);
  int value_to_send = knob->button_is_pressed ? 0 : 1;

  if(DEBUG == false) {
    midiOut.sendNoteOn(knob->control_number_mute, value_to_send, channel);
  } else {
    p("Button: %i:%i %s %i", knob->control_number_mute, channel, knob->button_is_pressed ? "Pressed" : "Released", value_to_send);
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

/*************
 * PositionLoopKnob
 *************/


PositionLoopKnob position_loop_knobs[] = {
  // DECK A
  {   2,   3,   4,  31,  32, 33, 33, DECK_A },
  {   5,   6,   7,  36,  37, 38, 38, DECK_A },

  // DECK B
  {  22,  24,  26,  31,  32, 33, 33, DECK_B },
  {  23,  25,  27,  36,  37, 38, 38, DECK_B },
};

const int count_position_loop_knobs = sizeof(position_loop_knobs) / sizeof(PositionLoopKnob);

void setupPositionLoopKnobs() {
  for(int i = 0; i < count_position_loop_knobs; i++) {
    PositionLoopKnob* currKnob = &position_loop_knobs[i];
    pinMode(currKnob->button_pin, INPUT_PULLUP);
  }
}

void loopPositionLoopKnobs() {
  for(int i = 0; i < count_position_loop_knobs; i++) {
    PositionLoopKnob* currKnob = &position_loop_knobs[i];
    processLoopPositionKnobRotary(currKnob);
    processLoopPositionKnobButton(currKnob);
  }
}

void processLoopPositionKnobRotary(PositionLoopKnob* knob) {
  int result = knob->rotary.process();

  if (result == DIR_NONE) return;

  bool turned_left;
  if (result == DIR_CCW) {
    turned_left = false;
  } else if(result == DIR_CW)  {
    turned_left = true;
  } else {
    return;
  }

  int channel = getChannelFromDeck(knob->deck);
  int control_number = turned_left ? knob->control_number_left : knob->control_number_right;
  
  if(DEBUG == false) {
    // send a MIDI CC -- 56 = note, 127 = velocity, 1 = channel
    midiOut.sendControlChange(
      control_number,
      1,
      channel
    );
  } else {
    p("PositionLoop: %i:%i %i %s", control_number, channel, 1, turned_left ? "Left": "Right");  
  }
}

void processLoopPositionKnobButton(PositionLoopKnob* knob) {
  int button = digitalRead(knob->button_pin);
  
  /*if(button == LOW && knob->button_is_pressed == false) {
    knob->button_is_pressed = true;
  } else if(button == HIGH && knob->button_is_pressed == true) {
    knob->button_is_pressed = false;
  } else {
    return;
  }*/
  
  if(button == LOW && knob->button_was_pressed == false) {
    if(isBouncing(&knob->button_last_flake)) return;
    knob->button_is_pressed = !knob->button_is_pressed;
    knob->button_was_pressed = true;
  } else if(button == HIGH && knob->button_was_pressed == true){
    if(isBouncing(&knob->button_last_flake)) return;
    knob->button_was_pressed = false;
    return;
  } else {
    return;
  }

  int control_number = knob->button_is_pressed ? knob->control_number_press : knob->control_number_release;
  int channel = getChannelFromDeck(knob->deck);
  
  int value_to_send = knob->button_is_pressed ? 0 : 1;

  if(DEBUG == false) {
    midiOut.sendNoteOn(control_number, 1, channel);
  } else {
    p("PositionLoop Button: %i:%i %s", control_number, channel, knob->button_is_pressed ? "Pressed" : "Released");
  }
}


/*************
 * Button
 *************/

void buttonSetup(Button* button) {
   pinMode(button->pin, INPUT_PULLUP);
 }

void buttonProcess(Button* button) {
  int state = digitalRead(button->pin);
  _buttonProcess(button, state);
}

void _buttonProcess(Button* button, int state) {
  if(state == LOW && button->switch_was_up == false) {
    if(isBouncing(&button->last_flake)) return;
    button->switch_is_up = !button->switch_is_up;
    button->switch_was_up = true;
  } else if(state == HIGH && button->switch_was_up == true){
    if(isBouncing(&button->last_flake)) return;
    button->switch_was_up = false;
    return;
  } else {
    return;
  }

  int value_to_send = 1;
  int channel = getChannelFromDeck(button->deck);

  if(DEBUG == false) {
    midiOut.sendNoteOn(button->control_number, value_to_send, channel);
  } else {
    p("Button: %i:%i %s %i", button->control_number, channel, value_to_send ? "Up" : "Down", value_to_send);
  }
}


/************
 * MATRIX
 ************/

int matrix_col_a = A6;
int matrix_col_b = A7;

Button matrix_buttons_col_a[] = {
  { 20, 50, DECK_A },
  { 21, 51, DECK_A },
  { 52, 52, DECK_A },
  { 53, 53, DECK_A },
};

const int count_matrix_buttons_col_a = sizeof(matrix_buttons_col_a) / sizeof(Button);


Button matrix_buttons_col_b[] = {
  { 20, 50, DECK_B },
  { 21, 51, DECK_B },
  { 52,  52, DECK_B },
  { 53,  53, DECK_B }
};

const int count_matrix_buttons_col_b = sizeof(matrix_buttons_col_b) / sizeof(Button);


void setupMatrix() {
  _setupMatrixCol(matrix_col_a);
  _setupMatrixCol(matrix_col_b);
  
  for(int i=0; i<count_matrix_buttons_col_a; i++) {
    Button* curRowButton = &matrix_buttons_col_a[i];
    _setupMatrixRow(curRowButton->pin);
  }
  
  // put your setup code here, to run once:
  for(int i=0; i<count_matrix_buttons_col_b; i++) {
    Button* curRowButton = &matrix_buttons_col_b[i];
    _setupMatrixRow(curRowButton->pin);
  }
}

void _setupMatrixCol(int pin) {
  pinMode(pin, INPUT_PULLUP);
}

void _setupMatrixRow(int pin) {
  pinMode(pin, INPUT);  
}

int _matrixRead(int curRow) {
  pinMode(curRow, INPUT_PULLUP);
  int val = !digitalRead(curRow);
  pinMode(curRow, INPUT);
  return val;  
}

void _matrixStartCol(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);  
}

void _matrixEndCol(int pin) {
  pinMode(pin, INPUT);  
}


void loopMatrixCol() {
  int curCol = matrix_col_a;
  
  _matrixStartCol(curCol);

  loopMatrixButtons(matrix_buttons_col_a, count_matrix_buttons_col_a);

  _matrixEndCol(curCol);

  curCol = matrix_col_b;
  _matrixStartCol(curCol);

  loopMatrixButtons(matrix_buttons_col_b, count_matrix_buttons_col_b);

  _matrixEndCol(curCol);

}

void loopMatrixButtons(Button buttons[], int count) {
  for(int i=0; i<count; i++) {
    Button* curButton = &buttons[i];

    int curButtonState = _matrixRead(curButton->pin);

    _buttonProcess(curButton, curButtonState);
  }  
}

/*************
 * COMMON
 *************/


int getChannelFromDeck(bool deck) {
  if(deck == DECK_A) {
    return channel_deck_a;
  } else {
    return channel_deck_b;
  }
}

bool isBouncing(unsigned long* last_flake_millis) {
  unsigned long current_flake = millis();
  bool returnValue = current_flake - *last_flake_millis < 50;
  *last_flake_millis = current_flake;
  return returnValue;  
}

void p(char *fmt, ... ){
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.println(buf);
}

/**********
 * ARDUINO
 **********/

void setup() {
  Serial.begin(115200);

  setupKnobs();
  setupPositionLoopKnobs();
  setupMatrix();
}

void loop() {
  if(BENCHMARK == true) {
    BENCH_START_TIME = micros();
  }


  loopKnobs();
  loopPositionLoopKnobs();
  loopMatrixCol();

  if(BENCHMARK == true) {
    BENCH_TOTAL_TIME = micros() - BENCH_START_TIME;
    if(BENCH_TOTAL_TIME > BENCH_MAX_TOTAL) {
      BENCH_MAX_TOTAL = BENCH_TOTAL_TIME;
      p("Benchmark: %i micros", BENCH_MAX_TOTAL);
    }  
  }

  delayMicroseconds(1000);
}
