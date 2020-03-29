#include <Arduino.h>
#include "System.h"

#define PUMP_ON HIGH
#define UPDATE_INTERVAL 100

typedef struct {
  const uint8_t pump_pin = 14;
  const uint8_t drain_valve_pin = 0;
  const uint8_t water_valve_pin = 0;
  const uint8_t system_flow_sensor = 0;
  uint8_t pump_running = 0;
  uint32_t pump_start_time = 0;
  uint8_t current_step = 0;
  uint32_t next_step_time = 0;
  uint32_t last_update = 0;
}system_t;

static system_t system_info;

void system_init(void)
{
  pinMode(system_info.pump_pin, OUTPUT);
}

uint8_t system_idle(void)
{
  return !system_info.pump_running;
}

void system_start_dose(washer_t washer, detergent_t detergent)
{
  uint32_t dosage_milli = dosage_time_calc(washer, detergent);
  system_info.pump_start_time = millis();
  system_info.next_step_time = system_info.pump_start_time+ dosage_milli;
  detergent_open_valve(detergent.number);
  washer_open_valve(washer.number);
  system_pump_on();
}

uint32_t dosage_time_calc(washer_t washer, detergent_t detergent)
{
  //find dosing amount for specific washer detergent combo
  // lbs . oz . milliseconds . 5min
  //       lb       5min        oz
  uint8_t lbs = washer.washer_size;
  uint8_t oz_lb = detergent.ounces_per_pound;
  uint16_t mill_oz = 300000 / detergent.ounces_per_5minutes;
  uint32_t dosage_time_milli = lbs * oz_lb * mill_oz;
  return dosage_time_milli;
}

void system_pump_on(void)
{
  digitalWrite(system_info.pump_pin, PUMP_ON);
}

void system_pump_off(void)
{
  digitalWrite(system_info.pump_pin, !PUMP_ON);
}

void system_update(void)
{
  if(system_info.pump_running && (system_info.last_update + UPDATE_INTERVAL < millis()))
  {
    if(system_info.next_step_time < millis())
    {
      system_pump_off();
      detergent_close_all_valves();
    }
  }
}
