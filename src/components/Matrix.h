#ifndef MATRIX_H
#define MATRIX_H
#include <Arduino.h>

namespace Matrix
{
  void setupCol(int pin);
  void setupRow(int pin);
  void startRow(int pin);
  void endRow(int pin);
  int digitalReadCol(int pin);
}

#endif
