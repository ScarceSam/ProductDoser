#include <Arduino.h>
#include "Detergent.h"

const uint8_t LEVEL_PIN[] = {29, 11, 34, 33};
const uint8_t FLOW_PIN[] = {16, 31, 17, 41};
const uint8_t VALVE_PIN[] = {45, 2, 1, 0};

detergent_t g_detergent[NUMBER_OF_DETERGENTS];

void detergent_init(void)
{
  for(int i = 0; i < NUMBER_OF_DETERGENTS; i++)
  {
    //chronologically label the detergents
    g_detergent[i].number = i+1;

    //assign all the pins from const arrays
    g_detergent[i].level_pin = LEVEL_PIN[i];
    g_detergent[i].flow_pin = FLOW_PIN[i];
    g_detergent[i].valve_pin = VALVE_PIN[i];
  }
}
