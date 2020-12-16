#include <Arduino.h>
#include "SDcard.h"
#include <SD.h>
#include <SPI.h>
#include "Chars.h"

#define MAX_LEN 60

static bool overwrite_settings = false;
static File saveFile;

uint32_t find_device_info(const char* device);
uint32_t find_setting_info(uint32_t start, const char* setting);
void remove_characters(char phrase[]);
uint8_t fetch_setting(uint32_t start, char* string);
void clean_setting(char* string);
void create_settings_file(void);

uint8_t sdcard_init(void)
{
  uint8_t return_value = 0;

  if (SD.begin(BUILTIN_SDCARD)) {
    return_value = 1;
  }

  if (return_value == 1)
  {
    if(overwrite_settings)
    {
      SD.remove("settings.txt");
    }

    if(!SD.exists("settings.txt"))
    {
      create_settings_file();
      return_value = 2;
    }
  }

  return return_value;
}

uint32_t find_device_info(const char* device)
{
  //open file
  saveFile = SD.open("settings.txt", FILE_READ);
  char line[MAX_LEN];

  //find device line
  bool device_found = 0;
  bool end_of_file = 0;
  while(device_found == 0 && saveFile.available())
  {
    for( int i = 0; i < MAX_LEN; i++)
    {
      
      if(saveFile.available())
      {
        line[i] = saveFile.read();
        if(line[i] >= 'A' && line[i] <= 'Z')
        {
          line[i] += ('a' - 'A');
        }
      }else{
        line[i] = '\n';
        end_of_file = 1;
      }
      
      if( line[i] == '\n')
      {
        i = MAX_LEN;
      }
    }

    remove_characters(line);

    bool check_done = 0;
    uint8_t number = 0;
    while(!check_done)
    {
      if(device[number] != line[number])
      {
        check_done = 1;
        if(device[number] == '\0' && line[number] == '\n')
          device_found = 1;
      }else if(device[number] == '\0' || line[number] == '\n'){
        check_done = 1;
      }
      number++;
    }
  }

  uint32_t return_value = saveFile.position();
  
  saveFile.close();

  if(end_of_file)
    return_value = 0;

  return return_value;
}

uint32_t find_setting_info(uint32_t start, const char* setting)
{
  saveFile = SD.open("settings.txt", FILE_READ);
  char line[MAX_LEN];
  saveFile.seek(start);

  int device_found = 0;
  bool end_of_file = 0;
  while(!device_found && saveFile.available())
  {
    for(int i = 0; i < MAX_LEN; i++)
    {
      if(saveFile.available())
      {
        line[i] = saveFile.read();
        if(line[i] >= 'A' && line[i] <= 'Z')
        {
          line[i] += ('a' - 'A');
        }
      }
      else
      {
        line[i] = '\n';
        end_of_file = 1;
      }
      
      if( line[i] == '\n' || line[i] == '=')
      {
        break;
      }
      else if(line[i] == ':' || line[i] == ';')
      {
        device_found = -1;
        break;
      }
    }

    remove_characters(line);

    bool check_done = 0;
    uint8_t number = 0;
    while(!check_done)
    {
      if(setting[number] != line[number])
      {
        check_done = 1;
        if(setting[number] == '\0' && line[number] == '=')
          device_found = 1;
      }else if(setting[number] == '\0' || line[number] == '\n'){
        check_done = 1;
      }
      number++;
    }
  }

  uint32_t return_value = saveFile.position();
  
  saveFile.close();

  if(end_of_file || (device_found <= 0))
    return_value = 0;

  return return_value;
}

void remove_characters(char phrase[])
{
  int out_cursor = 0;
  for(int i = 0; i < MAX_LEN; i++)
  {
    if((phrase[i] >= 'a' && phrase[i] <= 'z') || (phrase[i] >= '0' && phrase[i] <= '9') || phrase[i] == '\n' || phrase[i] == '=')
    {
      phrase[out_cursor] = phrase[i];
      out_cursor++;
    }
  }
}

uint8_t SDcard_read_string(const char* device, const char* setting, char* result, uint8_t max_len)
{
  /*  search settings file for section = device
   *  search found section for line = setting
   *  find the string after '=' and remove white space
   *  errors; 1 = could not find device, 2 = could not parse setting
   */

  char working_array[MAX_LEN];
  uint8_t return_value = -1;
  uint32_t location_in_file = 0;

  //find device name location in settings file
  location_in_file = find_device_info(device);

  
  if(location_in_file > 0)
  {
    //find the devices setting name in settings file
    location_in_file = find_setting_info(location_in_file, setting);
  }
  else
  {
    return_value = 1; //could not find device in settings file
  }

  if(location_in_file > 0)
  {
    fetch_setting(location_in_file, working_array);
    clean_setting(working_array);
    return_value = 0;
  }
  else
  {
    //a value could not be found after the setting name in the file
    working_array[0] =  '\0';
    return_value = 2;
  }

  copy_char_array(result, working_array, max_len);
  return return_value;
}

