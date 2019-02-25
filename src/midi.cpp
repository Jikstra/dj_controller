#include "midi.h"

midi::MidiInterface<HardwareSerial> midiOut((HardwareSerial&)Serial);
