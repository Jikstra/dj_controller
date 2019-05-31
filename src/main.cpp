#include "main.h"

const bool BENCHMARK = true;

int BENCH_START_TIME = 0;
int BENCH_TOTAL_TIME = 0;
int BENCH_MAX_TOTAL = 0;


/*************
 * START PIN LAYOUT
 *************/

// V O L U M E   C O U N T I N G  R O T A R Y  E N C O D E R S
VolumeCountingRotaryEncoder volume_counting_rotary_encoders[] = {
  // DECK A
  { 8,   9,  10,  5,  6, DECK_B },
  
  // DECK B
  { 28,  30,  32,  5,  6, DECK_A }
};

const int count_volume_counting_rotary_encoders = sizeof(volume_counting_rotary_encoders) / sizeof(VolumeCountingRotaryEncoder);

// C O U N T I N G  R O T A R Y  E N C O D E R S
CountingRotaryEncoder counting_rotary_encoders[] = {
  // DECK A
  {  A0,  A1,  A2,  9, 10, DECK_B },
  {  A3,  A4,  A5, 11, 12, DECK_B },
  {  A8,  A9, A10, 13, 14, DECK_B },
  { A11, A12, A13, 15, 16, DECK_B },
  {  14,  15,  16, 17, 18, DECK_B },
  {  17,  18,  19, 19, 20, DECK_B },
  
  // DECK B
  {  34,  36,  38,  9, 10, DECK_A },
  {  35,  37,  39, 11, 12, DECK_A },
  {  40,  42,  44, 13, 14, DECK_A },
  {  41,  43,  45, 15, 16, DECK_A },
  {  46,  48,  50, 17, 18, DECK_A },
  {  47,  49,  51, 19, 20, DECK_A }
};

const int count_counting_rotary_encoders = sizeof(counting_rotary_encoders) / sizeof(CountingRotaryEncoder);

// L R  R O T A R Y  E N C O D E R S
LRRotaryEncoder lr_rotary_encoders[] = {
  // DECK A
  {  11,  12,  13,  39,  40, 41, 42, DECK_B },

  // DECK B
  {  29,  31,  33,  39,  40, 41, 42, DECK_A },
};

const int count_lr_rotary_encoders = sizeof(lr_rotary_encoders) / sizeof(LRRotaryEncoder);

PositionKnob position_knobs[] = {
  // DECK B
  { 22, 24, 26, DECK_A },

  // DECK A
  {  2,  3,  4, DECK_B },
};

const int count_position_knobs = sizeof(position_knobs) / sizeof(PositionKnob);

LoopKnob loop_knobs[] = {
  // DECK B
  {  23,  25,  27, DECK_A },

  // DECK A
  {   5,   6,   7, DECK_B },
};

const int count_loop_knobs = sizeof(loop_knobs) / sizeof(LoopKnob);

// M A T R I X
int matrix_col_a = A6;
int matrix_col_b = A7;
int matrix_col_c = A14;
int matrix_col_d = A15;

int matrix_row_1 = 20;
int matrix_row_2 = 21;
int matrix_row_3 = 52;
int matrix_row_4 = 53;

Button matrix_buttons_col_a[] = {
  { matrix_row_1, 50, DECK_B },
  { matrix_row_2, 51, DECK_B },
  { matrix_row_3, 52, DECK_B },
  { matrix_row_4, 53, DECK_B }
};

const int count_matrix_buttons_col_a = sizeof(matrix_buttons_col_a) / sizeof(Button);

Button matrix_buttons_col_b[] = {
  { matrix_row_1, 50, DECK_A },
  { matrix_row_2, 51, DECK_A },
  { matrix_row_3, 52, DECK_A },
  { matrix_row_4, 53, DECK_A }
};

const int count_matrix_buttons_col_b = sizeof(matrix_buttons_col_b) / sizeof(Button);


/*************
 * END PIN LAYOUT
 *************/


void setupVolumeCountingRotaryEncoders() {
  for(int i = 0; i < count_volume_counting_rotary_encoders; i++) {
    // Rotation
    VolumeCountingRotaryEncoder* volume_roti = &volume_counting_rotary_encoders[i];
    volume_roti->setup();
  }
}

void loopVolumeCountingRotaryEncoders() {
  for(int i = 0; i < count_volume_counting_rotary_encoders; i++) {
    // Rotation
    VolumeCountingRotaryEncoder* volume_roti = &volume_counting_rotary_encoders[i];
    volume_roti->process();
  }
}

