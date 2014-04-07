#include "state.h"
#include <SDL2/SDL.h>

int currentState = 0;

void SwitchState(int state)
{
	currentState = state; 
}
