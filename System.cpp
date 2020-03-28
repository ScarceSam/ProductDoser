#include <Arduino.h>
#include "System.h"


typedef struct {
  const uint8_t pump_pin = 0;
  const uint8_t drain_valve_pin = 0;
  const uint8_t water_valve_pin = 0;
  const uint8_t system_flow_sensor = 0;
  uint8_t pump_running = 0;
  uint32_t pump_start_time = 0;
}system_t;

static system_t system_info;

uint8_t system_idle(void)
{
  return !system_info.pump_running;
}

void system_dose(washer_t washer, detergent_t detergent)
{
  uint32_t dosage_milli = dosage_time_calc(washer, detergent);
  
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
