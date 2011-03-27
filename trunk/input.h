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

#ifndef INPUT_H
#define INPUT_H

#define DOWN 1
#define UP 0

#include <GL/glut.h>

void mouse_motion (int x, int y);
void mouse_func (int button, int state, int x, int y);
void key_down (unsigned char key, int x, int y);
void key_up (unsigned char key, int x, int y);
void spec_key_down (int key, int x, int y);
void spec_key_up (int key, int x, int y);
void input_update (void);
void input_disable (void);

#endif
