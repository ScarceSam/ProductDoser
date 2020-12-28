#include <Arduino.h>
#include "Buttons.h"
#include <Wire.h>
#include <SparkFunSX1509.h>

SX1509 expander;

const int BUTTONS[] = {0, 1, 2, 3, 4};
const int BUTTON_COUNT = (sizeof(BUTTONS)/sizeof(BUTTONS[0]));
const int DEBOUNCE = 64;

void buttons_init(void)
{
  expander.begin(0x3E);
  
  for(int i = 0; i < BUTTON_COUNT; i++)
  {
    expander.pinMode(BUTTONS[i], INPUT_PULLUP);
    expander.enableInterrupt(BUTTONS[i], CHANGE);
  }
  expander.debounceTime(DEBOUNCE);
  for(int i = 0; i < BUTTON_COUNT; i++)
  {
    expander.debouncePin(BUTTONS[i]);
  }
}

int buttons_pushed(void)
{
  static int last_pushed = 0;
  int pushed = 0;
  
  for(int i = 0; i < BUTTON_COUNT; i++)
  {
    if(expander.digitalRead(BUTTONS[i]) == LOW)
    {
      pushed |= (1<<i);
    }
    else
    {
      last_pushed &= ~(1<<i);
    }
  }

  pushed = pushed - last_pushed;
  last_pushed += pushed;
  return pushed;
}
