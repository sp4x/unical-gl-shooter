#ifndef HUD_H
#define HUD_H

#include "camera.h"
#include <stdio.h>

/* Enters othographics projection mode to draw text or images to the
 * front of the screen */
void enter_ortho_mode (void);

/* Gets back to perspective projection mode after entering orthographic mode */
void exit_ortho_mode (void);

/* Writes red text on screen in the specified position */
void draw_text2d (int x, int y, char *string);

/* Draws the heads up display on screen */
void draw_hud (void);

void show_blood (void);

#endif
