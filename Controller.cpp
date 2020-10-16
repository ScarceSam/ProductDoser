#include <Arduino.h>
#include "View.h"
#include "State.h"

void controller_updateScreen(void)
{
  uint8_t currentState = state_currentState();
  static uint8_t previousState = 0;
  uint16_t currentMillis = 0;
  static uint16_t previousMillis = currentMillis;

  if(currentState == IDLE_STEP)
  {
    currentMillis = (millis() / 1000);
  }
  else
  {
    currentMillis = (state_remainingMillis() / 1000);
  }

  if ((currentState != previousState) || (currentMillis != previousMillis))
  {
    view_display_state();
    previousState = currentState;
    previousMillis = currentMillis;
  }
}
