#include <Arduino.h>
#include "Feedline.h"
#include "Pinmap.h"
#include "SDcard.h"
#include "FlowSensor.h"

typedef struct {
  const uint8_t PUMP_PIN = FEEDLINE_PUMP_PIN;
  bool position_A = 0;
  uint32_t pump_time = 0;
  uint32_t pulse_start_millis = 0;
  const uint8_t COIL_MAX_PULSE_PER_SEC = 5;
  const uint8_t PUMP_MAX_PULSE_PER_SEC = 5;
  const uint32_t PULSE_TIME_MILLI = 1000/((COIL_MAX_PULSE_PER_SEC <= PUMP_MAX_PULSE_PER_SEC) ? COIL_MAX_PULSE_PER_SEC : PUMP_MAX_PULSE_PER_SEC);
  const uint8_t LINE_DRAIN_VALVE_PIN = FEEDLINE_END_DRAIN_VALVE_PIN;
  const uint8_t WATER_VAVLE_PIN = FEEDLINE_WATER_VALVE_PIN;
  const uint8_t MANIFOLD_DRAIN_VALVE_PIN = FEEDLINE_MANIFOLD_DRAIN_VALVE_PIN;
  uint8_t flush_oz = 0;
  uint8_t manifold_oz = 0;
  uint8_t oz_per_min = 40;
  uint8_t flush_seconds = 10;
}feedline_t;

static feedline_t feedline_info;

void feedline_init(void)
{
  pinMode(feedline_info.PUMP_PIN, OUTPUT);
  pinMode(feedline_info.LINE_DRAIN_VALVE_PIN, OUTPUT);
  pinMode(feedline_info.WATER_VAVLE_PIN, OUTPUT);
  pinMode(feedline_info.MANIFOLD_DRAIN_VALVE_PIN, OUTPUT);
}

void feedline_flush(void)
{
  //purge feedline with water 
  feedline_valve(WATER_VALVE, VALVE_OPEN);
  feedline_valve(LINE_DRAIN_VALVE, VALVE_OPEN);
  feedline_run_pump(true);
  uint32_t start_time = millis();
  while((uint32_t)(millis() - start_time) < (feedline_info.flush_seconds * 1000))
  {
    if(!flowsensor_is_flowing())
        start_time = millis();
  }
  feedline_run_pump(false);
  feedline_valve(ALL_VALVES, VALVE_CLOSE);
}

uint32_t feedline_pump_millis(uint8_t volume_half_oz)
{
  //save number of pulses needed for tracking pumping
  feedline_info.pump_time = (((60000 / feedline_info.oz_per_min) * volume_half_oz) / 2);

  //caculate estimated time to finish pumping
  //uint32_t time_to_complete = (feedline_info.remaining_pump_pulses * feedline_info.PULSE_TIME_MILLI);

  //remember the start of the above pulse
  feedline_info.pulse_start_millis = millis();

  //return the estimated time
  return feedline_info.pump_time;//time_to_complete;  
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

void feedline_run_pump(bool power)
{
  digitalWrite(feedline_info.PUMP_PIN, power);
}

uint8_t feedline_flush_oz(void)
{
  return feedline_info.flush_oz;
}

uint8_t feedline_manifold_oz(void)
{
  return feedline_info.manifold_oz;
}

void feedline_update(void)
{
  if((uint32_t)(millis() - feedline_info.pulse_start_millis) > feedline_info.pump_time)
  {
    feedline_run_pump(false);
  }
}

bool feedline_is_pumping(void)
{
  //is the pump running?
  bool return_value = 0;
  
  return_value = !((uint32_t)(millis() - feedline_info.pulse_start_millis) > feedline_info.pump_time);

  return return_value;
}

bool feedline_load(void)
{
  bool return_value = 1;

  uint32_t volume = SDcard_read_int("system", "flushvolumeoz");
  feedline_info.flush_oz = (uint8_t)volume;

  if(volume == 0)
  {
    return_value = 0;
  }

  volume = 0;
  volume = SDcard_read_int("system", "manifoldvolumeoz");
  feedline_info.manifold_oz = (uint8_t)volume;

  if(volume == 0)
  {
    return_value = 0;
  }

  return return_value;
}
