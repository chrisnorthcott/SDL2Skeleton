#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "math.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;

class Sprite
{
	Sprite(const char *filename);
	SDL_Texture *tex;
	int x, y, w, h; 
	Vector2 v;
};

bool GraphicsInit();
void SetLogicalResolution(int w, int h);
void ClearScreen(int r, int g, int b, int a);

#endif
