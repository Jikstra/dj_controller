#include "Matrix.h"

using namespace Matrix;

void Matrix::setupCol(int pin) {
  pinMode(pin, INPUT_PULLUP);
}

void Matrix::setupRow(int pin) {
  pinMode(pin, INPUT);  
}

void Matrix::startCol(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);  
}

void Matrix::endCol(int pin) {
  pinMode(pin, INPUT);  
}

int Matrix::digitalReadRow(int curRow) {
  pinMode(curRow, INPUT_PULLUP);
  int val = !digitalRead(curRow);
  pinMode(curRow, INPUT);
  return val;  
}

