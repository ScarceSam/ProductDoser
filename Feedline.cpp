#include <Arduino.h>
#include "Feedline.h"
#include "Pinmap.h"


typedef struct {
  const uint8_t COIL_A = FEEDLINE_PUMP_COIL_A_PIN;
  const uint8_t COIL_B = FEEDLINE_PUMP_COIL_B_PIN;
  bool position_A = 0;
  uint8_t remaining_pump_pulses = 0;
  uint32_t pulse_start_millis = 0;
  const uint8_t COIL_MAX_PULSE_PER_SEC = 5;
  const uint8_t PUMP_MAX_PULSE_PER_SEC = 5;
  const uint32_t PULSE_TIME_MILLI = 1000/((COIL_MAX_PULSE_PER_SEC <= PUMP_MAX_PULSE_PER_SEC) ? COIL_MAX_PULSE_PER_SEC : PUMP_MAX_PULSE_PER_SEC);
  const uint8_t line_drain_valve_pin = FEEDLINE_END_DRAIN_VALVE_PIN;
  const uint8_t water_valve_pin = FEEDLINE_WATER_VALVE_PIN;
  const uint8_t feedline_flow_sensor = FEEDLINE_FLOW_PIN;
  const uint8_t manifold_drain_valve_pin = FEEDLINE_MANIFOLD_DRAIN_VALVE_PIN;
  uint8_t flush_oz = 10;
}feedline_t;

static feedline_t feedline_info;

void feedline_init(void)
{
  pinMode(feedline_info.COIL_A, OUTPUT);
  pinMode(feedline_info.COIL_B, OUTPUT);
  pinMode(feedline_info.line_drain_valve_pin, OUTPUT);
  pinMode(feedline_info.water_valve_pin, OUTPUT);
  pinMode(feedline_info.manifold_drain_valve_pin, OUTPUT);
}

uint32_t feedline_pump_start(uint8_t volume_half_oz)
{
  feedline_info.remaining_pump_pulses = volume_half_oz;
  uint32_t time_to_complete = (feedline_info.remaining_pump_pulses * feedline_info.PULSE_TIME_MILLI);
  pulse_pump();
  feedline_info.pulse_start_millis = millis();
  return time_to_complete;  
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

void pulse_pump(void)
{
  if(feedline_info.position_A)
  {
    digitalWrite(feedline_info.COIL_A, LOW);
    digitalWrite(feedline_info.COIL_B, HIGH);
    feedline_info.position_A = false;
  }
  else
  {
    digitalWrite(feedline_info.COIL_B, LOW);
    digitalWrite(feedline_info.COIL_A, HIGH);
    feedline_info.position_A = true;
  }
  
  feedline_info.remaining_pump_pulses--;
}

uint8_t feedline_flush_oz(void)
{
  return feedline_info.flush_oz;
}

void feedline_update(void)
{
  if((feedline_info.remaining_pump_pulses) && ((uint32_t)(millis() - feedline_info.pulse_start_millis) > feedline_info.PULSE_TIME_MILLI))
  {
    pulse_pump();
    feedline_info.pulse_start_millis = millis();
  }
}

bool feedline_pumping(void)
{
  return !((uint32_t)(millis() - feedline_info.pulse_start_millis) > feedline_info.PULSE_TIME_MILLI);
}
