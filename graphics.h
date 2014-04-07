#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "math.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern void (*RenderCallback)();
extern void Screenshot();

class Sprite
{
public:
	Sprite();
	Sprite(const char *name, const char *filename);
	void Render(SDL_Renderer *rend);
	void Rotate(float angle);
	void Translate(Vector2 t);
	void Scale(Vector2 s);
	char name[100];
	SDL_Texture *tex;
	int w, h;
	float rotate;
	Vector2 scale;
	Vector2 translate;
};

class Text
{
public:
	Text();
	Text(char *text, TTF_Font *font);
	void Render(SDL_Renderer *rend);
	void Rotate(float angle);
	void Translate(Vector2 t);
	void Scale(Vector2 s);
	void SetColor(SDL_Color c);
	void SetSize(int pointsize);
	char text[1024];
	int w, h;
	SDL_Texture *tex;
	TTF_Font *font;
	SDL_Color color;
	float rotate;
	Vector2 scale;
	Vector2 translate;
};

bool GraphicsInit();
void SetLogicalResolution(int w, int h);
void ClearScreen(int r, int g, int b, int a);
void SetRenderCallback(void (*callback)());

#endif
