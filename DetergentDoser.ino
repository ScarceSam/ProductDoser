#include "Washer.h"
#include "Detergent.h"
#include "Feedline.h"
#include "SDcard.h"
#include "FlowSensor.h"
#include "Model.h"
#include "View.h"

enum steps{ IDLE_STEP, DOSE_STEP, FLUSH_STEP, RINSE_STEP }; //TODO: move to states file

typedef struct {
  uint8_t current_step = 0;
  uint8_t current_washer = 0;
  uint8_t current_detergent = 0;
  uint32_t step_length_millis = 0;
  uint32_t step_start_millis = 0;
}system_t;//TODO: move to states file

static system_t system_info;//TODO: move to states file

uint32_t start_dosing(uint8_t washer, uint8_t detergent);
uint8_t dosage_oz_calc(uint8_t washer, uint8_t detergent);
uint32_t advance_step(void);//TODO: move to states file
uint8_t if_idle(void);//TODO: move to states file

void setup()
{
  //Initialize sub systems
  view_init();
  washer_init();
  detergent_init();
  feedline_init();
  flowsensor_init();

  view_println("SD Card");
  if (!sdcard_init())
  {
    view_println("-------ERROR--------");
    while(1);
  }

  view_println("Load Setup");
  if(!feedline_load())
  {
    //halt and alarm
  }

  view_println("Load Washers");
  washer_load();

  view_println("Load Products");
  detergent_load();

  view_println("System Flush");
  feedline_flush();

  view_clear();
  view_println("System Ready");
}

void loop()
{
  if (washer_ready() && if_idle())
  {
    //dequeue the next washer in queue
    uint8_t next[2] = {0, 0};
    washer_dequeue_next(next);

    uint8_t washer = next[0];
    uint8_t detergent = next[1];

    system_info.current_washer = washer;//TODO: move to states file
    system_info.current_detergent = detergent;//TODO: move to states file

    //start dosing
    system_info.step_length_millis = start_dosing(washer, detergent);
    system_info.step_start_millis = millis();
  }
  else if((!feedline_is_pumping()) && ((uint32_t)(millis() - system_info.step_start_millis) > system_info.step_length_millis))
  {
    system_info.step_length_millis = advance_step();
    system_info.step_start_millis = millis();
  }

  washer_update();
  feedline_update();
}

uint8_t if_idle(void)//TODO: move to states file
{
  return !system_info.current_step;
}

uint32_t start_dosing(uint8_t washer, uint8_t detergent) //TODO: move to states file
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

uint8_t dosage_oz_calc(uint8_t washer, uint8_t detergent)//TODO: move to detergent file
{
  //find dosing amount for specific washer detergent combo
  // lbs . oz . oz
  //       lb 
  uint8_t lbs = washer_size(washer);
  uint8_t half_oz_ten_lb = detergent_half_oz_per_ten_lbs(detergent);
  uint8_t dosage_half_ozs = ((lbs / 10) * half_oz_ten_lb);
  return dosage_half_ozs;
}

uint32_t advance_step(void)//TODO: move to states file
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
