#include "Washer.h"
#include "Product.h"
#include "Feedline.h"
#include "SDcard.h"
#include "FlowSensor.h"
#include "Model.h"
#include "View.h"
#include "State.h"
#include "Controller.h"
#include "Buttons.h"
#include "Menutree.h"

#define LED_PIN 13
void toggle_led(void);

void setup()
{
  //Initialize sub systems
  view_println("Initializing..");
  view_init();
  washer_init();
  feedline_init();
  buttons_init();

  view_println("SD Card");
  uint8_t temp_result = sdcard_init();
  if (temp_result == 0)
  {
    view_println("-------ERROR--------");
    while(1);
  }
  else if (temp_result == 2)
  {
    view_println("   Settings file   ");
    view_println(" added to SD card. ");
    view_println("   Populate file   ");
    view_println("-------HALT--------");
    while(1);
  }

  view_println("Load Setup");
  if(!feedline_load())
  {
    view_println("--Feedline Defaults--");
    delay(2000);
  }

  view_println("Load Washers");
  washer_load();

  product_init();
  view_println("Load Products");
  if(!product_load())
  {
    view_println("-------ERROR--------");
    while(1);
  }

  if(!flowsensor_init())
  {
    view_println("Flow Sensor - OFF");
  }

  view_println("System Flush");
  feedline_flush();

  view_clear();
  view_println("System loaded");

  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  if(state_current_state() == PAUSE_STEP)
  {
    //state_advance();
  }
  else if(washer_enqueued() && (state_current_state() == IDLE_STEP))
  {
    //dequeue the next washer in queue
    uint8_t next_job[2] = {0, 0};
    washer_dequeue_next(next_job);

    uint8_t washer = next_job[0];
    uint8_t product = next_job[1];

    //start dosing
    state_start(washer, product);
  }
  else if(state_current_state() != IDLE_STEP)
  {
    state_check_skip_rinse(washer_peek_product_in_queue(0));

    if(state_is_step_complete())
    {
      state_advance();
    }
  }

  controller_update_screen();
  washer_pollWashers();
  toggle_led();
}

void toggle_led(void)
{
  static bool b_led_on = false;
  b_led_on = !b_led_on;
  digitalWrite(LED_PIN, b_led_on);
}
