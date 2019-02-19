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


/*************
 * START PIN LAYOUT
 *************/

Knob knobs[] = {
  // DECK A
  {   2,   3,   4,  1,  2, 1, 4 },
  {   5,   6,   7,  3,  4, 1, 4 },
  {   8,   9,  10,  5,  6, 1, 4 },
  {  11,  12,  13,  7,  8, 1, 4 },
  {  A0,  A1,  A2,  9, 10, 1, 4 },
  {  A3,  A4,  A5, 11, 12, 1, 4 },
  {  A8,  A9, A10, 13, 14, 1, 4 },
  { A11, A12, A13, 15, 16, 1, 4 },
  {  14,  15,  16, 17, 18, 1, 4 },
  {  17,  18,  19, 19, 20, 1, 4 },
  
  // DECK B
  {  22,  24,  26,  1,  2, 2, 4 },
  {  23,  25,  27,  3,  4, 2, 4 },
  {  28,  30,  32,  5,  6, 2, 4 },
  {  29,  31,  33,  7,  8, 2, 4 },
  {  34,  36,  38,  9, 10, 2, 4 },
  {  35,  37,  39, 11, 12, 2, 4 },
  {  40,  42,  44, 13, 14, 2, 4 },
  {  41,  43,  45, 15, 16, 2, 4 },
  {  46,  48,  50, 17, 18, 2, 4 },
  {  47,  49,  51, 19, 20, 2, 4 }

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

void knobSetup(Knob* knob) {
   pinMode(knob->button_pin, INPUT_PULLUP);
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
    unsigned long current_flake = millis();
    if(current_flake - button->last_flake < 50) return;
    button->switch_is_up = !button->switch_is_up;
    button->switch_was_up = true;
    button->last_flake = current_flake;
  } else if(state == HIGH && button->switch_was_up == true){
    unsigned long current_flake = millis();
    if(current_flake - button->last_flake < 50) return;
    button->switch_was_up = false;
    button->last_flake = current_flake;
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

void p(char *fmt, ... ){
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.println(buf);
}

void setup() {
  Serial.begin(115200);
  for(int i = 0; i < count_knobs; i++) {
    knobSetup(&knobs[i]);
  }

  setupMatrix();
  
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
