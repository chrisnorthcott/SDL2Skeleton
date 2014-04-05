
#include <SDL2/SDL.h>
#include <iostream>

#include "state.h"
#include "graphics.h"
#include "errors.h"

using namespace std;

bool errorState;
int errorCode = 0;

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
}

void ExitState()
{
	SDL_Quit();
	exit(errorCode);
}

void DummyState()
{
	ClearScreen(0,0,0,255);
	SDL_Delay(50);
}

STATEDEF states[4] = {
	{"None"			, NULL},
	{"Initialisation"	, InitState },
	{"Shutdown"		, ExitState },
	{"Dummy State"		, DummyState }
};

int main(int argc, char **argv)
{
	SwitchState(ST_INIT);
	while(currentState != ST_EXIT)
	{
		void (*MainLoop)() = states[currentState].callback; 
		MainLoop();
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
