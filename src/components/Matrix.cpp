#include "Matrix.h"

using namespace Matrix;

void Matrix::setupCol(int pin) {
  pinMode(pin, INPUT_PULLUP);
}

void Matrix::setupRow(int pin) {
  pinMode(pin, INPUT);  
}

void Matrix::startRow(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);  
}

void Matrix::endRow(int pin) {
  pinMode(pin, INPUT);  
}

int Matrix::digitalReadCol(int pin) {
  pinMode(pin, INPUT_PULLUP);
  int val = !digitalRead(pin);
  pinMode(pin, INPUT);
  return val;  
}

