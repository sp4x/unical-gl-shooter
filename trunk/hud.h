/**
 * Copyright (C) 2011 Vincenzo Pirrone, Salvatore Loria
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

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
void draw_text2d (int x, int y, char *string, void* font);

/* Draws the heads up display on screen */
void draw_hud (void);

void show_blood (void);
void add_cube (void);
void del_cube (void);

#endif
