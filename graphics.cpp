
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "graphics.h"
#include "state.h"

#ifndef HEXDUMP_COLS
#define HEXDUMP_COLS 8
#endif

using namespace std;

SDL_Window 		*window;
SDL_Renderer	*renderer;
void (*RenderCallback)();

bool GraphicsInit()
{
	cout << "Initialising graphics...";

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

	cout << "Initialising image loader...";
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		cout << "Couldn't initialise image loader: " << IMG_GetError() << endl;
	}
	cout << "OK" << endl;

	cout << "Initialising font loader...";
	if(TTF_Init() < 0)
	{
		cout << "Couldn't initialise font loader: " << TTF_GetError() << endl;
	}
	cout << "OK" << endl;
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
}

Sprite::Sprite(){}

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
	Sprite::translate = Vector2(0,0);
	Sprite::scale = Vector2(1,1);
	Sprite::rotate = 0.0f;
}

void IRenderable::Rotate(float angle)
{
	rotate += angle;
}

void IRenderable::Translate(Vector2 t)
{
	this->translate = this->translate + t;
}

void IRenderable::Scale(Vector2 s)
{
	this->scale = s;
}

void IRenderable::Render(SDL_Renderer *rend)
{
	SDL_Rect dst;
	SDL_Point center;
	dst.x = this->translate.x;
	dst.y = this->translate.y;
	dst.w = this->w * this->scale.x;
	dst.h = this->h * this->scale.y;
	center.x = this->w / 2;
	center.y = this->h / 2;
	if(SDL_RenderCopyEx(renderer, this->tex, NULL, 
		&dst, this->rotate, &center, SDL_FLIP_NONE) < 0)
	{
		std::cout << "Couldn't render:" 
			<< SDL_GetError() << endl;
		SwitchState(ST_EXIT);
	}
}

Text::Text(){}

Text::Text(const char *text, TTF_Font *font, SDL_Color color)
{
	strncpy(Text::text, text, 1024);
	Text::font = font;
	Text::color = (SDL_Color){ 255, 255, 255, 255 };	//white by default
	Text::rotate = 0;
	Text::scale = Vector2(1,1);
	Text::translate = Vector2(0,0);
	SDL_Surface *tmpSfc = TTF_RenderText_Blended(font, 
		text, color);

	if(!tmpSfc)
	{
		std::cout << "Couldn't TTF_RenderText_Blended: " <<
			TTF_GetError() << endl;
	}
	Text::tex = SDL_CreateTextureFromSurface(renderer, tmpSfc);
	if(!Text::tex)
	{
		std::cout << "Couldn't SDL_CreateTextureFromSurface: " <<
			SDL_GetError() << endl;
	}
	int qw, qh;
	SDL_QueryTexture(tex, NULL, NULL, &qw, &qh);
	Text::w = qw;
	Text::h = qh;
}

SpriteSheet::SpriteSheet(){}

SDL_Surface * SpriteSheet::getCurrentFrame()
{
	/* FIXME: this needs to be updated to use GPU RTT */

    //HACK: avoid gcc narrowing conversion error
    int sx = (int)this->spriteSize.x;
    int sy = (int)this->spriteSize.y;

	SDL_Surface *s = SDL_CreateRGBSurface(0,
		sx, 
		sy,
		32, 
		//0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
		0x000000FF,0x0000FF00,0x00FF0000,0xFF000000
		);
	SDL_Rect src = {
		currentFrame * sx,
		0,						
		sx,			
		sy		
	};
	if(SDL_BlitSurface(this->sheet, &src, s, NULL) < 0)
	{
		std::cout << "Failed to SDL_BlitSurface" << endl;
		SwitchState(ST_EXIT);
	}

	return s;

}

SpriteSheet::SpriteSheet(const char* filename, Point spriteSize, int nFrames)
{
	SpriteSheet::spriteSize = spriteSize;
	SpriteSheet::currentFrame = 1;
	SDL_Surface *image = IMG_Load(filename);
	if(image == NULL)
	{
		std::cout << "Fail to load " << filename << "!";
		SwitchState(ST_EXIT);
	}
	SpriteSheet::nFrames = nFrames;
	SpriteSheet::spriteSize = spriteSize;
	SpriteSheet::sheet = image;
	SpriteSheet::w = spriteSize.x;
	SpriteSheet::h = spriteSize.y;
	SpriteSheet::translate = Vector2(0,0);
	SpriteSheet::scale = Vector2(1,1);
	SpriteSheet::rotate = 0.0f;
}

void SpriteSheet::RenderToTexture(SDL_Renderer *rend)
{
	SDL_Surface *tmpSfc = this->getCurrentFrame();
	this->tex = SDL_CreateTextureFromSurface(rend, tmpSfc);
}

void SpriteSheet::SelectFrame(int frameNum)
{
	if(frameNum > this->nFrames)
	{
		this->currentFrame = this->currentFrame;
		std::cout << "WARNING: selected frame past end of sheet";
	}else{
		this->currentFrame = frameNum;
	}
}

bool SpriteSheet::AtEnd()
{
	if(this->currentFrame > this->nFrames)
		return true;
	else
		return false;
}
