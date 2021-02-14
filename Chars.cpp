#include <Arduino.h>
#include "Chars.h"

void clear_char_array(char* string, uint8_t max_len)
{
  for(int i = 0; i < max_len; i++)
  {
    string[i] = '\0';
  }
  string[max_len - 1] = '\0';
}

void copy_char_array(char* copy_array, const char* master_array, uint8_t max_len)
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
  int string_end = 0;
  while (string_end < max_len)
  {
    if(string[string_end] == '\0')
    {
      break;
    }
    else
    {
      string_end++;
    }
  }

  char char_number = '0';
  if(number >= 10 && number < 101)
  {
    char char_number = '0';
    char_number += (number / 10);
    string[string_end] = char_number;
    char_number = '0';
    char_number += (number % 10);
    string[string_end + 1] = char_number;
    string[string_end + 2] = '\0';
  }
  else if(number < 10)
  {
    char_number = '0';
    char_number += (number % 10);
    string[string_end] = char_number;
    string[string_end + 1] = '\0';
  }
}

void char_concatenate(char* output, const char* input1, const char* input2, uint8_t sizeLimit)
{
  uint8_t endLoop = sizeLimit;
  uint8_t strOneLen = char_charLen(input1);

  for (int i = 0; i < endLoop; i++)
  {
    //if iteration is less than end of string one
    if (i < strOneLen)
    {
      //copy charater from 1 to returnstring
      output[i] = input1[i];
    }
    //if past string one end of line
    else if (i >= strOneLen)
    {
      //copy character form str2 to returnstring
      output[i] = input2[i - strOneLen];
    }

    //if last iteration
    if (i == (endLoop - 1))
    {
      //end line
      output[i] = '\0';
    }

    //if return string EOL
    if (output[i] == '\0')
    {
      //finish iteration
      i = endLoop;
    }
  }
}

uint8_t char_charLen(const char* string)
{
  uint8_t return_value = 0;
  for (int i = 0; i < (20 + 1);i++)
  {
    if( string[i] == '\0')
    {
      return_value = i;
      break;
    }
  }
  return return_value;
}
