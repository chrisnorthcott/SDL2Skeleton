#include "map.h"
#include <sstream>
#include <fstream>

// class TMXMap: public IRenderable
// {
// public:
// 	TMXMap(char * filename);
// 	void SetOffset(int offsetX, int offsetY);
// 	bool PreRender();
// private:
// 	XMLDocument doc;
// 	std::vector<Tile> tiles;
// 	int mapW, mapH, tileW, tileH, offsetX, offsetY;
// 	bool rendered;
// };

Tile::Tile(){};

Tile::Tile(int x, int y, int gid)
{
	this->x = x;
	this->y = y;
	this->gid = gid;
}

TMXMap::TMXMap(){};

void TMXMap::PreRender(SDL_Renderer *rend)
{
	using namespace std;

	//Given a set offset, render the portion of the map
	//visible in the player's viewport.
	//BEAR IN MIND:
	//	-- offset is ground biased, ie y axis is flipped.

	//grab the source rectangle for the map
	SDL_Rect srcRect = {
		this->offsetX,
		(this->mapSfc->h - this->vpH) - this->offsetY,
		this->vpW, this->vpH
	};

	//and blit it to a surface, ready for RTT
	if(SDL_BlitSurface(this->mapSfc, &srcRect, this->mapViewport, NULL) < 0)
	{
		cout << "Failed to pre-render map!" << endl;
		SwitchState(ST_EXIT);
		return;			
	}

	//give renderer info it needs
	this->w = this->vpW;
	this->h = this->vpH;
	this->rotate = 0.0f;
	this->scale = Vector2(1,1);
	this->translate = Vector2(0,0);

	//FINALLY, do the RTT!
	SDL_DestroyTexture(this->tex);

	this->tex = SDL_CreateTextureFromSurface(rend, this->mapViewport);
	this->prerendered = true;

	/*
		you should now be able to Render() the map!
	*/

}

TMXMap::~TMXMap()
{
	//
}

bool TMXMap::IsPreRendered()
{
	return this->prerendered;
}
bool TMXMap::IsLoaded()
{
	return this->loaded;
}

SDL_Surface *TMXMap::getTile(int gid)
{
	this->tileset.SelectFrame(gid);
	return this->tileset.getCurrentFrame();
}

void TMXMap::SetOffset(int offsetX, int offsetY)
{
	this->offsetX = offsetX;
	this->offsetY = offsetY;
}

void TMXMap::Scroll(int offsetX, int offsetY)
{
	if(this->offsetX >= 0)			//prevent scrolling off beginning of map
		this->offsetX += offsetX;
	if(this->offsetY >= 0)			//ditto top
		this->offsetY += offsetY;

	//TODO: stop scrolling off end of viewport
}

