#include "Washer.h"
#include "Detergent.h"
#include "Feedline.h"
#include "SDcard.h"
#include "FlowSensor.h"
#include "Model.h"
#include "View.h"
#include "State.h"

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
  if (washer_enqueued() && state_ifIdle())
  {
    //dequeue the next washer in queue
    uint8_t next[2] = {0, 0};
    washer_dequeue_next(next);

    uint8_t washer = next[0];
    uint8_t detergent = next[1];

    system_info.current_washer = washer;//TODO: move to state file
    system_info.current_detergent = detergent;//TODO: move to state file

    //start dosing
    system_info.step_length_millis = state_start(washer, detergent);
    system_info.step_start_millis = millis();
  }
  else if((!feedline_is_pumping()) && ((uint32_t)(millis() - system_info.step_start_millis) > system_info.step_length_millis))
  {
    system_info.step_length_millis = state_advance();
    system_info.step_start_millis = millis();
  }

  washer_pollWashers();
  feedline_update();
}
