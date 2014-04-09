#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "math.h"
#include "state.h"

#ifdef DEBUG
/*
	Neat hack: simply BREAKPOINT on a line on its own
	and GDB will auto breakpoint for you.

	This is in this file in particular due to the 
	relative compexity and difficulty of debugging code 
	that crosses CPU and GPU; not to mention SDL+GDB is a
	cumbersome mix. I got sick of "bp graphics.cpp:198"
	every bloody time.

	To resume, say

		gdb>signal 0
*/
#include <csignal>
#define BREAKPOINT raise(SIGABRT)
#else
/*
	If not in Debug profile, do nothing.
*/
#define BREAKPOINT 0
#endif

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

class SpriteSheet : public IRenderable
{
	friend class SpriteAnim;
public:
	SpriteSheet();
	SpriteSheet(const char *filename, Point spriteBoundary, int nFrames);
	void RenderToTexture(SDL_Renderer *rend);
	bool AtEnd();
	void SelectFrame(int frameNum);
private:
	SDL_Surface *getCurrentFrame();
	SDL_Surface *sheet;
	Point spriteSize;
	int currentFrame, nFrames;

};

class Sprite : public IRenderable
{
public:
	Sprite();
	Sprite(const char *filename);
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
