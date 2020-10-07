#include <Arduino.h>
#include "State.h"
#include "Detergent.h"
#include "Washer.h"
#include "Feedline.h"

static system_t system_info;

uint8_t dosage_oz_calc(uint8_t washer, uint8_t detergent);

bool state_start(uint8_t washer, uint8_t detergent)
{
  bool return_value = false;

  uint8_t dosage_oz = dosage_oz_calc(washer, detergent);

  if(dosage_oz)
  {
    system_info.current_washer = washer;
    system_info.current_detergent = detergent;
    system_info.step_length_millis[DOSE_STEP] = feedline_pump_millis(dosage_oz);
    system_info.step_length_millis[FLUSH_STEP] = feedline_pump_millis(feedline_flush_oz() * 2);
    system_info.step_length_millis[RINSE_STEP] = feedline_pump_millis(feedline_manifold_oz() * 2);
    system_info.current_step = DOSE_STEP;
    system_info.step_start_millis = millis();
    feedline_run_pump(true);
    detergent_open_valve(detergent);
    washer_open_valve(washer);
    return_value = true;
  }
  return return_value;
}

uint8_t state_currentState(void)
{
  return system_info.current_step;
}

void state_advance(void)
{
  system_info.current_step++;

  switch(system_info.current_step)
  {
    case FLUSH_STEP:
      detergent_close_all_valves();
      feedline_valve(WATER_VALVE, VALVE_OPEN);
      system_info.current_step = FLUSH_STEP;
      break;
    case RINSE_STEP:
      washer_close_all_valves();
      if(washer_peek_detergent_in_queue(0) != system_info.current_detergent)
      {
        feedline_valve(LINE_DRAIN_VALVE, VALVE_OPEN);
        system_info.current_step = RINSE_STEP;
        break;
      }
    default:
      feedline_run_pump(false);
      detergent_close_all_valves();
      washer_close_all_valves();
      feedline_valve(ALL_VALVES, VALVE_CLOSE);
      system_info.current_step = IDLE_STEP;
      break;
  }
  system_info.step_start_millis = millis();
}

bool state_isStepComplete(void)
{
  return ((uint32_t)(millis() - system_info.step_start_millis) > system_info.step_length_millis[system_info.current_step]);
}

uint8_t dosage_oz_calc(uint8_t washer, uint8_t detergent)
{
  //find dosing amount for specific washer detergent combo
  // lbs . oz . oz
  //       lb 
  uint8_t lbs = washer_size(washer);
  uint8_t half_oz_ten_lb = detergent_half_oz_per_ten_lbs(detergent);
  uint8_t dosage_half_ozs = ((lbs / 10) * half_oz_ten_lb);
  return dosage_half_ozs;
}
