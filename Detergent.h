#pragma once

#define ID_LIMIT 16

void product_init(void);

void product_open_valve(uint8_t product_number);

void product_close_all_valves(void);

uint8_t product_half_oz_per_ten_lbs(uint8_t product_number);

uint8_t product_load(void);

char* product_label(uint8_t);
