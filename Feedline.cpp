#include <Arduino.h>
#include "Feedline.h"
#include "Pinmap.h"
#include "SDcard.h"

//file scoped functions
void pulse_pump(void);

typedef struct {
  const uint8_t COIL_A = FEEDLINE_PUMP_COIL_A_PIN;
  const uint8_t COIL_B = FEEDLINE_PUMP_COIL_B_PIN;
  bool position_A = 0;
  uint8_t remaining_pump_pulses = 0;
  uint32_t pulse_start_millis = 0;
  const uint8_t COIL_MAX_PULSE_PER_SEC = 5;
  const uint8_t PUMP_MAX_PULSE_PER_SEC = 5;
  const uint32_t PULSE_TIME_MILLI = 1000/((COIL_MAX_PULSE_PER_SEC <= PUMP_MAX_PULSE_PER_SEC) ? COIL_MAX_PULSE_PER_SEC : PUMP_MAX_PULSE_PER_SEC);
  const uint8_t LINE_DRAIN_VALVE_PIN = FEEDLINE_END_DRAIN_VALVE_PIN;
  const uint8_t WATER_VAVLE_PIN = FEEDLINE_WATER_VALVE_PIN;
  const uint8_t FEEDLINE_FLOW_SENSOR = FEEDLINE_FLOW_PIN;
  const uint8_t MANIFOLD_DRAIN_VALVE_PIN = FEEDLINE_MANIFOLD_DRAIN_VALVE_PIN;
  uint8_t flush_oz = 0;
}feedline_t;

static feedline_t feedline_info;

void feedline_init(void)
{
  pinMode(feedline_info.COIL_A, OUTPUT);
  pinMode(feedline_info.COIL_B, OUTPUT);
  pinMode(feedline_info.LINE_DRAIN_VALVE_PIN, OUTPUT);
  pinMode(feedline_info.WATER_VAVLE_PIN, OUTPUT);
  pinMode(feedline_info.MANIFOLD_DRAIN_VALVE_PIN, OUTPUT);
}

void feedline_flush(void)
{
  //purge feedline with water 
  //TODO: - watch for flow meter
  feedline_valve(WATER_VALVE, VALVE_OPEN);
  feedline_valve(LINE_DRAIN_VALVE, VALVE_OPEN);
  uint32_t temp_pause_time = feedline_pump_start(20);
  feedline_info.pulse_start_millis = millis();
  while(feedline_is_pumping())
  {
    feedline_update();
  }
  feedline_valve(ALL_VALVES, VALVE_CLOSE);
}

uint32_t feedline_pump_start(uint8_t volume_half_oz)
{
  //save number of pulses needed for tracking pumping
  feedline_info.remaining_pump_pulses = volume_half_oz;

  //caculate estimated time to finish pumping
  uint32_t time_to_complete = (feedline_info.remaining_pump_pulses * feedline_info.PULSE_TIME_MILLI);

  pulse_pump();

  //remember the start of the above pulse
  feedline_info.pulse_start_millis = millis();

  //return the estimated time
  return time_to_complete;  
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

  //update pulses tracking info
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
    //pulse the pump and update tracking if last pulse is done
    pulse_pump();
    feedline_info.pulse_start_millis = millis();
  }
}

bool feedline_is_pumping(void)
{
  //is the pump running?
  bool return_value = 0;
  
  return_value = !((uint32_t)(millis() - feedline_info.pulse_start_millis) > feedline_info.PULSE_TIME_MILLI);

  return_value = (return_value) || (feedline_info.remaining_pump_pulses > 0);

  return (0 != return_value);
}

bool feedline_load(void)
{
  uint32_t volume = SDcard_read_int("system", "flushvolumeoz");

  Serial.println(volume);

  feedline_info.flush_oz = volume;

  return volume;
}
