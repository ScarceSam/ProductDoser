#pragma once

#define MENU_ROOT 0

int menu_get_parent(int node);

char* menu_get_name(int node);

int menu_get_child(int node);

int menu_get_next_sibling(int node);

int menu_get_prev_sibling(int node);

bool menu_is_function(int node);

void menu_function(int menu_location,char displaied_text[4][21], int* buttons_pressed);
