#include <Arduino.h>
#include "Feedline.h"
#include "Pinmap.h"

typedef struct {
  const uint8_t pump_pin = FEEDLINE_PUMP_PIN;
  const uint8_t line_drain_valve_pin = FEEDLINE_END_DRAIN_VALVE_PIN;
  const uint8_t water_valve_pin = FEEDLINE_WATER_VALVE_PIN;
  const uint8_t feedline_flow_sensor = FEEDLINE_FLOW_PIN;
  const uint8_t manifold_drain_valve_pin = FEEDLINE_MANIFOLD_DRAIN_VALVE_PIN;
  uint32_t flush_oz_x1000 = 2000;
  uint16_t calibration_volume_oz = 87;
  uint16_t calibration_time_sec = 300;
}feedline_t;

static feedline_t feedline_info;

void feedline_init(void)
{
  pinMode(feedline_info.pump_pin, OUTPUT);
  pinMode(feedline_info.line_drain_valve_pin, OUTPUT);
  pinMode(feedline_info.water_valve_pin, OUTPUT);
  pinMode(feedline_info.manifold_drain_valve_pin, OUTPUT);
}

uint32_t feedline_flush_time_milli(void)
{
  uint32_t volume_oz_x1000 = (feedline_info.flush_oz_x1000);
  uint32_t cal_time_milli = (feedline_info.calibration_time_sec * 1000);
  uint64_t time_volume = (cal_time_milli * volume_oz_x1000);
  uint32_t cal_oz_x1000 = (feedline_info.calibration_volume_oz * 1000);
  uint32_t rinse_time_milli = (time_volume / cal_oz_x1000);
  return rinse_time_milli;
}

void feedline_pump(uint8_t state)
{
  digitalWrite(feedline_info.pump_pin, state);
}

void feedline_valve(uint8_t valve, uint8_t state)
{
  switch(valve)
  {
    case WATER_VALVE:
      digitalWrite(feedline_info.water_valve_pin, state);
      break;
    case LINE_DRAIN_VALVE:
      digitalWrite(feedline_info.line_drain_valve_pin, state);
      break;
    case MANIFOLD_DRAIN_VALVE:
      digitalWrite(feedline_info.manifold_drain_valve_pin, state);
      break;
    case ALL_VALVES:
      digitalWrite(feedline_info.water_valve_pin, state);
      digitalWrite(feedline_info.line_drain_valve_pin, state);
      digitalWrite(feedline_info.manifold_drain_valve_pin, state);
  }
}
