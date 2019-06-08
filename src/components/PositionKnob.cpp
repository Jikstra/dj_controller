#include "PositionKnob.h"

PositionKnob::PositionKnob
    (
      int rotary_pin_a, 
      int button_pin, 
      int rotary_pin_b, 
      bool deck
    ) : 
    RotaryEncoder(rotary_pin_a, button_pin, rotary_pin_b),
    deck(deck) {}

void PositionKnob::handleRotaryTurn(bool turnedLeft) {
  turnedLeft = !turnedLeft;
  int channel = getChannelFromDeck(deck);

  int step_size = getStepSize();
  
  int midi_control;

  if(step_size == 1) {
    midi_control = turnedLeft ?
      midi_beatjump_backward_1 :
      midi_beatjump_forward_1;
  } else if(step_size == 4) {
    midi_control = turnedLeft ?
      midi_beatjump_backward_2 :
      midi_beatjump_forward_2;
  } else {
    midi_control = turnedLeft ?
      midi_beatjump_backward_3 :
      midi_beatjump_forward_3;
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
    p("PositionKnob [%i]: %i:%i %i %s", step_size, midi_control, channel, 1, turnedLeft ? "Left": "Right");  
  );
}

void PositionKnob::handleButtonState(ButtonState button_state) {
  if(button_state == ButtonState::Unchanged || (int)button_state == 226) return;
  
  IFDEBUG(
    p("PositionButton: %s", buttonStateToString(button_state));
  )
  int channel = getChannelFromDeck(deck);
  bool beatmatch_mode = getStepSize() == 1;
  int midi_control;
  int value_to_send;

  if(beatmatch_mode == true) {
    bool beatgrid_curpos = button_state == ButtonState::Unpressed;
  
    // Send midi_rate_temp_down
    midi_control = midi_rate_temp_down_small;
    value_to_send = button_state == ButtonState::Pressed;

    IFDEBUG(
      p("PositionKnob RateTempDown: %i:%i %i", midi_control, channel, value_to_send)
    );
    IFNDEBUG(midiOut.sendNoteOn(midi_control, value_to_send, channel));
    if(beatgrid_curpos == false) return;

    midi_control = midi_beats_translate_curpos;
    value_to_send = 1;
    IFDEBUG(
      p("PositionKnob BeatGridSync: %i:%i %i", midi_control, channel, value_to_send)
    );
    IFNDEBUG(midiOut.sendNoteOn(midi_control, value_to_send, channel));

      
  } else {
    if(button_state == ButtonState::Pressed) return;

    midi_control = midi_reloop_toggle;
    value_to_send = 1;
    IFDEBUG(
      p("PositionKnob ReloopToggle: %i:%i %i", midi_control, channel, value_to_send)
    );

    IFNDEBUG(midiOut.sendNoteOn(midi_control, value_to_send, channel));
  }
}

