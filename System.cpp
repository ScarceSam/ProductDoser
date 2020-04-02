#include <Arduino.h>
#include "System.h"

#define UPDATE_INTERVAL 100

enum valves{ LINE_DRAIN_VALVE, WATER_VALVE, MANIFOLD_DRAIN_VALVE, ALL_VALVES };
enum steps{ IDLE_STEP, DOSE_STEP, FLUSH_STEP, RINSE_STEP };

typedef struct {
  const uint8_t pump_pin = 14;
  const uint8_t line_drain_valve_pin = 8;
  const uint8_t water_valve_pin = 10;
  const uint8_t system_flow_sensor = 0;
  const uint8_t manifold_drain_valve_pin = 26;
  uint8_t current_step = 0;
  uint32_t next_step_time = 0;
  uint32_t last_update = 0;
  uint16_t tube_ID = 125;
  uint16_t tube_length = 120;
  uint16_t calibration_volume_oz = 128;
  uint16_t calibration_time_sec = 300;
  uint16_t rinse_time_sec = 25;
}system_t;

static system_t system_info;

void system_init(void)
{
  pinMode(system_info.pump_pin, OUTPUT);
  pinMode(system_info.line_drain_valve_pin, OUTPUT);
  pinMode(system_info.water_valve_pin, OUTPUT);
  pinMode(system_info.manifold_drain_valve_pin, OUTPUT);
}

uint8_t system_idle(void)
{
  return !system_info.current_step;
}

void system_start_dose(uint8_t washer, uint8_t detergent)
{
  uint32_t dosage_milli = dosage_time_calc(washer, detergent);
  system_info.next_step_time = millis() + dosage_milli;
  detergent_open_valve(detergent);
  washer_open_valve(washer);
  system_pump(PUMP_ON);
  system_info.current_step = DOSE_STEP;
}

uint32_t dosage_time_calc(uint8_t washer, uint8_t detergent)
{
  //find dosing amount for specific washer detergent combo
  // lbs . oz . milliseconds . 5min
  //       lb       5min        oz
  uint8_t lbs = washer_size(washer);
  uint8_t oz_lb = ounces_per_pound(detergent);
  uint16_t mill_oz = 300000 / ounces_per_5minutes(detergent);
  uint32_t dosage_time_milli = lbs * oz_lb * mill_oz;
  return dosage_time_milli;
}

uint32_t water_flush_time(void)
{
  return millis() + 1000; // #TODO actually calculate time
}

void system_pump(uint8_t state)
{
  digitalWrite(system_info.pump_pin, state);
}

void system_valve(uint8_t valve, uint8_t state)
{
  switch(valve)
  {
    case WATER_VALVE:
      digitalWrite(system_info.water_valve_pin, state);
      break;
    case LINE_DRAIN_VALVE:
      digitalWrite(system_info.line_drain_valve_pin, state);
      break;
    case MANIFOLD_DRAIN_VALVE:
      digitalWrite(system_info.manifold_drain_valve_pin, state);
      break;
    case ALL_VALVES:
      digitalWrite(system_info.water_valve_pin, state);
      digitalWrite(system_info.line_drain_valve_pin, state);
      digitalWrite(system_info.manifold_drain_valve_pin, state);
  }
}

void system_update(void)
{
  if(system_info.current_step && (system_info.last_update + UPDATE_INTERVAL < millis())) //#TODO update last_update variable
  {
    if(system_info.next_step_time < millis())
    {
      system_advance_step();
    }
  }
}

void system_advance_step(void)
{
  switch(system_info.current_step)
  {
    case DOSE_STEP:
      detergent_close_all_valves();
      system_valve(WATER_VALVE, VALVE_OPEN);
      system_info.next_step_time = (millis() + 1000);//#TODO add data and function to caculate flush time
      system_info.current_step = FLUSH_STEP;
      break;
    case FLUSH_STEP:
      system_pump(PUMP_OFF);
      system_valve(MANIFOLD_DRAIN_VALVE, VALVE_OPEN);
      washer_close_all_valves();
      system_info.next_step_time = (millis() + 1000);//#TODO add rinse time to Struct
      system_info.current_step = RINSE_STEP;
      break;
    case RINSE_STEP:
      system_valve(ALL_VALVES, VALVE_CLOSE);
      system_info.current_step = IDLE_STEP;
      delay(1000);
      break;
  }
}
