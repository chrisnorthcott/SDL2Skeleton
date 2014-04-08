#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "math.h"
#include "state.h"

#define FONT_DEFAULT	"fonts/source.otf"
extern TTF_Font *defaultFont;		//You need to put source.otf (Adobe Source Sans Pro) in
							//fonts/ for this to work out the box.
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern void (*RenderCallback)();

class IRenderable
{
public:
	void Render(SDL_Renderer *rend);
	void Rotate(float angle);
	void Translate(Vector2 t);
	void Scale(Vector2 s);

	int w, h;
	float rotate;
	Vector2 scale;
	Vector2 translate;
protected:
	SDL_Texture *tex;
};

class Sprite : public IRenderable
{
public:
	Sprite();
	Sprite(const char *name, const char *filename);
};

class Text : public IRenderable
{
public:
	Text();
	Text(const char *text, TTF_Font *font, SDL_Color color);
	char text[1024];
	TTF_Font *font;
	SDL_Color color;
};

bool GraphicsInit();
void SetLogicalResolution(int w, int h);
void ClearScreen(int r, int g, int b, int a);

#endif
