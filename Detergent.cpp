#include <Arduino.h>
#include "Detergent.h"

#define NUMBER_OF_DETERGENTS 4
#define VALVE_OPEN HIGH

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
    pinMode(VALVE_PIN[i], OUTPUT);
    digitalWrite(VALVE_PIN[i], !VALVE_OPEN);
  }
}

detergent_t detergetent_get_data(uint8_t detergent_number)
{
  return detergent[detergent_number - 1];
}

void detergent_open_valve(uint8_t detergent_number)
{
  digitalWrite(detergent[detergent_number - 1].valve_pin, VALVE_OPEN);
}

void detergent_close_all_valves(void)
{
  for(int i = 0; i < NUMBER_OF_DETERGENTS; i++)
  {
    digitalWrite(detergent[i].valve_pin, !VALVE_OPEN);
  }
}

uint16_t ounces_per_pound_x100(uint8_t detergent_number)
{
  return detergent[detergent_number -1].ounces_per_pound_x100;
}

uint16_t calibration_volume_oz(uint8_t detergent_number)
{
  return detergent[detergent_number - 1].calibration_volume_oz;
}

uint16_t calibration_time_sec(uint8_t detergent_number)
{
  return detergent[detergent_number - 1].calibration_time_sec;
}
