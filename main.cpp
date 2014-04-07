
#include <SDL2/SDL.h>
#include <iostream>

#include "math.h"
#include "state.h"
#include "graphics.h"
#include "errors.h"


using namespace std;

bool errorState;
int errorCode = 0;

void ExitState()
{
	SDL_Quit();
	exit(errorCode);
}

void DummyState()
{
	SDL_Delay(50);
}

void DummyRenderCallback()
{
	//Just to make sure we're alive here...
	cout << ".";
	ClearScreen(0,0,0,255);
}

void InitState()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		errorState = true;
		errorCode = E_SDL_INIT_FAILED; 
	}
	if(GraphicsInit() == false)
	{
		errorState = true;
		errorCode = E_VIDEO_INIT_FAILED;
	}
	if(!errorState)
	{
		cout << "Init OK, proceeding" << endl;
		SwitchState(ST_DUMMY);
	}
	//throw up a dummy render callback - just clear the screen
	SetRenderCallback(DummyRenderCallback);
}

STATEDEF states[4] = {
	{"None"			, NULL},
	{"Initialisation"	, InitState },
	{"Shutdown"			, ExitState },
	{"Dummy State"		, DummyState }
};

int main(int argc, char **argv)
{
	SwitchState(ST_INIT);
	while(currentState != ST_EXIT)
	{
		//MainLoop(): deal with logic, AI, physics etc for each frame
		void (*MainLoop)() = states[currentState].callback; 
		MainLoop();

		//we deal with graphics seperately
		RenderCallback();
		SDL_RenderPresent(renderer);

		if(errorState){
			cout << "An error has occured, code " << errorCode 
				<< endl;
			SwitchState(ST_EXIT);
		}
		//stuff that happens EVERY frame goes here
		//
		if(SDL_QuitRequested())
		{
			cout << "Quit requested, exiting." << endl;	
			SwitchState(ST_EXIT);
		}
		//
		/////////////////////////////////////////
	}
}
