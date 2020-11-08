#include <Arduino.h>
#include "View.h"
#include "State.h"

void controller_update_screen(void)
{
  static uint8_t previous_state = 0;
  static uint16_t previous_millis = 0;

  uint8_t current_state = state_currentState();
  uint16_t current_millis = 0;

  if(current_state == IDLE_STEP)
  {
    current_millis = (millis() / 1000);
  }
  else
  {
    current_millis = (state_remainingMillis() / 1000);
  }

  if ((current_state != previous_state) || (current_millis != previous_millis))
  {
    view_display_state();
    previous_state = current_state;
    previous_millis = current_millis;
  }
}
