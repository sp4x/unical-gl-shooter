#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <GL/glut.h>
#include "obj.h"

void drawModel (ObjModel *model)
{
	glPushMatrix();
	
	glBegin (GL_TRIANGLES);
		
	int i;
	for (i = 0; i < model->nTriangle; i++)
	{
		ObjTriangle t = model->TriangleArray[i];
		
		ObjVertex v1 = model->VertexArray[ t.Vertex[0]-1 ];
		ObjVertex v2 = model->VertexArray[ t.Vertex[1]-1 ];
		ObjVertex v3 = model->VertexArray[ t.Vertex[2]-1 ];
		
		ObjNormal n1 = model->NormalArray[ t.Normal[0]-1 ];
		ObjNormal n2 = model->NormalArray[ t.Normal[1]-1 ];
		ObjNormal n3 = model->NormalArray[ t.Normal[2]-1 ];
		
		ObjTexCoord c1 = model->TexCoordArray[ t.TexCoord[0]-1 ];
		ObjTexCoord c2 = model->TexCoordArray[ t.TexCoord[1]-1 ];
		ObjTexCoord c3 = model->TexCoordArray[ t.TexCoord[2]-1 ];
		
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

ObjModel* ObjLoadModel(char* memory, size_t size)
{
    char* p = NULL, * e = NULL;
    ObjModel* model = (ObjModel*) calloc(1, sizeof(ObjModel));
    memset(model, 0, sizeof(ObjModel));
    p = memory;
    e = memory + size;
    while (p != e)
    {
        if (memcmp(p, "vn", 2) == 0)
            model->nNormal++;
        else if (memcmp(p, "vt", 2) == 0)
            model->nTexCoord++;
        else if (memcmp(p, "v",  1) == 0)
            model->nVertex++;
        else if (memcmp(p, "f",  1) == 0)
            model->nTriangle++;
            
        // 0x0A = LF
        while (*p++ != (char) 0x0A);
    }

    model->VertexArray   = (ObjVertex*)   malloc(sizeof(ObjVertex) * model->nVertex);
    model->NormalArray   = (ObjNormal*)   malloc(sizeof(ObjNormal) * model->nNormal);
    model->TexCoordArray = (ObjTexCoord*) malloc(sizeof(ObjTexCoord) * model->nTexCoord);
    model->TriangleArray = (ObjTriangle*) malloc(sizeof(ObjTriangle) * model->nTriangle);
    
    p = memory;
    int nV = 0, nN = 0, nT = 0, nF = 0;
    while (p != e)
    {
        if (memcmp(p, "vn", 2) == 0)
        {
            sscanf(p, "vn %f %f %f", &model->NormalArray[nN].x, &model->NormalArray[nN].y, &model->NormalArray[nN].z);
            nN++;
        }
        else if (memcmp(p, "vt", 2) == 0)
        {
            sscanf(p, "vt %f %f", &model->TexCoordArray[nT].u, &model->TexCoordArray[nT].v);
            nT++;
        }
        else if (memcmp(p, "v", 1) == 0) /* or *p == 'v' */
        {
            sscanf(p, "v %f %f %f", &model->VertexArray[nV].x, &model->VertexArray[nV].y, &model->VertexArray[nV].z);
            nV++;
        }
        else if (memcmp(p, "f", 1) == 0) /* or *p == 'f' */
        {
            sscanf(p, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
            		&model->TriangleArray[nF].Vertex[0], &model->TriangleArray[nF].TexCoord[0], &model->TriangleArray[nF].Normal[0], 
            		&model->TriangleArray[nF].Vertex[1], &model->TriangleArray[nF].TexCoord[1], &model->TriangleArray[nF].Normal[1], 
            		&model->TriangleArray[nF].Vertex[2], &model->TriangleArray[nF].TexCoord[2], &model->TriangleArray[nF].Normal[2]);
            nF++;
        }
        while (*p++ != (char) 0x0A);
    }

    return model;
}

size_t ObjLoadFile(char* szFileName, char** memory)
{
    size_t bytes = 0;
    FILE* file = fopen(szFileName, "rt");
    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        size_t end = ftell(file);
        fseek(file, 0, SEEK_SET);
        *memory = (char*) malloc(end);
        bytes = fread(*memory, sizeof(char), end, file);
        fclose(file);
    }
    return bytes;
}





























