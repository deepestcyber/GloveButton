#ifndef GloveButton_h
#define GloveButton_h

#include "Arduino.h"

class GloveButton {
  // down, justPressed, justReleased
  // justRepeated, justLongPressed
  // pressed, long, repeat, release
  public:
    static const uint8_t PRESS   = B00000001;
    static const uint8_t HOLD    = B00000010;
    static const uint8_t REPEAT  = B00000100;
    static const uint8_t RELEASE = B00001000;
  private:
    uint8_t pin;
    unsigned long last_read_millis = 0;
    unsigned long millis_delay = 10;
    int8_t cumulate = 0;
    boolean down = false;
    boolean hold = false;
    uint8_t event = 0;
    unsigned long last_event_millis = 0;
    unsigned long hold_millis = 500;
    unsigned long repeat_millis = 100;
    void calculate_event(unsigned int now, boolean new_down);
  public:
    GloveButton(uint8_t pin);
    void setup(void);
    boolean raw_state(void);
    void read(void);
    boolean get_down(void) { return this->down; }
    int8_t get_cumulate(void) { return this->cumulate; }
    uint8_t get_event(void) { return this->event; }
};


#endif // GloveButton_h

