#include "LoopKnob.h"

LoopKnob::LoopKnob
    (
      int rotary_pin_a, 
      int button_pin, 
      int rotary_pin_b, 
      bool deck
    ) : 
    RotaryEncoder(rotary_pin_a, button_pin, rotary_pin_b),
    deck(deck) {}

void LoopKnob::handleRotaryTurn(bool turnedLeft) {
  int channel = getChannelFromDeck(deck);

  bool beatmatch_mode = getStepSize() == 1;
  
  int midi_control;

  if(beatmatch_mode == false) {
    midi_control = turnedLeft ?
      midi_loop_halve :
      midi_loop_double;
  } else {
    midi_control = turnedLeft ?
      midi_beatjump_backward_small:
      midi_beatjump_forward_small;
  }
  
  IFNDEBUG(
    // send a MIDI CC -- 56 = note, 127 = velocity, 1 = channel
    midiOut.sendControlChange(
      midi_control,
      1,
      channel
    )
  );

  IFDEBUG(
    p("LoopKnob: %i:%i %i %s", midi_control, channel, 1, turnedLeft ? "Left": "Right");  
  );
}

void LoopKnob::handleButtonState(ButtonState button_state) {
  if(button_state == ButtonState::Unchanged || (int)button_state == 226) return;
  
  IFDEBUG(
    p("LoopButton: %s", buttonStateToString(button_state));
  )
  int channel = getChannelFromDeck(deck);
  bool beatmatch_mode = getStepSize() == 1;
  int midi_control;
  int value_to_send;

  if(beatmatch_mode == true) {
    bool beatgrid_curpos = button_state == ButtonState::Unpressed;
  
    // Send midi_rate_temp_down
    midi_control = midi_rate_temp_up_small;
    value_to_send = button_state == ButtonState::Pressed;

    IFDEBUG(
      p("LoopKnob RateTempDown: %i:%i %i", midi_control, channel, value_to_send)
    );
    IFNDEBUG(midiOut.sendNoteOn(midi_control, value_to_send, channel));
    if(beatgrid_curpos == false) return;

    midi_control = midi_beats_translate_curpos;
    value_to_send = 1;
    IFDEBUG(
      p("LoopKnob BeatGridSync: %i:%i %i", midi_control, channel, value_to_send)
    );
    IFNDEBUG(midiOut.sendNoteOn(midi_control, value_to_send, channel));

      
  } else {
    if(button_state == ButtonState::Pressed) return;

    midi_control = midi_loop_activate;
    value_to_send = 1;
    IFDEBUG(
      p("LoopKnob ReloopToggle: %i:%i %i", midi_control, channel, value_to_send)
    );

    IFNDEBUG(midiOut.sendNoteOn(midi_control, value_to_send, channel));
  }
}

