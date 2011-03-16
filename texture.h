
#ifndef TEXTURE_H
#define TEXTURE_H

#define MAX_LOADABLE_TEXTURES 10

#define TEXTURE_BRICK "texture/brick.tga"
#define TEXTURE_BRONZE "texture/bronze.tga"       
#define TEXTURE_BUMPPLAT "texture/bumpplat.tga"
#define TEXTURE_DARK_METAL "texture/dark_metal.tga"
#define TEXTURE_METAL2 "texture/metal2.tga"
#define TEXTURE_METAL_PLATE_FILL "texture/metal_plate_fill.tga"
#define TEXTURE_STONE "texture/Stone.tga"
#define TEXTURE_WHITE "texture/white.tga"

void loadTexture(const char *texname);
void cleanTextures();


#endif
