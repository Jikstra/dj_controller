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

Rotary rotary1 = Rotary(5, 6);
int counter1 = 0;

Rotary rotary2 = Rotary(3, 4);
int counter2 = 0;


void setup() {
  Serial.begin(9600);
}

void loop() {
  /*Serial.println("hey");
  int i = 0;
  for(i = 0; i < count_rotaries; i++) {
    unsigned char result = rotaries[i].process();
    if (result == DIR_CW) {
      counters[i]++;
      Serial.println(String(i) + " " + String(counters[i]));
    } else if (result == DIR_CCW) {
      counters[i]--;
      Serial.println(String(i) + " " + String(counters[i]));
    }  
  }*/
  
  int result1 = rotary1.process();
  if (result1 == DIR_CW) {
      counter1++;
      Serial.println("1 " + String(counter1));
  } else if (result1 == DIR_CCW) {
      counter1--;
      Serial.println("1 " + String(counter1));
  }  

  int result2 = rotary2.process();
  //Serial.println(result2);
  if (result2 == DIR_CW) {
      counter2++;
      Serial.println("2 " + String(counter2));
  } else if (result2 == DIR_CCW) {
      counter2--;
      Serial.println("2 " + String(counter2));
  }  

  
}
