#include "Washer.h"
#include "Detergent.h"
#include "Feedline.h"

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
  if (washer_ready() && feedline_idle())
  {
    //dequeue the next washer in queue
    uint8_t next[2] = {0, 0};
    washer_dequeue_next(next);

    uint8_t next_washer = next[0];
    uint8_t next_detergent = next[1];

    //start dosing
    feedline_start_dose(next_washer, next_detergent);
  }


  washer_update();
//  detergent_update();
  feedline_update();
//  button_update();
//  interface_update();

}
