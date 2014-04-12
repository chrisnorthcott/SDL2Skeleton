#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "graphics.h"
#include "rapidxml.h"

class Tile
{
public:
	Tile();
	Tile(int, int, int);
	int x, y, gid;
};

class TMXMap : public IRenderable
{
public:
	TMXMap();
	TMXMap(const char * filename);
	~TMXMap();
	void SetOffset(int offsetX, int offsetY);
	void Scroll(int offsetX, int offsetY);
	void PreRender(SDL_Renderer *rend);
	bool IsPreRendered();
	bool IsLoaded();
private:
	SDL_Surface *getTile(int gid);
	SDL_Surface *mapSfc, *mapViewport;
	SpriteSheet tileset;
	std::vector<Tile> tiles;
	int mapW, mapH, tileW, tileH, offsetX, offsetY, vpW, vpH;
	bool prerendered, loaded;
};