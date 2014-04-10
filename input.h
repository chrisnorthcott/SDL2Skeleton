#pragma once
#include <vector>
#include <SDL2/SDL.h>

#define DEADZONE		1000

typedef struct EVENTMAPPING
{
	char eventName[50];
	void (*callback)();
} EVENTMAPPING;

typedef struct CONTROLMAPPING
{
	char lookupKey[50];
	char eventName[50];
} CONTROLMAPPING;

class ControllerMap
{
public:
	ControllerMap();
	ControllerMap(const char *configFile);
	void ReloadKeymap();
	void Disengage();
	void Engage();
	void DispatchEvents();
	void AddHook(const char *eventName, void (*callback)());
private:
	SDL_Joystick *joydev;
	bool engaged;
	char configFile[1024];
	std::vector<EVENTMAPPING> events;		//programmed in-state via AddHook
	std::vector<CONTROLMAPPING> controls;	//loaded from config
};