#include <Arduino.h>
#include "Pinmap.h"
#include "View.h"

static bool ignore_flowsensor = true;
static bool ignore_notice = false;
static volatile uint32_t pulse_time[256] = {0};
static volatile uint8_t time_cursor = 0;
static uint16_t max_pulse_delta = 750;
static volatile bool save_pulse = false;

static void add_pulse()
{
  save_pulse = true;
}

void flowsensor_init(void)
{
  pinMode(FLOWSENSOR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(FLOWSENSOR_PIN), add_pulse, CHANGE);
}

bool flowsensor_is_flowing(void)
{
  bool b_return_value = false;

  if(save_pulse)
  {
    pulse_time[time_cursor] = millis();
    ++time_cursor;
    save_pulse = false;
  }

  uint32_t test_value = pulse_time[(uint8_t)(time_cursor - 40)];

  b_return_value = (uint32_t)(millis() - test_value < max_pulse_delta);

  if(ignore_flowsensor)
  {
      b_return_value = true;
      if(!ignore_notice)
      {
        view_println("Flow Sensor - OFF");
        ignore_notice = true;
      }
  }

  return b_return_value;
}
