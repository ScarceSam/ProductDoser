#include <Arduino.h>
#include "View.h"
#include "State.h"
#include "Buttons.h"
#include "Menutree.h"

void display_state(void);
bool display_menu(int buttons_pressed);

void controller_update_screen(void)
{
  static bool in_menu = false;
  int nav_buttons = buttons_pushed();

  if(nav_buttons || in_menu)
  {
    in_menu = display_menu(nav_buttons);
  }

  if(!in_menu)
  {
    display_state();
  }
}

void display_state(void){
  static uint8_t previous_state = 0;
  static uint16_t previous_sec = 0;

  uint8_t current_state = state_current_state();
  uint16_t current_sec = 0;

  if(current_state == IDLE_STEP)
  {
    current_sec = (millis() / 1000);
  }
  else
  {
    current_sec = (state_remaining_millis() / 1000);
  }

  if ((current_state != previous_state) || (current_sec != previous_sec))
  {
    view_display_state();
    previous_state = current_state;
    previous_sec = current_sec;
  }
}

bool display_menu(int buttons_pressed)
{
  bool return_value = false;
  static int menu_location = -1;
  static int menu_selection = 0;
  static long interaction_at = 0;
  static long menu_timeout = 5000;

  if(buttons_pressed)
  {
    interaction_at = millis();
  }
  else if((millis() - interaction_at) > menu_timeout)
  {
    menu_location = -1;
  }

  if((menu_location < 0) && (buttons_pressed & (1<<3)))
  {
    menu_location = 0;
    menu_selection = menu_get_child(menu_location);
  }
  else if((menu_location >= 0) && (buttons_pressed & (1<<1)))
  {
    menu_location = menu_get_parent(menu_location);
  }

  if((menu_location >= 0) && buttons_pressed)
  {
    view_clear();
    view_println(menu_get_name(menu_location));
    view_println(menu_get_name(menu_selection));
  }

  (menu_location >= 0) ? (return_value = true) : (return_value = false);
  return return_value;
}
