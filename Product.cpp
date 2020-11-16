#include <Arduino.h>
#include "Product.h"
#include "Washer.h"
#include "Pinmap.h"
#include "SDcard.h"
#include "Chars.h"

#define NUMBER_OF_PRODUCTS 4
#define PUMP_ON HIGH

static const uint8_t LEVEL_PIN[] = {PRODUCT_1_LEVEL_PIN, PRODUCT_2_LEVEL_PIN,
                                    PRODUCT_3_LEVEL_PIN, PRODUCT_4_LEVEL_PIN};
static const uint8_t PUMP_PIN[] = {PRODUCT_1_PUMP_PIN, PRODUCT_2_PUMP_PIN,
                                    PRODUCT_3_PUMP_PIN, PRODUCT_4_PUMP_PIN};

typedef struct{
  uint8_t number;
  uint8_t level_pin;
  uint8_t pump_pin;
  uint16_t cal_oz_per_min = 40;
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
    digitalWrite(PUMP_PIN[i], !PUMP_ON);
  }
}

void product_pump_on(uint8_t product_number)
{
  digitalWrite(product[product_number - 1].pump_pin, PUMP_ON);
}

void product_all_pumps_off(void)
{
  for(int i = 0; i < NUMBER_OF_PRODUCTS; i++)
  {
    digitalWrite(product[i].pump_pin, !PUMP_ON);
  }
}

uint8_t product_half_oz_per_ten_lbs(uint8_t product_number)
{
  return product[product_number -1].half_oz_per_ten_lbs;
}

uint8_t product_load(void)
{
  uint8_t products_loaded = 0;
  char product_name[10];
  clear_char_array(product_name, 10);
  const char* product_word = "product";
  copy_char_array(product_name, product_word, 8);

  for (int i = 0; i < NUMBER_OF_PRODUCTS; i++)
  {
    product_name[7] = '\0';
    char_append_digits(product_name, (i + 1), 10);

    uint8_t product_dosage = 0;
    SDcard_read_int(product_name, "dosage", &product_dosage);
    if(product_dosage > 0)
    {
      product[i].half_oz_per_ten_lbs = product_dosage;
      products_loaded++;
    }

    char saved_name[ID_LIMIT];
    clear_char_array(saved_name, 10);
    SDcard_read_string(product_name,  "label", saved_name, ID_LIMIT);

    if(saved_name[0] != '\0')
    {
      copy_char_array(product[i].product_name, saved_name, ID_LIMIT);
    }
  }
  return products_loaded;
}

char* product_label(uint8_t product_number)
{
  return product[product_number - 1].product_name;
}

uint32_t product_pump_millis(uint8_t product_number, uint8_t volume_oz)
{
  uint16_t oz_per_min = product[product_number - 1].cal_oz_per_min;

  uint32_t millis_per_oz = ( 60000 / oz_per_min );

  return ((millis_per_oz * volume_oz)/2);
}
