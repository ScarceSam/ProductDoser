#include <Arduino.h>
#include "Detergent.h"
#include "Washer.h"
#include "Pinmap.h"
#include "SDcard.h"

#define NUMBER_OF_DETERGENTS 4
#define VALVE_OPEN HIGH

static const uint8_t LEVEL_PIN[] = {DETERGENT_1_LEVEL_PIN, DETERGENT_2_LEVEL_PIN,
                                    DETERGENT_3_LEVEL_PIN, DETERGENT_4_LEVEL_PIN};
static const uint8_t VALVE_PIN[] = {DETERGENT_1_VALVE_PIN, DETERGENT_2_VALVE_PIN,
                                    DETERGENT_3_VALVE_PIN, DETERGENT_4_VALVE_PIN};

typedef struct{
  uint8_t number;
  uint8_t level_pin;
  uint8_t valve_pin;
  uint16_t half_oz_per_ten_lbs = 0;
  char detergent_name[ID_LIMIT];
} detergent_t;

static detergent_t detergent[NUMBER_OF_DETERGENTS];

void detergent_init(void)
{
  for(int i = 0; i < NUMBER_OF_DETERGENTS; i++)
  {
    //chronologically label the detergents
    detergent[i].number = i+1;

    //assign all the pins from const arrays
    detergent[i].level_pin = LEVEL_PIN[i];
    detergent[i].valve_pin = VALVE_PIN[i];
    pinMode(VALVE_PIN[i], OUTPUT);
    digitalWrite(VALVE_PIN[i], !VALVE_OPEN);
  }
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

uint8_t detergent_half_oz_per_ten_lbs(uint8_t detergent_number)
{
  return detergent[detergent_number -1].half_oz_per_ten_lbs;
}

uint8_t detergent_load(void)
{
  uint8_t detergents_loaded = 0;

  for (uint8_t i = 0; i < NUMBER_OF_DETERGENTS; i++)
  {
    String detergent_name = "product";
    int detergent_number = i + 1;
    detergent_name += detergent_number;

    int32_t detergent_dosage = SDcard_read_int(detergent_name, "dosage");
    if(detergent_dosage > 0)
    {
      detergent[i].half_oz_per_ten_lbs = detergent_dosage;
      detergents_loaded++;
    }
  }

  return detergents_loaded;
}

uint8_t dosage_oz_calc(uint8_t washer, uint8_t detergent)
{
  //find dosing amount for specific washer detergent combo
  // lbs . oz . oz
  //       lb 
  uint8_t lbs = washer_size(washer);
  uint8_t half_oz_ten_lb = detergent_half_oz_per_ten_lbs(detergent);
  uint8_t dosage_half_ozs = ((lbs / 10) * half_oz_ten_lb);
  return dosage_half_ozs;
}
