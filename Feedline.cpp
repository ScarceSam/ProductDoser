#include <Arduino.h>
#include "Feedline.h"
#include "Pinmap.h"
#include "SDcard.h"
#include "FlowSensor.h"
#include "Chars.h"

typedef struct {
  const uint8_t LINE_DRAIN_VALVE_PIN = FEEDLINE_END_DRAIN_VALVE_PIN;
  const uint8_t WATER_VAVLE_PIN = FEEDLINE_WATER_VALVE_PIN;
  const uint8_t MANIFOLD_DRAIN_VALVE_PIN = FEEDLINE_MANIFOLD_DRAIN_VALVE_PIN;
  uint32_t flush_millis = 10000;
}feedline_t;

static feedline_t feedline_info;

void feedline_init(void)
{
  pinMode(feedline_info.LINE_DRAIN_VALVE_PIN, OUTPUT);
  pinMode(feedline_info.WATER_VAVLE_PIN, OUTPUT);
  pinMode(feedline_info.MANIFOLD_DRAIN_VALVE_PIN, OUTPUT);
}

void feedline_flush(void)
{
  //purge feedline with water 
  feedline_valve(WATER_VALVE, VALVE_OPEN);
  feedline_valve(LINE_DRAIN_VALVE, VALVE_OPEN);
  uint32_t start_time = millis();
  while((uint32_t)(millis() - start_time) < (feedline_info.flush_millis))
  {
    if(!flowsensor_is_flowing())
        start_time = millis();
  }
  feedline_valve(ALL_VALVES, VALVE_CLOSE);
}

void feedline_valve(uint8_t valve, uint8_t state)
{
  switch(valve)
  {
    case WATER_VALVE:
      digitalWrite(feedline_info.WATER_VAVLE_PIN, state);
      break;
    case LINE_DRAIN_VALVE:
      digitalWrite(feedline_info.LINE_DRAIN_VALVE_PIN, state);
      break;
    case MANIFOLD_DRAIN_VALVE:
      digitalWrite(feedline_info.MANIFOLD_DRAIN_VALVE_PIN, state);
      break;
    case ALL_VALVES:
      digitalWrite(feedline_info.WATER_VAVLE_PIN, state);
      digitalWrite(feedline_info.LINE_DRAIN_VALVE_PIN, state);
      digitalWrite(feedline_info.MANIFOLD_DRAIN_VALVE_PIN, state);
  }
}

uint32_t feedline_flush_millis(void)
{
  return feedline_info.flush_millis;
}

bool feedline_load(void)
{
  bool b_return_value = false;

  uint8_t duration = 0;

  if(!SDcard_read_int("system", "flushtimeseconds", &duration))
  {
    feedline_info.flush_millis = (uint32_t)(duration * 1000);
    b_return_value = true;
  }

  return b_return_value;
}
