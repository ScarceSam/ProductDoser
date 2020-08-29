#include <Arduino.h>
#include "Pinmap.h"

static volatile uint32_t pulseTime[256] = {0};
static volatile uint8_t timeCursor = 0;
static uint8_t minTime = 200;
static uint8_t maxTime = 750;

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

  return_value = (uint32_t)(millis() - pulseTime[timeCursor - 1] < minTime) || (uint32_t)(millis() - pulseTime[timeCursor - 1] < maxTime);

  return return_value;
}
