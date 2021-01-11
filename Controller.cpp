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
  static menu_node_t* menu_location = nullptr;
  static long interaction_at = 0;
  static long menu_timeout = 5000;

  if(buttons_pressed)
  {
    interaction_at = millis();
  }

  if(!menu_location && (buttons_pressed & (1<<3)))
  {
    menu_location = first_node();
    view_clear();
    view_println(menu_location->node_name);
    view_println(menu_location->child->node_name);
    view_println(menu_location->child->next_sibling->node_name);
    view_println(menu_location->child->next_sibling->next_sibling->node_name);
  }
  else if((menu_location) && (buttons_pressed & (1<<1)))
  {
    menu_location = menu_location->parent;
  }

  if((millis() - interaction_at) > menu_timeout)
  {
    menu_location = nullptr;
  }

  return menu_location;
}
