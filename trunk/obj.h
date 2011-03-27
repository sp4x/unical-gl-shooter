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

#ifndef OBJ_H
#define OBJ_H

typedef struct vertex
{
    float x, y, z;
    
} vertex;

typedef struct vertex normal;

typedef struct tex_coord
{
    float u, v;
    
} tex_coord;

typedef struct face
{
    int vertices[3];
    int normals[3];
    int tex_coords[3];
    
} face;

typedef struct model
{
    int n_vertices, n_normals, n_tex_coords, n_faces;
    vertex* vertices;
    normal* normals;
    tex_coord* tex_coords;
    face* faces;
    
} model;


model* loadModel (char* buffer, int size);
void drawModel (model * m);
int loadFile (char* file_name, char** buffer);

#endif
