#include <Arduino.h>
#include "Washer.h"

washer_t washer[NUMBER_OF_WASHERS];

void washer_init(void)
{
  for(int i = 0; i < NUMBER_OF_WASHERS; i++)
  {
    washer[i].number = i+1;
    washer[i].i2cAddress = I2C_START_ADDRESS + (i/WASHERS_PER_EXPANSION);
    for(int j = 0; j < NUMBER_OF_PINS; j++)
    {
      if(j < (NUMBER_OF_PINS - 1))
      {
        washer[i].com_pin[j] = (((i % NUMBER_OF_PINS) * NUMBER_OF_PINS) + j) % USED_PINS_PER_EXPANSION;
      }
      else
      {
        washer[i].valve_pin = (((i % NUMBER_OF_PINS) * NUMBER_OF_PINS) + j) % USED_PINS_PER_EXPANSION;
      }
    }
  }
}
