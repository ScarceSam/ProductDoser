#include "Washer.h"
#include "Detergent.h"
#include "System.h"

void setup()
{
  //Initialize sub systems
  washer_init();
  detergent_init();
  system_init();

  //pull saved data from SD card
  //check SD card for data

  //if there is data confirm usage

  //load saved data

}

void loop()
{
  if (washer_ready() && system_idle())
  {
    //pull the next washer out of queue
    uint8_t next[2] = {0, 0};
    washer_get_next(next);

    //pull washer and detergent structs for passing
    uint8_t next_washer = next[0];// #TODO implement queue and remove test code
    uint8_t next_detergent = next[1];// #TODO implement queue and remove test code

    //start dosing
    system_start_dose(next_washer, next_detergent);
  }


//  washer_update();
//  detergent_update();
  system_update();
//  button_update();
//  interface_update();

//  delay(1000000);
}
