#include <Arduino.h>
#include "Feedline.h"

#define UPDATE_INTERVAL 100

enum valves{ LINE_DRAIN_VALVE, WATER_VALVE, MANIFOLD_DRAIN_VALVE, ALL_VALVES };
enum steps{ IDLE_STEP, DOSE_STEP, FLUSH_STEP, RINSE_STEP };

typedef struct {
  const uint8_t pump_pin = 14;
  const uint8_t line_drain_valve_pin = 8;
  const uint8_t water_valve_pin = 10;
  const uint8_t feedline_flow_sensor = 0;
  const uint8_t manifold_drain_valve_pin = 26;
  uint8_t current_step = 0;
  uint8_t current_washer = 0;
  uint8_t current_detergent = 0;
  uint32_t next_step_time = 0;
  uint32_t last_update = 0;
  uint32_t flush_oz_x1000 = 2000;
  uint16_t calibration_volume_oz = 87;
  uint16_t calibration_time_sec = 300;
  uint16_t rinse_time_sec = 10;
}feedline_t;

static feedline_t feedline_info;

void feedline_init(void)
{
  pinMode(feedline_info.pump_pin, OUTPUT);
  pinMode(feedline_info.line_drain_valve_pin, OUTPUT);
  pinMode(feedline_info.water_valve_pin, OUTPUT);
  pinMode(feedline_info.manifold_drain_valve_pin, OUTPUT);
}

uint8_t feedline_idle(void)
{
  return !feedline_info.current_step;
}

void feedline_start_dose(uint8_t washer, uint8_t detergent)
{
  uint32_t dosage_milli = dosage_time_calc(washer, detergent);
  feedline_info.next_step_time = millis() + dosage_milli;
  detergent_open_valve(detergent);
  washer_open_valve(washer);
  feedline_pump(PUMP_ON);
  feedline_info.current_step = DOSE_STEP;
  feedline_info.current_washer = washer;
  feedline_info.current_detergent = detergent;
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

uint32_t water_flush_time_milli(void)
{
  uint32_t volume_oz_x1000 = (feedline_info.flush_oz_x1000);
  uint32_t cal_time_milli = (feedline_info.calibration_time_sec * 1000);
  uint64_t time_volume = (cal_time_milli * volume_oz_x1000);
  uint32_t cal_oz_x1000 = (feedline_info.calibration_volume_oz * 1000);
  uint32_t rinse_time_milli = (time_volume / cal_oz_x1000);
  return rinse_time_milli;
}

void feedline_pump(uint8_t state)
{
  digitalWrite(feedline_info.pump_pin, state);
}

void feedline_valve(uint8_t valve, uint8_t state)
{
  switch(valve)
  {
    case WATER_VALVE:
      digitalWrite(feedline_info.water_valve_pin, state);
      break;
    case LINE_DRAIN_VALVE:
      digitalWrite(feedline_info.line_drain_valve_pin, state);
      break;
    case MANIFOLD_DRAIN_VALVE:
      digitalWrite(feedline_info.manifold_drain_valve_pin, state);
      break;
    case ALL_VALVES:
      digitalWrite(feedline_info.water_valve_pin, state);
      digitalWrite(feedline_info.line_drain_valve_pin, state);
      digitalWrite(feedline_info.manifold_drain_valve_pin, state);
  }
}

void feedline_update(void)
{
  if(feedline_info.current_step && (feedline_info.last_update + UPDATE_INTERVAL < millis())) //#TODO update last_update variable
  {
    if(feedline_info.next_step_time < millis())
    {
      feedline_advance_step();
    }
  }
}

void feedline_advance_step(void)
{
  feedline_info.current_step++;

  switch(feedline_info.current_step)
  {
    case FLUSH_STEP:
      detergent_close_all_valves();
      feedline_valve(WATER_VALVE, VALVE_OPEN);
      feedline_info.next_step_time = (millis() + water_flush_time_milli());
      feedline_info.current_step = FLUSH_STEP;
      break;
    case RINSE_STEP:
      feedline_pump(PUMP_OFF);
      washer_close_all_valves();
      if(washer_peek_detergent_in_queue(0) != feedline_info.current_detergent)
      {
        feedline_valve(MANIFOLD_DRAIN_VALVE, VALVE_OPEN);
        feedline_info.next_step_time = (millis() + (feedline_info.rinse_time_sec * 1000));
        feedline_info.current_step = RINSE_STEP;
        break;
      }
    case IDLE_STEP:
      feedline_valve(ALL_VALVES, VALVE_CLOSE);
      feedline_info.current_step = IDLE_STEP;
      //delay(1000);
      break;
  }
}
