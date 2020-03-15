#include <Arduino.h>
#include "Washer.h"

void washer_init(void)
{
  for(int i; i < NUMBER_OF_WASHERS; i++)
  {
    washer[i].number = i+1;
  }
}
