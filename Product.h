#pragma once

#define ID_LIMIT 16

void product_init(void);

void product_pumpOn(uint8_t product_number);

void product_allPumpsOff(void);

uint8_t product_half_oz_per_ten_lbs(uint8_t product_number);

uint8_t product_load(void);

char* product_label(uint8_t);
