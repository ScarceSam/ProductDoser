#pragma once

typedef struct node{
  const char *node_name;
  struct node* parent = nullptr;
  struct node* child = nullptr;
  struct node* prev_sibling = nullptr;
  struct node* next_sibling = nullptr;
}menu_node_t;

void menu_init(void);

menu_node_t* first_node(void);
