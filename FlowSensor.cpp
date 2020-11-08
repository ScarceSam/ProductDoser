#include <Arduino.h>
#include "Pinmap.h"

static bool ignore_flowsensor = false;
static volatile uint32_t pulse_time[256] = {0};
static volatile uint8_t time_cursor = 0;
static uint16_t max_pulse_delta = 750;

static void increment_count()
{
    ++time_cursor;
    pulse_time[time_cursor] = millis();
}

void flowsensor_init(void)
{
  attachInterrupt(digitalPinToInterrupt(FLOWSENSOR_PIN), increment_count, CHANGE);
}

bool flowsensor_is_flowing(void)
{
  bool b_return_value = false;

  b_return_value = (uint32_t)(millis() - pulse_time[time_cursor - 1] < max_pulse_delta);

  if(ignore_flowsensor)
      b_return_value = true;

  return b_return_value;
}
