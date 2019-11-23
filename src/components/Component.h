#ifndef COMPONENT_H
#define COMPONENT_H
#include <Arduino.h>

class Component {

  public:
    virtual void setup();
    virtual void process();
    virtual void onPotentiometerChange(int midiValue);
    virtual void onPotentiometerClick();
};

#endif
