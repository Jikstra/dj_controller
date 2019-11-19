#include "main.h"

const bool BENCHMARK = true;

int BENCH_START_TIME = 0;
int BENCH_TOTAL_TIME = 0;
int BENCH_MAX_TOTAL = 0;


/*************
 * START PIN LAYOUT
 *************/

Component* components[] = {
  new VolumeCountingRotaryEncoder(32,   30,    28,  5,  6, DECK_A),
  new VolumeCountingRotaryEncoder(A11,  A12,  A13,  5,  6, DECK_B),

  new CountingRotaryEncoder(17,  18,  19, 13, 14, DECK_A), // High
  new CountingRotaryEncoder(26,  24,  22, 11, 12, DECK_A), // Mid
  new CountingRotaryEncoder(27,  25,  23,  9, 10, DECK_A), // Low
  new CountingRotaryEncoder(13,  12,  11, 19, 20, DECK_A), // Fx3
  new CountingRotaryEncoder(10,   9,   8, 17, 18, DECK_A), // Fx2
//new CountingRotaryEncoder( 7,   6,   5, 15, 16, DECK_A), // Fx1
  new CountingRotaryEncoder( 5,   6,   7, 13, 14, DECK_B), // High
  new CountingRotaryEncoder(A3,  A4,  A5, 11, 12, DECK_B), // Mid
  new CountingRotaryEncoder(A8,  A9,  A10, 9, 10, DECK_B), // Low
  new CountingRotaryEncoder(41,  39,  37, 19, 20, DECK_B), // Fx3
  new CountingRotaryEncoder(40,  38,  36, 17, 18, DECK_B), // Fx2
//new CountingRotaryEncoder(47,  45,  43, 15, 16, DECK_B), // Fx1

  new PositionKnob(33, 31, 29, DECK_A),
  new PositionKnob(52, 50, 48, DECK_B),

  new LoopKnob(14, 15, 16, DECK_A),
  new LoopKnob(53, 51, 49, DECK_B),

  new LRRotaryEncoder( 4,   3,   2,  39,  40, 41, 42, DECK_A), // BPM
  new LRRotaryEncoder(46,  44,  42,  39,  40, 41, 42, DECK_B), // BPM

  new Potentiometer(A1, 47, DECK_A),
  new Potentiometer(A2, 45, DECK_B)
};

const int count_components = sizeof(components) / sizeof(Component);

// M A T R I X
int matrix_row_1 = A6;
int matrix_row_2 = A7;
int matrix_row_3 = A14;
int matrix_row_4 = A15;

int matrix_col_a = 20;
int matrix_col_b = 21;
int matrix_col_c = 34;
int matrix_col_d = 35;

Button matrix_buttons_col_a[] = {
  { matrix_col_a, 50, DECK_A },
  { matrix_col_b, 51, DECK_A },
  { matrix_col_c, 52, DECK_A },
  { matrix_col_a, 53, DECK_A }
};

const int count_matrix_buttons_col_a = sizeof(matrix_buttons_col_a) / sizeof(Button);

Button matrix_buttons_col_b[] = {
  { matrix_col_a, 50, DECK_B },
  { matrix_col_b, 51, DECK_B },
  { matrix_col_c, 52, DECK_B },
  { matrix_col_b, 53, DECK_B }
};

const int count_matrix_buttons_col_b = sizeof(matrix_buttons_col_b) / sizeof(Button);


/*************
 * END PIN LAYOUT
 *************/

void setupComponents() {
  for(int i = 0; i < count_components; i++) {
    Component* currComponent = components[i];

    currComponent->setup();
  }
}

void loopComponents() {
  for(int i = 0; i < count_components; i++) {
    Component* currComponent = components[i];

    currComponent->process();
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
  for(int i=0; i<count-1; i++) {
    Button* curButton = &buttons[i];

    int curButtonState = Matrix::digitalReadCol(curButton->pin);

    curButton->_process(curButtonState);
  }  
}

void loopMatrixChannelSwitches() {
  int deck_a_is_first = Matrix::digitalReadCol(matrix_col_c);
  
  if(deck_a_is_first) {
    setChannelForDeck(DECK_A, 1);
  } else {
    setChannelForDeck(DECK_A, 3);
  }


  int step_size_is_top = Matrix::digitalReadCol(matrix_col_a);
  int step_size_is_bottom = Matrix::digitalReadCol(matrix_col_b);

  if(step_size_is_top) {
    setStepSize(1);
  } else if(step_size_is_bottom) {
    setStepSize(8);
  } else {
    setStepSize(4);
  }
}

void loopMatrixHotfix() {

  Button* curButton = &matrix_buttons_col_a[3];
  int curButtonState = Matrix::digitalReadCol(curButton->pin);
  curButton->_process(curButtonState);

  curButton = &matrix_buttons_col_b[3];
  curButtonState = Matrix::digitalReadCol(curButton->pin);
  curButton->_process(curButtonState);

  int deck_b_is_first = Matrix::digitalReadCol(matrix_col_c);

  if(deck_b_is_first) {
    setChannelForDeck(DECK_B, 2);
  } else {
    setChannelForDeck(DECK_B, 4);
  }
}

void loopMatrix() {
  int curRow = matrix_row_1;
  Matrix::startRow(curRow);
  loopMatrixButtons(matrix_buttons_col_a, count_matrix_buttons_col_a);
  Matrix::endRow(curRow);

  curRow = matrix_row_2;
  Matrix::startRow(curRow);
  loopMatrixButtons(matrix_buttons_col_b, count_matrix_buttons_col_b);
  Matrix::endRow(curRow);
  
  curRow = matrix_row_3;
  Matrix::startRow(curRow);
  loopMatrixChannelSwitches();
  Matrix::endRow(curRow);

  curRow = matrix_row_4;
  Matrix::startRow(curRow);
  loopMatrixHotfix();
  Matrix::endRow(curRow);
}


/*******
 * ARDUINO
 ******/


void setup()
{
  Serial.begin(115200);

  setupMatrix();
  setupComponents();
}


void loop()
{
  loopMatrix();
  loopComponents();
}
