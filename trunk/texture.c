
#include <string.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <stdio.h>
#include "texture.h"
#include "tgaload.h"

char *loaded_textures[MAX_LOADABLE_TEXTURES];
GLuint names[MAX_LOADABLE_TEXTURES];
int last_texture_index = 0;


int getTextureIndex(const char *texname) {
	int i;
	for (i=0;i<last_texture_index;i++)
		if ( strcmp(loaded_textures[i], texname) == 0 )
			return i;
	return -1;
}


GLuint newTexture(const char *texname) {
	int i = last_texture_index;
	
	if (last_texture_index < MAX_LOADABLE_TEXTURES)
		last_texture_index++;
	else
		//if all texture slots are used clean the last one
		free(loaded_textures[i]);
		
	loaded_textures[i] = malloc( strlen(texname)+1 );
	strcpy(loaded_textures[i], texname);
	return names[i];
}

void loadTexture(const char *texname) {
	if (last_texture_index == 0)
		glGenTextures(MAX_LOADABLE_TEXTURES, names);
	
	int index = getTextureIndex(texname);
	if (index == -1) {
		
		glBindTexture( GL_TEXTURE_2D, newTexture(texname) );
		
		//~ GLint iWidth, iHeight, iComponents;
		//~ GLenum eFormat;
		//~ GLubyte *pBytes = gltLoadTGA(texname, &iWidth, &iHeight, &iComponents, &eFormat);
		//~ glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
		//~ free(pBytes);
		
		//~ glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//~ glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//~ glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//~ glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//~ 
		//~ glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		
		int res = 0, format = GL_RGB;
		tImageTGA *tex = malloc(sizeof(tImageTGA));
		tex = loadTGA(texname, &res);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		gluBuild2DMipmaps(GL_TEXTURE_2D, format, tex->sizeX, tex->sizeY, format, GL_UNSIGNED_BYTE, tex->data);

		free(tex->data);
		tex->data = NULL;
		free(tex);
		tex = NULL;
		free(tex);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, names[index]);
	}
}

void cleanTextures() {
	int i;
	for (i=0; i<last_texture_index; i++) {
		free(loaded_textures[i]);
	}
}
