#include <Arduino.h>
#include "State.h"
#include "Detergent.h"
#include "Washer.h"
#include "Feedline.h"

uint32_t start_dosing(uint8_t washer, uint8_t detergent)
{
  uint8_t dosage_oz = dosage_oz_calc(washer, detergent);
  uint32_t return_value = 0;

  if(0 == dosage_oz)
  {
    //TODO: Error handle
    return_value = 0;
  }
  else
  {
    detergent_open_valve(detergent);
    washer_open_valve(washer);
    system_info.current_step = DOSE_STEP;
    return_value = feedline_pump_start(dosage_oz);
  }

  return return_value;
}

uint8_t if_idle(void)
{
  return !system_info.current_step;
}

uint32_t advance_step(void)
{
  system_info.current_step++;
  uint32_t step_length_millis = 0;

  switch(system_info.current_step)
  {
    case FLUSH_STEP:
      detergent_close_all_valves();
      feedline_valve(WATER_VALVE, VALVE_OPEN);
      step_length_millis = feedline_pump_start(feedline_flush_oz() * 2);
      system_info.current_step = FLUSH_STEP;
      break;
    case RINSE_STEP:
      washer_close_all_valves();
      if(washer_peek_detergent_in_queue(0) != system_info.current_detergent)
      {
        feedline_valve(LINE_DRAIN_VALVE, VALVE_OPEN);
        step_length_millis = feedline_pump_start(feedline_manifold_oz() * 2);
        system_info.current_step = RINSE_STEP;
        break;
      }
    default:
      detergent_close_all_valves();
      washer_close_all_valves();
      feedline_valve(ALL_VALVES, VALVE_CLOSE);
      system_info.current_step = IDLE_STEP;
      break;
  }
  return step_length_millis;
}
