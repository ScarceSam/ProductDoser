#include "Washer.h"
#include "Detergent.h"
#include "Feedline.h"
#include "SDcard.h"
#include "FlowSensor.h"
#include "Model.h"
#include "View.h"
#include "State.h"

#define LED_PIN 13
void toggleLED(void);

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

  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  if (washer_enqueued() && (state_currentState() == IDLE_STEP))
  {
    //dequeue the next washer in queue
    uint8_t next[2] = {0, 0};
    washer_dequeue_next(next);

    uint8_t washer = next[0];
    uint8_t detergent = next[1];

    //start dosing
    state_start(washer, detergent);
    view_display_state();
  }
  else if((state_currentState() != IDLE_STEP) && state_isStepComplete())
  {
    state_advance();
    view_display_state();
  }
  else if(state_currentState() == IDLE_STEP)
  {
    view_clear();
  }

  washer_pollWashers();
  toggleLED();
}

void toggleLED(void)
{
  static bool indicatorLED = false;
  indicatorLED = !indicatorLED;
  digitalWrite(LED_PIN, indicatorLED);
}
