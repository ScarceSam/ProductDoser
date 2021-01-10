#include <Arduino.h>
#include "Menutree.h"

menu_node_t main_node = {"Main Menu"};
  menu_node_t washer_node = {"Washers"};
    menu_node_t washer_name_node = {"Change Name"};
    menu_node_t washer_weight_node = {"Change Weight"};
    menu_node_t washer_manual_node = {"Manual function"};
  menu_node_t system_node = {"System"};
    menu_node_t system_manual_node = {"Manual function"};
  menu_node_t product_node = {"Products"};
    menu_node_t product_cal_node = {"Calibration"};
    menu_node_t product_name_node = {"Change Name"};
    menu_node_t product_dosage_node = {"Change Dosage"};
    menu_node_t product_manual_node = {"Manual function"};
  menu_node_t errors_node = {"Errors"};
  menu_node_t log_node = {"Logging"};

void menu_init(void){
  main_node.parent = nullptr;
  main_node.child = &washer_node;
  main_node.prev_sibling = nullptr;
  main_node.next_sibling = nullptr;

    washer_node.parent = &main_node;
    washer_node.child = nullptr;
    washer_node.prev_sibling = &log_node;
    washer_node.next_sibling = &system_node;

    system_node.parent = &main_node;
    system_node.child = nullptr;
    system_node.prev_sibling = &washer_node;
    system_node.next_sibling = &product_node;

    product_node.parent = &main_node;
    product_node.child = nullptr;
    product_node.prev_sibling = &system_node;
    product_node.next_sibling = &errors_node;

    errors_node.parent = &main_node;
    errors_node.child = nullptr;
    errors_node.prev_sibling = &product_node;
    errors_node.next_sibling = &log_node;

    log_node.parent = &main_node;
    log_node.child = nullptr;
    log_node.prev_sibling = &errors_node;
    log_node.next_sibling = &washer_node;
}

menu_node_t* first_node(void)
{
  return &main_node;
}
