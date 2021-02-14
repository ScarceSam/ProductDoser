#pragma once


void clear_char_array(char* string, uint8_t max_len);

void copy_char_array(char* copy_array, const char* master_array, uint8_t max_len);

void char_append_digits(char* string, uint8_t number, uint8_t max_len);

void char_concatenate(char* output, const char* input1, const char* input2, uint8_t sizeLimit);

uint8_t char_charLen(const char* string);
