#ifndef COMMON_H
#define COMMON_H

#include "Arduino.h"
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>

const bool DECK_A = 0;
const bool DECK_B = 1;

extern int channel_deck_b;
extern int channel_deck_b;

void p(char *fmt, ... );
int getChannelFromDeck(bool deck);
bool isBouncing(unsigned long* last_flake_millis);



/*
 *
 *
 * define MIDI_CREATE_INSTANCE(Type, SerialPort, Name)                            \
 * midi::MidiInterface<Type> Name((Type&)SerialPort);
 *
 * MIDI_CREATE_INSTANCE(HardwareSerial,Serial, midiOut); // create a MIDI object called midiOut
*/

extern midi::MidiInterface<HardwareSerial> midiOut;

#endif
