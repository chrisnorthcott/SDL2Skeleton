#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "math.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern void (*RenderCallback)();

class Sprite
{
	Sprite(const char *filename);
	void Render();
	void Rotate(float angle);
	void Transform(Vector2 t);
	void Scale(Vector2 s);
	SDL_Texture *tex;
	int w, h;
	float rotate;
	Vector2 scale;
	Vector2 transform;
	Vector2 velocity;
};

bool GraphicsInit();
void SetLogicalResolution(int w, int h);
void ClearScreen(int r, int g, int b, int a);
void SetRenderCallback(void (*callback)());

#endif
