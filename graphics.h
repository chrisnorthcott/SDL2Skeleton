#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL2/SDL.h>

#define RGBA(r,g,b,a)	r, g, b, a

extern SDL_Window *window;
extern SDL_Renderer *renderer;

bool GraphicsInit();
void SetLogicalResolution(int w, int h);
void ClearScreen(int r, int g, int b, int a);

#endif
