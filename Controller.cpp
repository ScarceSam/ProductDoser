#include <Arduino.h>
#include "View.h"
#include "State.h"

void controller_updateScreen(void)
{
  uint8_t currentState = state_currentState();
  static uint8_t previousState = 0;

  if (currentState != previousState)
  {
    view_display_state();
    previousState = currentState;
  }
}
