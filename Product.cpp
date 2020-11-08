#include <Arduino.h>
#include "Product.h"
#include "Washer.h"
#include "Pinmap.h"
#include "SDcard.h"

#define NUMBER_OF_PRODUCTS 4
#define VALVE_OPEN HIGH

static const uint8_t LEVEL_PIN[] = {PRODUCT_1_LEVEL_PIN, PRODUCT_2_LEVEL_PIN,
                                    PRODUCT_3_LEVEL_PIN, PRODUCT_4_LEVEL_PIN};
static const uint8_t PUMP_PIN[] = {PRODUCT_1_PUMP_PIN, PRODUCT_2_PUMP_PIN,
                                    PRODUCT_3_PUMP_PIN, PRODUCT_4_PUMP_PIN};

typedef struct{
  uint8_t number;
  uint8_t level_pin;
  uint8_t pump_pin;
  uint16_t half_oz_per_ten_lbs = 0;
  char product_name[ID_LIMIT];
} product_t;

static product_t product[NUMBER_OF_PRODUCTS];

void product_init(void)
{
  for(int i = 0; i < NUMBER_OF_PRODUCTS; i++)
  {
    //chronologically label the products
    product[i].number = i+1;

    //assign all the pins from const arrays
    product[i].level_pin = LEVEL_PIN[i];
    product[i].pump_pin = PUMP_PIN[i];
    pinMode(PUMP_PIN[i], OUTPUT);
    digitalWrite(PUMP_PIN[i], !VALVE_OPEN);
  }
}

void product_pumpOn(uint8_t product_number)
{
  digitalWrite(product[product_number - 1].pump_pin, VALVE_OPEN);
}

void product_allPumpsOff(void)
{
  for(int i = 0; i < NUMBER_OF_PRODUCTS; i++)
  {
    digitalWrite(product[i].pump_pin, !VALVE_OPEN);
  }
}

uint8_t product_half_oz_per_ten_lbs(uint8_t product_number)
{
  return product[product_number -1].half_oz_per_ten_lbs;
}

uint8_t product_load(void)
{
  uint8_t products_loaded = 0;

  for (uint8_t i = 0; i < NUMBER_OF_PRODUCTS; i++)
  {
    String product_name = "product";
    int product_number = i + 1;
    product_name += product_number;

    int32_t product_dosage = SDcard_read_int(product_name, "dosage");
    if(product_dosage > 0)
    {
      product[i].half_oz_per_ten_lbs = product_dosage;
      products_loaded++;
    }

    String product_label = SDcard_read_string(product_name, "label");

    for (int j = 0; j < ID_LIMIT; j++)
    {
      if ((product_label[j] == '\0') || (j == (ID_LIMIT - 1)))
      {
        product[i].product_name[j] = '\0';
        j = ID_LIMIT;
      }
      else
      {
        product[i].product_name[j] = product_label[j];
      }
    }
  }

  return products_loaded;
}

char* product_label(uint8_t washerNumber)
{
  return product[washerNumber - 1].product_name;
}
