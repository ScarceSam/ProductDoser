#include "Washer.h"
#include "Detergent.h"
#include "System.h"

void setup()
{
  //Initialize sub systems
  washer_init();
  detergent_init();

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
    washer_t next_washer = washer_get_data(next[0]);
    detergent_t next_detergent = detergetent_get_data(next[1]);

    //start dosing
    system_dose(next_washer, next_detergent);
  }


//  washer_update();
//  system_update();
//  button_update();
//  interface_update();

//  delay(1000000);
}
