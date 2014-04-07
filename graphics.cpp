
#include <iostream>
#include <vector>
#include "graphics.h"
#include "state.h"

using namespace std;

SDL_Window 		*window;
SDL_Renderer	*renderer;

bool GraphicsInit()
{
	cout << "Graphics Init" << endl;

	if(SDL_CreateWindowAndRenderer(0, 0,
		SDL_WINDOW_FULLSCREEN_DESKTOP,
		&window, &renderer) < 0)
	{
		cout << "Couldn't initialise graphics:" << SDL_GetError() << endl;
		return false;
	}

	//grab some info
	SDL_RendererInfo info;
	if(SDL_GetRenderDriverInfo(0, &info) < 0)
	{
		cout << "WARNING: Couldn't probe driver..." << endl;
	}else{
		cout << "Renderer: " << info.name << " [";
		if(info.flags & SDL_RENDERER_SOFTWARE)
		{
			cout << "Software";
		}
		if(info.flags & SDL_RENDERER_ACCELERATED)
		{
			cout << "Hardware";
		}
		if(info.flags & SDL_RENDERER_PRESENTVSYNC)
		{
			cout << ",Vsync";
		}
		if(info.flags & SDL_RENDERER_TARGETTEXTURE)
		{
			cout << ",RenderToTexture";
		}
		cout << "] " << endl;
	}

	cout << "Image Loader init" << endl;
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		cout << "Couldn't initialise image loader: " << IMG_GetError() << endl;
	}
	
	return true;
}

void SetLogicalResolution(int w, int h)
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, w, h); 
}

void ClearScreen(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

Sprite::Sprite(const char *filename)
{
	SDL_Texture *image = IMG_LoadTexture(renderer, filename);
	if(image == NULL)
	{
		std::cout << "FAILED to load " << filename << "!";
		SwitchState(ST_EXIT);
	}
	int w, h;
	SDL_QueryTexture(image, NULL, NULL, &w, &h);
	Sprite::tex = image;
	Sprite::w = w;
	Sprite::h = h;
	Sprite::x = 0;
	Sprite::y = 0;
}