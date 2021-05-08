#pragma once

void controller_update_screen(void);

int controller_manual_func(char displaied_text[4][21], int* buttons_pressed);

long controller_calibrate_product(char displaied_text[4][21], int* buttons_pressed);
