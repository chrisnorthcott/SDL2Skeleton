#include "state.h"
#include "globals.h"

int currentState = 0;

void SwitchState(int state)
{
	currentState = state;
}
