#include <GloveButton.h>

GloveButton::GloveButton(uint8_t pin) {
  this->pin = pin;
}

void GloveButton::setup(void) {
  pinMode(this->pin, INPUT_PULLUP);
}

boolean GloveButton::raw_state(void) {
  return digitalRead(this->pin) == LOW;
}

void GloveButton::read(void) {
  unsigned long now = millis();
  this->event = 0;
  if ( now < this->last_read_millis ) {
    // milli counter overrun?
    // let's just pretend this never happened:
    this->last_read_millis = now;
    return;
  }
  if ( now - this->last_read_millis < this->millis_delay) {
    // last value not old enough yet, so nothing changes here:
    return;
  }
  this->last_read_millis = now;
  // current reading on pin:
  boolean raw_pressed = digitalRead(this->pin) == LOW;
  // integrate value (low pass):
  if ( raw_pressed ) {
    this->cumulate = min(this->cumulate+1, 5);
  } else {
    this->cumulate = max(this->cumulate-1, 0);
  }
  // measured state of the button (pressed?) now:
  boolean new_down = this->cumulate >= 3;
  this->calculate_event(now, new_down);
  // update current status:
  this->down = new_down;
  if ( !this->down ) {
    this->hold = false;
  }
}

void GloveButton::calculate_event(unsigned int now, boolean new_down) {
  if ( now < this->last_event_millis ) {
    // timer overrun:
    this->last_event_millis = now;
    return;
  }
  if ( this->down ) {
    if ( new_down ) {
      if ( this->hold ) {
        if ( now - this->last_event_millis >= this->repeat_millis ) {
          this->event = GloveButton::REPEAT;
          this->last_event_millis = now;
        }
      } else {
        if ( now - this->last_event_millis >= this->hold_millis ) {
          this->hold = true;
          this->event = GloveButton::HOLD | GloveButton::REPEAT;
          this->last_event_millis = now;
        }
      }
    } else {
      this->event = GloveButton::RELEASE;
      this->last_event_millis = now;
    }
  } else {
    if ( new_down ) {
      this->event = GloveButton::PRESS;
      this->last_event_millis = now;
    }
  }
}

