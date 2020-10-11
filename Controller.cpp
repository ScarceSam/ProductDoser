#include <Arduino.h>
#include "View.h"
#include "State.h"

void controller_updateScreen(void)
{
  uint8_t currentState = state_currentState();
  static uint8_t previousState = 0;
  uint16_t currentMillis = (state_remainingMillis() / 1000);
  static uint16_t previousMillis = currentMillis;

  if ((currentState != previousState) || (currentMillis != previousMillis))
  {
    view_display_state();
    previousState = currentState;
    previousMillis = currentMillis;
  }
}
