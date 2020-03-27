#include <Arduino.h>
#include "System.h"


struct system_info{
  static const uint8_t pump_pin = 0;
  static const uint8_t drain_valve_pin = 0;
  static const uint8_t water_valve_pin = 0;
  static const uint8_t system_flow_sensor = 0;
  static uint8_t pump_running = 0;
  static uint32_t pump_start_time = 0;
}system_t;

static system_t system_info;

uint8_t system_idle(void)
{
  return !pump_running;
}

void system_dose(washer_t washer, detergent_t detergent)
{
  
}
