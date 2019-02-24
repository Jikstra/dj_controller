#ifndef MATRIX_H
#define MATRIX_H
#include <Arduino.h>

namespace Matrix
{
  void setupCol(int pin);
  void setupRow(int pin);
  void startCol(int pin);
  void endCol(int pin);
  int digitalReadRow(int curRow);
}

#endif
