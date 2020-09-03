#include <Arduino.h>
#include "Pinmap.h"

static bool ignore_flowsensor = false;
static volatile uint32_t pulseTime[256] = {0};
static volatile uint8_t timeCursor = 0;
static uint16_t maxPulseDelta = 750;

void incrementCount()
{
    ++timeCursor;
    pulseTime[timeCursor] = millis();
}

void flowsensor_init(void)
{
  attachInterrupt(digitalPinToInterrupt(FLOWSENSOR_PIN), incrementCount, CHANGE);
}

bool flowsensor_is_flowing(void)
{
  bool return_value = false;

  return_value = (uint32_t)(millis() - pulseTime[timeCursor - 1] < maxPulseDelta);

  if(ignore_flowsensor)
      return_value = true;

  return return_value;
}
