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
