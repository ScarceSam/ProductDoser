#include "Washer.h"
#include "Detergent.h"
#include "Feedline.h"

#define UPDATE_INTERVAL 100

enum steps{ IDLE_STEP, DOSE_STEP, FLUSH_STEP, RINSE_STEP };

typedef struct {
  uint8_t current_step = 0;
  uint8_t current_washer = 0;
  uint8_t current_detergent = 0;
  uint32_t next_step_time = 0;
  uint32_t last_update = 0;
  uint16_t rinse_time_sec = 10;
}system_t;

static system_t system_info;

void start_dosing(uint8_t washer, uint8_t detergent);
uint32_t dosage_time_calc(uint8_t washer, uint8_t detergent);
void advance_step(void);
uint8_t if_idle(void);

void setup()
{
  //Initialize sub systems
  washer_init();
  detergent_init();
  feedline_init();

  //pull saved data from SD card
  //check SD card for data

  //if there is data confirm usage

  //load saved data

}

void loop()
{
  if (washer_ready() && if_idle())
  {
    //dequeue the next washer in queue
    uint8_t next[2] = {0, 0};
    washer_dequeue_next(next);

    uint8_t next_washer = next[0];
    uint8_t next_detergent = next[1];

    //start dosing
    start_dosing(next_washer, next_detergent);
  }else{
    if(system_info.current_step && (system_info.last_update + UPDATE_INTERVAL < millis()))
    {
      if(system_info.next_step_time < millis())
      {
        advance_step();
      }
      system_info.last_update = millis();
    }
  }

  washer_update();
}




uint8_t if_idle(void)
{
  return !system_info.current_step;
}

void start_dosing(uint8_t washer, uint8_t detergent)
{
  uint32_t dosage_milli = dosage_time_calc(washer, detergent);
  system_info.next_step_time = millis() + dosage_milli;
  detergent_open_valve(detergent);
  washer_open_valve(washer);
  feedline_pump(PUMP_ON);
  system_info.current_step = DOSE_STEP;
  system_info.current_washer = washer;
  system_info.current_detergent = detergent;
}

uint32_t dosage_time_calc(uint8_t washer, uint8_t detergent)
{
  //find dosing amount for specific washer detergent combo
  // lbs . oz . milliseconds . 5min
  //       lb       5min        oz
  uint8_t lbs = washer_size(washer);
  uint16_t oz_lb_x100 = ounces_per_pound_x100(detergent);
  uint16_t volume_oz_x100 = (lbs * oz_lb_x100);
  uint32_t cal_time_milli = (calibration_time_sec(detergent) * 1000);
  uint64_t time_volume = (cal_time_milli * volume_oz_x100);
  uint32_t cal_oz_x100 = (calibration_volume_oz(detergent) * 100);
  uint32_t dosage_time_milli = (time_volume/cal_oz_x100);
  return dosage_time_milli;
}

void advance_step(void)
{
  system_info.current_step++;

  switch(system_info.current_step)
  {
    case FLUSH_STEP:
      detergent_close_all_valves();
      feedline_valve(WATER_VALVE, VALVE_OPEN);
      system_info.next_step_time = (millis() + feedline_flush_time_milli());
      system_info.current_step = FLUSH_STEP;
      break;
    case RINSE_STEP:
      feedline_pump(PUMP_OFF);
      washer_close_all_valves();
      if(washer_peek_detergent_in_queue(0) != system_info.current_detergent)
      {
        feedline_valve(MANIFOLD_DRAIN_VALVE, VALVE_OPEN);
        system_info.next_step_time = (millis() + (system_info.rinse_time_sec * 1000));
        system_info.current_step = RINSE_STEP;
        break;
      }
    default:
      feedline_valve(ALL_VALVES, VALVE_CLOSE);
      system_info.current_step = IDLE_STEP;
      break;
  }
}
