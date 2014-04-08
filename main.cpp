
#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "math.h"
#include "state.h"
#include "graphics.h"
#include "errors.h"

using namespace std;

#define HARNESS_N 1500

Sprite spr[HARNESS_N];
float angle;
TTF_Font *defaultFont;

bool errorState;
int errorCode = 0;

void ExitState()
{
	SDL_Quit();
	exit(errorCode);
}

void DummyState()
{
	SDL_Delay(10);
}

void DummyRenderCallback()
{
	//render code goes here
}

void InitState()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "Oh no! SDL startup failed, something is mad wrong." << endl;
		errorState = true;
		SwitchState(ST_EXIT);
	}
	if(GraphicsInit() == false)
	{
		errorState = true;
		SwitchState(ST_EXIT);
	}
	//Load default font
	defaultFont = TTF_OpenFont(FONT_DEFAULT, 16);
	if(defaultFont == NULL)
		cout << "Warning: default font missing";
	if(!errorState)
	{
		cout << "Init OK, proceeding" << endl;
		SwitchState(ST_DUMMY);
	}

}

STATEDEF states[4] = {
	{"None"				, false , NULL, NULL, NULL},
	{"Initialisation"	, false , NULL, InitState, DummyRenderCallback },
	{"Shutdown"			, false , NULL, ExitState, DummyRenderCallback },
	{"Dummy State"		, false , NULL, DummyState, DummyRenderCallback }
};

int main(int argc, char **argv)
{
	SwitchState(ST_INIT);
#ifdef DEBUG
	long nFrames = 1;
	float avgFPS = 0.0f;
	stringstream fps;
	Text fpsText;
#endif
	while(currentState != ST_EXIT)
	{
		//if state is not initialised initialise it, if it has an initcallback
		if(!states[currentState].initialised && states[currentState].initCallback != NULL)
		{
			void (*InitCallback)() = states[currentState].initCallback;
			InitCallback();
			states[currentState].initialised = true;
		}
		//MainLoop(): deal with logic, AI, physics etc for each frame
		void (*MainLoop)() = states[currentState].mainCallback;
		if(MainLoop == NULL)
		{
			cout << "ERROR: " << states[currentState].name << " state does not have a mainCallback";
			SwitchState(ST_EXIT);
		}
		MainLoop();

		//we deal with graphics seperately
		ClearScreen(0,0,0,255);
		void (*RenderCallback)() = states[currentState].renderCallback;
		if(RenderCallback == NULL)
		{
			cout << "ERROR: " << states[currentState].name << " state does not have a renderCallback";
			SwitchState(ST_EXIT);
		}
		RenderCallback();
#ifdef DEBUG
		//<- end of last frame
		int t = SDL_GetTicks();
		avgFPS = ++nFrames / (t / 1000.0f);
		stringstream fps;
		fps.str("");
		fps << "FPS: " << std::setprecision(3) << avgFPS;
		if(avgFPS < 20.0f)
			fpsText = Text(fps.str().c_str(), defaultFont, {255, 0, 0, 255});
		else
			fpsText = Text(fps.str().c_str(), defaultFont, {255, 255, 255, 255});		
		fpsText.Render(renderer);
#endif

		SDL_RenderPresent(renderer);
		if(errorState){
			cout << "An error has occured, code " << errorCode 
				<< endl;
			SwitchState(ST_EXIT);
		}
		if(SDL_QuitRequested())
		{
			cout << "Quit requested, exiting." << endl;	
			SwitchState(ST_EXIT);
		}
	}
}
