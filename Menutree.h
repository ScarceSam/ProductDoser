#pragma once

#define MENU_ROOT 0

int menu_get_parent(int node);

char* menu_get_name(int node);

int menu_get_child(int node);

int menu_get_next_sibling(int node);

int menu_get_prev_sibling(int node);
