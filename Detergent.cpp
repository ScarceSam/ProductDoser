#include <Arduino.h>
#include "Detergent.h"

#define NUMBER_OF_DETERGENTS 4

static const uint8_t LEVEL_PIN[] = {29, 11, 34, 33};
static const uint8_t FLOW_PIN[] = {16, 31, 17, 41};
static const uint8_t VALVE_PIN[] = {45, 2, 1, 0};

static detergent_t detergent[NUMBER_OF_DETERGENTS];

void detergent_init(void)
{
  for(int i = 0; i < NUMBER_OF_DETERGENTS; i++)
  {
    //chronologically label the detergents
    detergent[i].number = i+1;

    //assign all the pins from const arrays
    detergent[i].level_pin = LEVEL_PIN[i];
    detergent[i].flow_pin = FLOW_PIN[i];
    detergent[i].valve_pin = VALVE_PIN[i];
  }
}

detergent_t detergetent_get_data(uint8_t detergent_number)
{
  return detergent[detergent_number - 1];
}
