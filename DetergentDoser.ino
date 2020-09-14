#include "Washer.h"
#include "Detergent.h"
#include "Feedline.h"
#include "SDcard.h"

enum steps{ IDLE_STEP, DOSE_STEP, FLUSH_STEP, RINSE_STEP };

typedef struct {
  uint8_t current_step = 0;
  uint8_t current_washer = 0;
  uint8_t current_detergent = 0;
  uint32_t step_length_millis = 0;
  uint32_t step_start_millis = 0;
}system_t;

static system_t system_info;

uint32_t start_dosing(uint8_t washer, uint8_t detergent); //TODO: combine with advance step
uint8_t dosage_oz_calc(uint8_t washer, uint8_t detergent); //TODO: move to Product file
uint32_t advance_step(void);
uint8_t if_idle(void);

void setup()
{
  //Initialize sub systems
  washer_init();
  detergent_init();
  feedline_init();
  if(!sdcard_init())
  {
    while(true)
    {
      ;
    }
  }


  if(!feedline_load())
  {
    //halt and alarm
  }
  washer_load();
  detergent_load();

  //if there is data confirm usage

  //load saved data
  feedline_flush();

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

    system_info.current_washer = washer;
    system_info.current_detergent = detergent;

    //start dosing
    system_info.step_length_millis = start_dosing(washer, detergent);
    system_info.step_start_millis = millis();
  }
  else if((!feedline_is_pumping()) && ((uint32_t)(millis() - system_info.step_start_millis) > system_info.step_length_millis))//TODO: is pumping in detergent.h
  {
    system_info.step_length_millis = advance_step();
    system_info.step_start_millis = millis();
  }

  washer_update();
  feedline_update();
  //TODO: detergent_update();
}

uint8_t if_idle(void)
{
  return !system_info.current_step;
}

uint32_t start_dosing(uint8_t washer, uint8_t detergent)
{
  uint8_t dosage_oz = dosage_oz_calc(washer, detergent); //TODO: handle millis time & label for time
  uint32_t return_value = 0;

  if(0 == dosage_oz)
  {
    //TODO: Error handle
    return_value = 0;
  }
  else
  {
    detergent_open_valve(detergent); //TODO:delete
    washer_open_valve(washer);
    system_info.current_step = DOSE_STEP;
    return_value = feedline_pump_start(dosage_oz); //TODO: Product dispense(product time)
  }

  return return_value;
}

uint8_t dosage_oz_calc(uint8_t washer, uint8_t detergent)
{
  //TODO: return dispensing time(millis)
  //find dosing amount for specific washer detergent combo
  // lbs . oz(tenths) . time(seconds) . time(millis) 
  //          lb          oz(int) 
  uint8_t lbs = washer_size(washer);
  uint8_t half_oz_ten_lb = detergent_half_oz_per_ten_lbs(detergent);
  uint8_t dosage_half_ozs = ((lbs / 10) * half_oz_ten_lb);
  return dosage_half_ozs;
}

uint32_t advance_step(void)
{
  system_info.current_step++;
  uint32_t step_length_millis = 0;

  switch(system_info.current_step)
  {
    case FLUSH_STEP:
      detergent_close_all_valves();//TODO: delete
      feedline_valve(WATER_VALVE, VALVE_OPEN); 
      step_length_millis = feedline_pump_start(feedline_flush_oz() * 2); //TODO: change to flush time
      system_info.current_step = FLUSH_STEP;
      break;
    case RINSE_STEP:
      washer_close_all_valves();
      if(washer_peek_detergent_in_queue(0) != system_info.current_detergent)
      {
        feedline_valve(LINE_DRAIN_VALVE, VALVE_OPEN);
        step_length_millis = feedline_pump_start(feedline_manifold_oz() * 2); //TODO: change to flush time
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
