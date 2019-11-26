#pragma once

#include <Arduino.h>

class Component {

  public:
    virtual void setup();
    virtual void process();
    virtual void onPotentiometerChange(int midiValue);
};
