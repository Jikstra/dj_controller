#include "main.h"

const bool BENCHMARK = false;

int BENCH_START_TIME = 0;
int BENCH_TOTAL_TIME = 0;
int BENCH_MAX_TOTAL = 0;


/*************
 * START PIN LAYOUT
 *************/

CountingRotaryEncoder knobs[] = {
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

int matrix_col_a = A6;
int matrix_col_b = A7;
int matrix_col_c = A14;

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

Button matrix_buttons_col_c[] = {
  { 20, 60, DECK_B },
  { 21, 61, DECK_B },
  { 52, 62, DECK_B },
  { 53, 63, DECK_B }
};

const int count_matrix_buttons_col_c = sizeof(matrix_buttons_col_c) / sizeof(Button);


const int count_knobs = sizeof(knobs) / sizeof(CountingRotaryEncoder);
//int count_knobs = 2;

/*************
 * END PIN LAYOUT
 *************/

void setupKnobs() {
  for(int i = 0; i < count_knobs; i++) {
    // Rotation
    CountingRotaryEncoder* knob = &knobs[i];
    knob->setup();
    free(knob);
  }
}

void setupMatrix() {
  Matrix::setupCol(matrix_col_a);
  
  for(int i=0; i<count_matrix_buttons_col_a; i++) {
    Button* curRowButton = &matrix_buttons_col_a[i];
    Matrix::setupRow(curRowButton->pin);
  }
  
  Matrix::setupCol(matrix_col_b);
  // put your setup code here, to run once:
  for(int i=0; i<count_matrix_buttons_col_b; i++) {
    Button* curRowButton = &matrix_buttons_col_b[i];
    Matrix::setupRow(curRowButton->pin);
  }
}

void setup()
{
  Serial.begin(115200);
  setupKnobs();
  setupMatrix();
}

void loopKnobs() {
  for(int i = 0; i < count_knobs; i++) {
    // Rotation
    CountingRotaryEncoder* knob = &knobs[i];
    knob->process();
    free(knob);
  }
}
void loopMatrixButtons(Button buttons[], int count) {
  for(int i=0; i<count; i++) {
    Button* curButton = &buttons[i];

    int curButtonState = Matrix::digitalReadRow(curButton->pin);

    curButton->_process(curButtonState);
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

}

void loop()
{
  loopKnobs();
  loopMatrix();
}
