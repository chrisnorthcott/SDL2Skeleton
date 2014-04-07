
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include "graphics.h"
#include "state.h"

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

Sprite::Sprite(const char *name, const char *filename)
{
#ifdef DEBUG
	cout << "[L] " << name << " <- " << filename << endl; 
#endif
	SDL_Texture *image = IMG_LoadTexture(renderer, filename);
	if(image == NULL)
	{
		std::cout << "FAILED to load " << filename << "!";
		SwitchState(ST_EXIT);
	}
	int w, h;
	strncpy(Sprite::name, name, 100);
	SDL_QueryTexture(image, NULL, NULL, &w, &h);
	Sprite::tex = image;
	Sprite::w = w;
	Sprite::h = h;
	Sprite::translate = Vector2(0,0);
	Sprite::scale = Vector2(1,1);
	Sprite::rotate = 0.0f;
}

Sprite::Sprite()
{

}

void Sprite::Rotate(float angle)
{
	rotate += angle;
}

void Sprite::Translate(Vector2 t)
{
	this->translate = this->translate + t;
}

void Sprite::Scale(Vector2 s)
{
	this->scale = s;
}

void Sprite::Render(SDL_Renderer *rend)
{
	SDL_Rect dst;
	SDL_Point center;
	dst.x = this->translate.x;
	dst.y = this->translate.y;
	dst.w = this->w * this->scale.x;
	dst.h = this->h * this->scale.y;
	center.x = this->w / 2;
	center.y = this->h / 2;
#ifdef DEBUG_MORE
	cout << "[R " << this->name << "] @ " << dst.w << "x" << dst.h << "+"
		<<  dst.x << "x" << dst.y << ", th " << this->rotate << ", "
		<< "cen " << center.x << "x" << center.y << endl;
#endif
	if(SDL_RenderCopyEx(renderer, this->tex, NULL, 
		&dst, this->rotate, &center, SDL_FLIP_NONE) < 0)
	{
		std::cout << "Couldn't render sprite '" << this->name << "':" 
			<< SDL_GetError() << endl;
		SwitchState(ST_EXIT);
	}
}

Text::Text(){};
Text::Text(char *text, TTF_Font *font)
{
	strncpy(Text::text, text, 1024);
	Text::font = font;
	Text::color = (SDL_Color){ 255, 255, 255, 255 };	//white by default
	Text::rotate = 0;
	Text::scale = Vector2(1,1);
	Text::translate = Vector2(0,0);
}

void Text::Render(SDL_Renderer *rend)
{
	//first render the text to a texture
	SDL_Surface *tmpSfc = TTF_RenderText_Blended(this->font, 
		this->text, this->color);
	if(!tmpSfc)
	{
		std::cout << "Couldn't TTF_RenderText_Blended: " <<
			TTF_GetError() << endl;
	}
	this->tex = SDL_CreateTextureFromSurface(rend, tmpSfc);
	if(!this->tex)
	{
		std::cout << "Couldn't SDL_CreateTextureFromSurface: " <<
			SDL_GetError() << endl;
	}
	SDL_QueryTexture(tex, NULL, NULL, &this->w, &this->h);
	SDL_Rect dst;
	SDL_Point center;
	dst.x = this->translate.x;
	dst.y = this->translate.y;
	dst.w = this->w * this->scale.x;
	dst.h = this->h * this->scale.y;
	center.x = this->w / 2;
	center.y = this->h / 2;
#ifdef DEBUG_MORE
	cout << "[R " << this->text << "] @ " << dst.w << "x" << dst.h << "+"
		<<  dst.x << "x" << dst.y << ", th " << this->rotate << ", "
		<< "cen " << center.x << "x" << center.y << endl;
#endif
	if(SDL_RenderCopyEx(rend, this->tex, NULL, 
		&dst, this->rotate, &center, SDL_FLIP_NONE) < 0)
	{
		std::cout << "Couldn't render text '" << this->text << "':" 
			<< SDL_GetError() << endl;
		SwitchState(ST_EXIT);
	}		
}

void Text::Rotate(float angle)
{
	rotate += angle;
}

void Text::Translate(Vector2 t)
{
	this->translate = this->translate + t;
}

void Text::Scale(Vector2 s)
{
	this->scale = s;
}

void Text::SetColor(SDL_Color c)
{
	this->color = c;
}

void SetRenderCallback(void (*callback)())
{
	RenderCallback = callback;
}

