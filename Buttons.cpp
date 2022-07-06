#include <Arduino.h>
#include "Buttons.h"
#include "Pinmap.h"

#define BUTTON_PUSHED 0

static const int BUTTONS[] = {BUTTON_UP, BUTTON_LEFT, BUTTON_ENTER, BUTTON_RIGHT, BUTTON_DOWN, BUTTON_RETURN};
static const int BUTTON_COUNT = (sizeof(BUTTONS)/sizeof(BUTTONS[0]));
static const int DEBOUNCE = 64;

void buttons_init(void)
{
  for(int i = 0; i < BUTTON_COUNT; i++)
  {
    pinMode(BUTTONS[i], INPUT_PULLUP);
  }
}

int buttons_pushed(void)
{
  int pushed = 0;
  //static uint32_t last_check = millis();
  uint32_t current_check = millis();
  static uint32_t pushed_buttons[BUTTON_COUNT][2];// 0=button state 1=button time

  for(int i = 0; i < BUTTON_COUNT; i++)
  {
    bool button_state = digitalRead(BUTTONS[i]);

    if (button_state != BUTTON_PUSHED)
    {
      pushed_buttons[i][0] = !BUTTON_PUSHED;
      pushed_buttons[i][1] = 0;
    }
    else if ((button_state == 0) && (pushed_buttons[i][1] == 0))
    {
      pushed_buttons[i][1] = current_check;
    }
    else if ((button_state == 0) && (current_check > (pushed_buttons[i][1] + DEBOUNCE)) && (pushed_buttons[i][0] != BUTTON_PUSHED))
    {
      pushed = BUTTONS[i];
      pushed_buttons[i][0] = BUTTON_PUSHED;
    }
  }

  return pushed;
}
