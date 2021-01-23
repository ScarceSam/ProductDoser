#include <Arduino.h>
#include "Menutree.h"

enum menu_index{
  MAIN_MENU,
    SYSTEM,
      S_MANUAL_FUNCTIONS,
    WASHERS,
      W_CHANGE_NAME,
      W_CHANGE_WEIGHT,
      W_MANUAL_FUNCTIONS,
    PRODUCTS,
      P_CALIBRATION,
      P_CHANGE_NAME,
      P_MANUAL_FUNCTIONS,
    ERRORS,
    LOGGING
};

typedef struct node{
  const char* node_name;
  bool is_function;
  void (*function)(void);
  int parent;
  int child;
  int next_sibling;
  int prev_sibling;
}menu_node_t;

const menu_node_t MenuNodes[] = {
//
// node_name,            is_function,    (*function)(void),    parent,                child,                next_sibling,          Prev_sibling
//
  {"Menu",               false,          nullptr,              -1,                  SYSTEM,               NULL,                  NULL},
    {"System",             false,          nullptr,              MAIN_MENU,             S_MANUAL_FUNCTIONS,   WASHERS,               LOGGING},
      {"Manual Functions",   false,          nullptr,              SYSTEM,                NULL,                 NULL,                  NULL},
    {"Washers",            false,          nullptr,              MAIN_MENU,             W_CHANGE_NAME,        PRODUCTS,              SYSTEM},
      {"Change Name",        false,          nullptr,              WASHERS,               NULL,                 W_CHANGE_WEIGHT,       W_MANUAL_FUNCTIONS},
      {"Change Weight",      false,          nullptr,              WASHERS,               NULL,                 W_MANUAL_FUNCTIONS,    W_CHANGE_NAME},
      {"Manual Functions",   false,          nullptr,              WASHERS,               NULL,                 W_CHANGE_NAME,         W_CHANGE_WEIGHT},
    {"Products",           false,          nullptr,              MAIN_MENU,             P_CHANGE_NAME,        ERRORS,                WASHERS},
      {"Pump Calibration",   false,          nullptr,              PRODUCTS,              NULL,                 P_CHANGE_NAME,         P_MANUAL_FUNCTIONS},
      {"Change Name",        false,          nullptr,              PRODUCTS,              NULL,                 P_MANUAL_FUNCTIONS,    P_CALIBRATION},
      {"Manual Functions",   false,          nullptr,              PRODUCTS,              NULL,                 P_CALIBRATION,         P_CHANGE_NAME},
    {"Errors",             false,          nullptr,              MAIN_MENU,             NULL,                 LOGGING,               PRODUCTS},
    {"Logging",            false,          nullptr,              MAIN_MENU,             NULL,                 SYSTEM,                ERRORS}
};

int menu_get_parent(int node)
{
  return MenuNodes[node].parent;
}

char* menu_get_name(int node)
{
  return MenuNodes[node].node_name;
}

int menu_get_child(int node)
{
  return MenuNodes[node].child;
}

int menu_get_next_sibling(int node)
{
  return MenuNodes[node].next_sibling;
}

int menu_get_prev_sibling(int node)
{
  return MenuNodes[node].prev_sibling;
}
