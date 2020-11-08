#include <Arduino.h>
#include "State.h"
#include "Product.h"
#include "Washer.h"
#include "Feedline.h"

static system_t system_info;

uint8_t dosage_oz_calc(uint8_t washer, uint8_t product);

bool state_start(uint8_t washer, uint8_t product)
{
  bool return_value = false;

  uint8_t dosage_oz = dosage_oz_calc(washer, product);

  if(dosage_oz)
  {
    system_info.current_washer = washer;
    system_info.current_product = product;
    system_info.step_length_millis[IDLE_STEP] = 1;
    system_info.step_length_millis[DOSE_STEP] = feedline_pump_millis(dosage_oz);
    system_info.step_length_millis[FLUSH_STEP] = feedline_flush_millis();
    system_info.step_length_millis[RINSE_STEP] = feedline_flush_millis();;
    system_info.current_step = DOSE_STEP;
    system_info.step_start_millis = millis();
    feedline_run_pump(true);
    product_open_valve(product);
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
      product_close_all_valves();
      feedline_valve(WATER_VALVE, VALVE_OPEN);
      system_info.current_step = FLUSH_STEP;
      break;
    case RINSE_STEP:
      washer_close_all_valves();
      if(system_info.step_length_millis[RINSE_STEP] > 0)
      {
        feedline_valve(LINE_DRAIN_VALVE, VALVE_OPEN);
        system_info.current_step = RINSE_STEP;
        break;
      }
    default:
      feedline_run_pump(false);
      product_close_all_valves();
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

uint8_t dosage_oz_calc(uint8_t washer, uint8_t product)
{
  //find dosing amount for specific washer product combo
  // lbs . oz . oz
  //       lb 
  uint8_t lbs = washer_size(washer);
  uint8_t half_oz_ten_lb = product_half_oz_per_ten_lbs(product);
  uint8_t dosage_half_ozs = ((lbs / 10) * half_oz_ten_lb);
  return dosage_half_ozs;
}

void state_copyStateData(system_t* copyOfData)
{
  *copyOfData = system_info;
}

uint32_t state_remainingMillis(void)
{
  uint32_t return_value = 0;
  for (int i = system_info.current_step; i < 4; i++)
  {
    return_value += system_info.step_length_millis[i];
    if (system_info.current_step == i)
    {
      return_value -= (millis() - system_info.step_start_millis);
    }
  }

  return return_value;
}

void state_checkSkipRinse(uint8_t)
{
  if(washer_peek_product_in_queue(0) == system_info.current_product)
  {
    system_info.step_length_millis[RINSE_STEP] = 0;
  }
  else
  {
    system_info.step_length_millis[RINSE_STEP] = feedline_flush_millis();;
  }
}