uint8_t SDcard_read_int(const char* device, const char* setting, uint8_t* result)
{
  /*  conver char* type setting to int
   *  errors; 1 = could not parse setting, 2 = setting value overflow
   */
  uint8_t return_value = -1;
  char char_result[MAX_LEN];
  clear_char_array(char_result, MAX_LEN);

  SDcard_read_string(device, setting, char_result, MAX_LEN);

  if('\0' != char_result[0])
  {
    char** pointer_to_end = NULL;
    long int convertion_result = strtol(char_result, pointer_to_end, 0);
    if(0xFFFF >= convertion_result)
    {
      *result = (int8_t)convertion_result;
      return_value = 0;
    }
    else
    {
      return_value = 2;
    }
  }
  else
  {
    return_value = 1;
  }

  return return_value;
}

uint8_t fetch_setting(uint32_t start, char* string)
{
  char working_array[MAX_LEN];
  clear_char_array(working_array, MAX_LEN);

  saveFile = SD.open("settings.txt", FILE_READ);
  saveFile.seek(start);

  for(int i = 0; i < MAX_LEN; i++)
  {
    if(saveFile.available())
    {
      working_array[i] = saveFile.read();

      if (working_array[i] == '\n')
      {
        i = MAX_LEN;
      }
    }
  }

  saveFile.close();

  copy_char_array(string, working_array, MAX_LEN);
}


void clean_setting(char* string)
{
  char* working_array = string;
  bool copying = 1;
  int in_cursor, out_cursor = 0;

  while(copying)
  {
    working_array[out_cursor] = working_array[in_cursor];

    if(out_cursor == 0 && working_array[in_cursor] == ' ')
    {
      in_cursor++;
    }
    else if(working_array[in_cursor] == '\n')
    {
      copying = 0;
      working_array[out_cursor] = '\0';
    }
    else
    {
      in_cursor++;
      out_cursor++;
    }
  }

  copy_char_array(string, working_array, MAX_LEN);
}

void create_settings_file(void)
{
  saveFile = SD.open("settings.txt", FILE_WRITE);

  saveFile.write("System:\n");
  saveFile.write("Flush time(seconds) = 15\n");

  saveFile.write("\n");

  saveFile.write("Product1:\n");
  saveFile.write("Dosage =\n");
  saveFile.write("Label = Product A\n");
  saveFile.write("Calibration =\n");

  saveFile.write("Product2:\n");
  saveFile.write("Dosage =\n");
  saveFile.write("Label = Product B\n");
  saveFile.write("Calibration =\n");

  saveFile.write("Product3:\n");
  saveFile.write("Dosage =\n");
  saveFile.write("Label = Product C\n");
  saveFile.write("Calibration =\n");

  saveFile.write("Product4:\n");
  saveFile.write("Dosage =\n");
  saveFile.write("Label = Product D\n");
  saveFile.write("Calibration =\n");

  saveFile.write("\n");

  saveFile.write("Washer1:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 1\n");

  saveFile.write("Washer2:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 2\n");

  saveFile.write("Washer3:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 3\n");

  saveFile.write("Washer4:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 4\n");

  saveFile.write("Washer5:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 5\n");

  saveFile.write("Washer6:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 6\n");

  saveFile.write("Washer7:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 7\n");

  saveFile.write("Washer8:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 8\n");

  saveFile.write("Washer9:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 9\n");

  saveFile.write("Washer10:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 10\n");

  saveFile.write("Washer11:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 11\n");

  saveFile.write("Washer12:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 12\n");

  saveFile.write("Washer13:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 13\n");

  saveFile.write("Washer14:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 14\n");

  saveFile.write("Washer15:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 15\n");

  saveFile.write("Washer16:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 16\n");

  saveFile.write("Washer17:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 17\n");

  saveFile.write("Washer18:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 18\n");

  saveFile.write("Washer19:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 19\n");

  saveFile.write("Washer20:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 20\n");

  saveFile.write("Washer21:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 21\n");

  saveFile.write("Washer22:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 22\n");

  saveFile.write("Washer23:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 23\n");

  saveFile.write("Washer24:\n");
  saveFile.write("Capacity(lbs) =\n");
  saveFile.write("Label = Washer 24\n");

  saveFile.write("end\n");

  saveFile.close();
}
