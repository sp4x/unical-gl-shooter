/*------------------------------------------------------------------------------
* Alpha Shooter
* A 3D OpenGL First-Person Shooter game
* Copyright (C) 2005, 2006, 2007 Nicola Cocchiaro <ncocchiaro@users.sf.net>
* Alpha Shooter can be found at http://www.sf.net/projects/alphashooter.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 or, (at your option) any later
* version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*-------------------------------------------------------------------------------
*/

// tgaload.h
// header file for TGA image loader

#ifndef ALPSHOOT_TGALOAD_H
#define ALPSHOOT_TGALOAD_H

#include <stdlib.h>
#include <stdio.h>

#ifdef ALPSHOOT_WIN32
#include <windows.h>
#endif

#ifndef ALPSHOOT_MACOSX
  #ifndef ALPSHOOT_WIN32
    #include <endian.h>
  #endif
#include <GL/glut.h>
#else
#include <glut.h>
#endif

#define TGA_RGB		2
#define TGA_A		3
#define TGA_RLE		10

#define CHECK_READ_ERR(name, res, stream)				\
if (res == 0)								\
{									\
	if (ferror(stream))						\
	{								\
		printf("Error: Reading from file %s failed.\n", name);	\
		fclose(stream);						\
		return NULL;						\
	}								\
}									\

struct tImageTGA
{
	int channels;
	int sizeX;
	int sizeY;
	unsigned char *data;
};
typedef struct tImageTGA tImageTGA;

tImageTGA *loadTGA(const char *filename, int *status);

#endif
