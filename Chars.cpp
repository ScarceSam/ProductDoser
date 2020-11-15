#include <Arduino.h>

void clear_char_array(char* string, uint8_t max_len)
{
  for(int i = 0; i < max_len; i++)
  {
    string[i] = '\0';
  }
  string[max_len - 1] = '\0';
}

void copy_char_array(char* copy_array, char* master_array, uint8_t max_len)
{
  for(int i = 0; i < max_len; i++)
  {
    copy_array[i] = master_array[i];
    if(copy_array[i] == '\0')
      break;
  }
  copy_array[max_len - 1] = '\0';
}

void char_append_digits(char* string, uint8_t number, uint8_t max_len)
{
  
}