TMXMap::TMXMap(const char *filename)
{
	using namespace std;

	//first get viewport
	SDL_DisplayMode current;
	if(SDL_GetCurrentDisplayMode(0, &current) != 0)
	{
		cout << "Can't render map, don't know viewport size." << endl;
		SwitchState(ST_EXIT);
		return;		
	}
	this->vpW = current.w;
	this->vpH = current.h;
	this->mapViewport = SDL_CreateRGBSurface(0,
		this->vpW, this->vpH,
		32, 
		//0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
		0x000000FF,0x0000FF00,0x00FF0000,0x00000000
	);	

	this->tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,
		SDL_TEXTUREACCESS_STATIC, this->vpW, this->vpH);

	if(string(filename) == string(""))
	{
		cout << "TMXMap: No map file specified." << endl;
		SwitchState(ST_EXIT);
		return;
	}
	rapidxml::xml_document<> x;

	//load the file
	ifstream in(filename);
	stringstream ss;
	ss << in.rdbuf();
	if(in.fail())
	{
		cout << "TMXMap load failed: " << strerror(errno) << endl;
		SwitchState(ST_EXIT);
		return;
	}
	in.close();
	string xmlBuf(ss.str());

	cout << "Loaded map " << filename << ", " << xmlBuf.size() 
		<< " bytes" << endl;

	//load and validate
	std::vector<char> wtf;
	copy(xmlBuf.begin(), xmlBuf.end(), back_inserter(wtf));
	wtf.push_back('\0');
	x.parse<0>(&wtf[0]);

	if(string(x.first_node()->name()) != string("map"))
	{
		cout << "Couldn't find <map>: this is not a valid TMX map." << endl;
		SwitchState(ST_EXIT);
		return;
	}else{
		cout << "Found TMX <map> element, loading metadata..." << endl;
	}

	//check this is an orthogonal map, we don't support iso maps (yet)
	if(string(x.first_node()->first_attribute("orientation")->value())
		!= string("orthogonal"))
	{
		cout << "Map must use orthogonal orientation!" << endl;
		SwitchState(ST_EXIT);
		return;
	}

	//set default offset
	this->offsetX = this->offsetY = 0;

	//load metadata from <map>
	this->mapW = atoi(x.first_node()->first_attribute("width")->value());
	this->mapH = atoi(x.first_node()->first_attribute("height")->value());
	this->tileW = atoi(x.first_node()->first_attribute("tilewidth")->value());
	this->tileH = atoi(x.first_node()->first_attribute("tileheight")->value());

	//get tileset image stats while we're here
	rapidxml::xml_node<> *tilesetNode =
		x.first_node()->first_node("tileset")->first_node("image");

	string tilesetSource(tilesetNode->first_attribute("source")->value());
	int tilesetWidth = atoi(tilesetNode->first_attribute("width")->value());
	int tilesetHeight = atoi(tilesetNode->first_attribute("height")->value());

	if(this->mapW && this->mapH && this->tileW && this->tileH)
	{
		cout << "Map is " << this->mapW << "x" << this->mapH << " tiles in size" << endl;
		cout << "Tiles are " << this->tileW << "x" << this->tileH << " pixels" << endl;
		cout << "Map total surface is " << this->tileW * this->mapW << "x" 
			<< this->tileH * this->mapH << " pixels" << endl;
	}else{
		cout << "Malformed map, couldn't read metadata." << endl;
		SwitchState(ST_EXIT);
		return;		
	}

	int n, tx, ty = 0;

	//Now, load the graphic layer
	//this is in <layer><data>...</></>
	rapidxml::xml_node<> *graphicLayer = 
		x.first_node()->first_node("layer")->first_node("data");
	//iterate over all children
	for(rapidxml::xml_node<> *child = graphicLayer->first_node("tile");
			child;
			child = child->next_sibling()){
		tiles.push_back(
			Tile(
				tx, ty,
				atoi(child->first_attribute("gid")->value())
			)
		);
		if(++tx > (this->mapW - 1)){
 			tx = 0;
 			ty++;			
		}
		n++;
	}
	if(n == (this->mapW * this->mapH)){
		cout << "graphicLayer successfully read " << 
			n << " tiles." << endl;
	}

	this->loaded = true;

	//load the tileset, now we know the metadata.
	//we also need to find the total number of tiles
	//in the tileset
	int nTiles = (tilesetWidth / this->tileW) * (tilesetHeight / this->tileH);
	this->tileset = SpriteSheet(tilesetSource.c_str(),
		Point(this->tileW, this->tileH), nTiles);

	//construct the map itself
	cout << "Constructing map..." << endl;
	this->mapSfc = SDL_CreateRGBSurface(0,
		this->tileW * this->mapW, 
		this->tileH * this->mapH,
		32, 
		0, 0, 0, 0
		//0xFF000000,0x00FF0000,0x0000FF00,0x000000FF
		//0x000000FF,0x0000FF00,0x00FF0000,0xFF000000
	);

	int i = 0, len = this->tiles.size();
	for(i = 0; i < len; i++)
	{
		SDL_Rect dst = {
			this->tiles[i].x * this->tileW,
			this->tiles[i].y * this->tileH,
			this->tileW, this->tileH			
		};
		int gid = this->tiles[i].gid;

		if(SDL_BlitSurface(this->getTile(gid), NULL, this->mapSfc, &dst) < 0)
		{
			std::cout << "Failed to SDL_BlitSurface" << endl;
			SwitchState(ST_EXIT);
		}

	}

	//TODO: collision layer, goo layer, ...
}