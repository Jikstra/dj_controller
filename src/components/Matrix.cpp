#include "Matrix.h"

void setupMatrixCol(int pin) {
  pinMode(pin, INPUT_PULLUP);
}

void setupMatrixRow(int pin) {
  pinMode(pin, INPUT);  
}

void matrixStartCol(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);  
}

void matrixEndCol(int pin) {
  pinMode(pin, INPUT);  
}

int matrixDigitalReadRow(int curRow) {
  pinMode(curRow, INPUT_PULLUP);
  int val = !digitalRead(curRow);
  pinMode(curRow, INPUT);
  return val;  
}
