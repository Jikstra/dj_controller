#ifndef MATRIX_H
#define MATRIX_H
#include <Arduino.h>

void setupMatrixCol(int pin);
void setupMatrixRow(int pin);
void matrixStartCol(int pin);
void matrixStartCol(int pin);
int matrixDigitalReadRow(int curRow);

#endif
