#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <GL/glut.h>
#include "obj.h"

void drawModel (model *m)
{
	glPushMatrix();
	
	glBegin (GL_TRIANGLES);
		
	int i;
	for (i = 0; i < m->n_faces; i++)
	{
		face t = m->faces[i];
		
		vertex v1 = m->vertices[ t.vertices[0]-1 ];
		vertex v2 = m->vertices[ t.vertices[1]-1 ];
		vertex v3 = m->vertices[ t.vertices[2]-1 ];
		
		normal n1 = m->normals[ t.normals[0]-1 ];
		normal n2 = m->normals[ t.normals[1]-1 ];
		normal n3 = m->normals[ t.normals[2]-1 ];
		
		tex_coord c1 = m->tex_coords[ t.tex_coords[0]-1 ];
		tex_coord c2 = m->tex_coords[ t.tex_coords[1]-1 ];
		tex_coord c3 = m->tex_coords[ t.tex_coords[2]-1 ];
		
		glTexCoord2f (c1.u, c1.v);
		glNormal3f (n1.x, n1.y, n1.z);
		glVertex3f (v1.x, v1.y, v1.z);
		
		glTexCoord2f (c2.u, c2.v);
		glNormal3f (n2.x, n2.y, n2.z);
		glVertex3f (v2.x, v2.y, v2.z);
		
		glTexCoord2f (c3.u, c3.v);
		glNormal3f (n3.x, n3.y, n3.z);
		glVertex3f (v3.x, v3.y, v3.z);
	}
		
	glEnd();	
	
	glPopMatrix();
}

model* loadModel (char* buffer, int size)
{
    char *p = NULL, *e = NULL;
    model* m = malloc (sizeof(model));
    memset (m, 0, sizeof(model));
    p = buffer;
    e = buffer + size;
    while (p != e)
    {
        if (memcmp (p, "vn", 2) == 0)
            m->n_normals++;
        else if (memcmp (p, "vt", 2) == 0)
            m->n_tex_coords++;
        else if (memcmp (p, "v",  1) == 0)
            m->n_vertices++;
        else if (memcmp (p, "f",  1) == 0)
            m->n_faces++;
            
        // 0x0A = LF
        while (*p++ != (char) 0x0A);
    }

    m->vertices = malloc (sizeof(vertex) * m->n_vertices);
    m->normals = malloc (sizeof(normal) * m->n_normals);
    m->tex_coords = malloc (sizeof(tex_coord) * m->n_tex_coords);
    m->faces = malloc (sizeof(face) * m->n_faces);
    
    p = buffer;
    int nV = 0, nN = 0, nT = 0, nF = 0;
    while (p != e)
    {
        if (memcmp (p, "vn", 2) == 0)
        {
            sscanf (p, "vn %f %f %f", &m->normals[nN].x, &m->normals[nN].y, &m->normals[nN].z);
            nN++;
        }
        else if (memcmp (p, "vt", 2) == 0)
        {
            sscanf (p, "vt %f %f", &m->tex_coords[nT].u, &m->tex_coords[nT].v);
            nT++;
        }
        else if (memcmp (p, "v", 1) == 0) /* or *p == 'v' */
        {
            sscanf (p, "v %f %f %f", &m->vertices[nV].x, &m->vertices[nV].y, &m->vertices[nV].z);
            nV++;
        }
        else if (memcmp (p, "f", 1) == 0) /* or *p == 'f' */
        {
            sscanf (p, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
            		&m->faces[nF].vertices[0], &m->faces[nF].tex_coords[0], &m->faces[nF].normals[0], 
            		&m->faces[nF].vertices[1], &m->faces[nF].tex_coords[1], &m->faces[nF].normals[1], 
            		&m->faces[nF].vertices[2], &m->faces[nF].tex_coords[2], &m->faces[nF].normals[2]);
            nF++;
        }
        while (*p++ != (char) 0x0A);
    }

    return m;
}

int loadFile (char* file_name, char** buffer)
{
    int bytes = 0;
    FILE* file = fopen (file_name, "r");
    if (file != NULL)
    {
        fseek (file, 0, SEEK_END);
        int size = ftell (file);
        fseek (file, 0, SEEK_SET);
        *buffer = malloc (size);
        bytes = fread (*buffer, sizeof(char), size, file);
        fclose (file);
    }
    return bytes;
}
