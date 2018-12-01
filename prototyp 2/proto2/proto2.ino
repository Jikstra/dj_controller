#include <Rotary.h>

/*
 * Example using the Rotary library, dumping integers to the serial
 * port. The integers increment or decrement depending on the direction
 * of rotation.
 *
 * This example uses polling rather than interrupts.
 */

#include <Rotary.h>

// Rotary encoder is wired with the common to ground and the two
// outputs to pins 5 and 6.
/*Rotary rotaries[] = {Rotary(5, 6), Rotary(1, 2)};
const int count_rotaries = sizeof(rotaries);
int counters[count_rotaries] = {0};*/

Rotary rotaries[] = {
  Rotary(1, 2),
  Rotary(3, 4),
  Rotary(5, 6),
};

const int count_rotaries = 3;

int counters[count_rotaries] = {0};

int startTime;
int endTime;

void setup() {
  Serial.begin(9600);
}

void loop() {
  //startTime = millis();
  int i;
  for(i = 0; i < count_rotaries; i++) {
    int result = rotaries[i].process();
    if (result == DIR_NONE) {
      continue;
    }

    if (result == DIR_CW) {
      counters[i]++;
    } else {
      counters[i]--;
    }
    
    log(i);
  }
  //endTime = millis();
  //Serial.println(endTime - startTime);

}

void log(int i) {
  Serial.println(String(i) + " " + String(counters[i]));
}