void setupCountingRotaryEncoders() {
  for(int i = 0; i < count_counting_rotary_encoders; i++) {
    // Rotation
    CountingRotaryEncoder* knob = &counting_rotary_encoders[i];
    knob->setup();
  }
}

void loopCountingRotaryEncoders() {
  for(int i = 0; i < count_counting_rotary_encoders; i++) {
    // Rotation
    //p("test1 %i", i);
    CountingRotaryEncoder* knob = &counting_rotary_encoders[i];
    knob->process();
  }
}

void setupLRRotaryEncoders() {
  for(int i = 0; i < count_lr_rotary_encoders; i++) {
    LRRotaryEncoder* currKnob = &lr_rotary_encoders[i];
    currKnob->setup();
  }
}

void loopLRRotaryEncoders() {
  for(int i = 0; i < count_lr_rotary_encoders; i++) {
    LRRotaryEncoder* currKnob = &lr_rotary_encoders[i];
    currKnob->process();
  }
}

void setupPositionKnobs() {
  for(int i = 0; i < count_position_knobs; i++) {
    PositionKnob* currKnob = &position_knobs[i];
    currKnob->setup();
  }
}

void loopPositionKnobs() {
  for(int i = 0; i < count_position_knobs; i++) {
    PositionKnob* currKnob = &position_knobs[i];

    currKnob->process();
  }
}

void setupLoopKnobs() {
  for(int i = 0; i < count_loop_knobs; i++) {
    LoopKnob* currKnob = &loop_knobs[i];
    currKnob->setup();
  }
}

void loopLoopKnobs() {
  for(int i = 0; i < count_loop_knobs; i++) {
    LoopKnob* currKnob = &loop_knobs[i];

    currKnob->process();
  }
}

void setupMatrix() { 
  Matrix::setupRow(matrix_row_1);
  Matrix::setupRow(matrix_row_2);
  Matrix::setupRow(matrix_row_3);
  Matrix::setupRow(matrix_row_4);

  Matrix::setupCol(matrix_col_a); 
  Matrix::setupCol(matrix_col_b);
  Matrix::setupCol(matrix_col_c);
}

void loopMatrixButtons(Button buttons[], int count) {
  for(int i=0; i<count; i++) {
    Button* curButton = &buttons[i];

    int curButtonState = Matrix::digitalReadRow(curButton->pin);

    curButton->_process(curButtonState);
  }  
}

void loopMatrixChannelSwitches() {
  int deck_a_is_first = Matrix::digitalReadRow(matrix_row_1);
  
  if(deck_a_is_first) {
    setChannelForDeck(DECK_A, 1);
  } else {
    setChannelForDeck(DECK_A, 3);
  }

  int deck_b_is_first = Matrix::digitalReadRow(matrix_row_2);

  if(deck_b_is_first) {
    setChannelForDeck(DECK_B, 2);
  } else {
    setChannelForDeck(DECK_B, 4);
  }

  int step_size_is_top = Matrix::digitalReadRow(matrix_row_3);
  int step_size_is_bottom = Matrix::digitalReadRow(matrix_row_4);

  if(step_size_is_top) {
    setStepSize(1);
  } else if(step_size_is_bottom) {
    setStepSize(8);
  } else {
    setStepSize(4);
  }
}

void loopMatrix() {
  int curCol = matrix_col_a;
  Matrix::startCol(curCol);
  loopMatrixButtons(matrix_buttons_col_a, count_matrix_buttons_col_a);
  Matrix::endCol(curCol);

  curCol = matrix_col_b;
  Matrix::startCol(curCol);
  loopMatrixButtons(matrix_buttons_col_b, count_matrix_buttons_col_b);
  Matrix::endCol(curCol);
  
  curCol = matrix_col_c;
  Matrix::startCol(curCol);
  loopMatrixChannelSwitches();
  Matrix::endCol(curCol);
}


/*******
 * ARDUINO
 ******/


void setup()
{
  Serial.begin(115200);

  setupVolumeCountingRotaryEncoders();
  setupCountingRotaryEncoders();
  setupLRRotaryEncoders();
  setupMatrix();
  setupPositionKnobs();
  setupLoopKnobs();

}


void loop()
{
  loopVolumeCountingRotaryEncoders();
  loopCountingRotaryEncoders();
  loopLRRotaryEncoders();
  loopMatrix();
  loopPositionKnobs(); 
  loopLoopKnobs(); 
}
