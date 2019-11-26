#pragma once

#include <Arduino.h>

namespace Matrix
{
  void setupCol(int pin);
  void setupRow(int pin);
  void startRow(int pin);
  void endRow(int pin);
  int digitalReadCol(int pin);
}
