
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
#define TEXTURE_AR15 "texture/ar15.tga"
#define TEXTURE_SHOOTGUN "texture/shootgun.tga"
#define TEXTURE_TURRET "texture/turret.tga"
//~ #define SKYBOX "texture/skybox/sky.tga"
//~ #define TEXTURE_NORTH "texture/skybox/north.tga"
//~ #define TEXTURE_SOUTH "texture/skybox/south.tga"
//~ #define TEXTURE_EAST "texture/skybox/east.tga"
//~ #define TEXTURE_WEST "texture/skybox/west.tga"
//~ #define TEXTURE_UP "texture/skybox/up.tga"
//~ #define TEXTURE_DOWN "texture/skybox/down.tga"

void loadTexture(const char *texname);
void cleanTextures();

#endif
