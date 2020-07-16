#include <Arduino.h>
#include "SDcard.h"
#include <SD.h>
#include <SPI.h>

static File saveFile;

uint32_t find_device_info(String device);
uint32_t find_setting_info(uint32_t start, String setting);
String fetch_setting(uint32_t start);
String clean_setting(String value);
void remove_characters(char phrase[]);

uint8_t sdcard_init(void)
{
  uint8_t return_value = 0;

  if (SD.begin(BUILTIN_SDCARD)) {
    return_value += 1;
  }

  return return_value;
}

String SDcard_read_string(String device, String setting)
{
  String return_string;
  uint32_t location_in_file = 0;

  location_in_file = find_device_info(device);

  if(location_in_file > 0)
    location_in_file = find_setting_info(location_in_file, setting);

  if(location_in_file == 0)
  {
    return_string =  "\0";
  }
  else
  {
    return_string = fetch_setting(location_in_file);
    return_string = clean_setting(return_string);
  }

  return return_string;
}

int32_t SDcard_read_int(String device, String setting)
{
  String value = SDcard_read_string(device, setting); 

  int32_t number = -1;

  if('\0' != value[0])
  {
    number = value.toInt();
  }

  return number;
}

uint32_t find_device_info(String device)
{
  //open file
  saveFile = SD.open("settings.txt", FILE_READ);
  char line[60];

  //find device line
  bool device_found = 0;
  bool end_of_file = 0;
  while(device_found == 0 && saveFile.available())
  {
    for( int i = 0; i < 60; i++)
    {
      
      if(saveFile.available())
      {
        line[i] = saveFile.read();
        String temp = line[i];
        temp.toLowerCase();
        line[i] = temp[0];
      }else{
        line[i] = '\n';
        end_of_file = 1;
      }
      
      if( line[i] == '\n')
      {
        i = 60;
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

uint32_t find_setting_info(uint32_t start, String setting)
{
  saveFile = SD.open("settings.txt", FILE_READ);
  char line[60];
  saveFile.seek(start);

  bool device_found = 0;
  bool end_of_file = 0;
  while(!device_found && saveFile.available())
  {
    for(int i = 0; i < 60; i++)
    {
      if(saveFile.available())
      {
        line[i] = saveFile.read();
        String temp = line[i];
        temp.toLowerCase();
        line[i] = temp[0];
      }
      else
      {
        line[i] = '\n';
        end_of_file = 1;
      }
      
      if( line[i] == '\n' || line[i] == '=')
      {
        i = 60;
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

  if(end_of_file)
    return_value = 0;

  return return_value;
}

String fetch_setting(uint32_t start)
{
  saveFile = SD.open("settings.txt", FILE_READ);
  char working_array[60];
  saveFile.seek(start);

  for(int i = 0; i < 60; i++)
  {
    if(saveFile.available())
    {
      working_array[i] = saveFile.read();

      if (working_array[i] == '\n')
      {
        i = 60;
      }
    }
  }
    
  saveFile.close();

  String return_string = working_array;
  return return_string;
}

String clean_setting(String input)
{
  char working_array[60];
  input.toCharArray(working_array, 60);

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
  
  String return_string = working_array;
  return return_string;
}

void remove_characters(char phrase[])
{
  int out_cursor = 0;
  for(int i = 0; i < 60; i++)
  {
    if((phrase[i] >= 'a' && phrase[i] <= 'z') || (phrase[i] >= '0' && phrase[i] <= '9') || phrase[i] == '\n' || phrase[i] == '=')
    {
      phrase[out_cursor] = phrase[i];
      out_cursor++;
    }
  }
}
