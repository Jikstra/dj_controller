#pragma once

#include <Arduino.h>

#include "../common.h"
#include "../midi.h"
#include "../debug.h"
#include "CountingRotaryEncoder.h"

class VolumeCountingRotaryEncoder : public CountingRotaryEncoder {

  public:
    VolumeCountingRotaryEncoder(
      int rotary_pin_a, 
      int button_pin,
      int rotary_pin_b, 
      int control_number_value, 
      int control_number_mute, 
      bool deck 
    );
    void handleButtonToggle(bool toggle);
};
