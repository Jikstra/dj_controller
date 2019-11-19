#ifndef COMPONENT_H
#define COMPONENT_H
#include <Arduino.h>

#include "common.h"

class Component {

  public:
    virtual void setup();
    virtual void process();
};

#endif
