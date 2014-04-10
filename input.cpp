#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include "math.h"
#include "input.h"

extern Vector2 joymove;
extern Vector2 mousemove;

ControllerMap::ControllerMap(){}

ControllerMap::ControllerMap(const char *configFile)
{
	if(SDL_NumJoysticks() < 1)
	{
		std::cout << "No joysticks/gamepads detected." << std::endl;
	}else{
		this->joydev = SDL_JoystickOpen(0);
		if(this->joydev == NULL)
		{
			std::cout << "WARNING: couldn't open joystick!";
		}
	}
#ifdef DEBUG
	std::cout << "Loading keymap " << configFile << std::endl;
#endif
	strncpy(this->configFile, configFile, 1024);
	this->ReloadKeymap();
	this->Engage();
}

void ControllerMap::Disengage()
{
#ifdef DEBUG
	std::cout << "Input dispatcher disengaged." << std::endl;
#endif
	this->engaged = false;
}
void ControllerMap::Engage()
{
#ifdef DEBUG
	std::cout << "Input dispatcher engaged." << std::endl;
#endif
	this->engaged = true;
}

void ControllerMap::ReloadKeymap()
{
#ifdef DEBUG
	std::cout << "Reloading Keymap..." << std::endl;
#endif
	std::ifstream is;
	std::string line;
	int linenum;
	is.open(this->configFile);
	while(std::getline(is, line))
	{
		linenum++;
		if(line[0] != '#')		//ignore comments
		{
			//Hook MM:Things
			CONTROLMAPPING cm;

			//find where the space is
			std::string::size_type n = line.find(" ");
			if(n == std::string::npos)
			{
				std::cout << "Badly formed config, line " << linenum;
			}else{
				std::string eventName = line.substr(0, n);
				std::string controlName = line.substr(n + 1, std::string::npos);

				strncpy(cm.eventName, eventName.c_str(), 50);
				strncpy(cm.lookupKey, controlName.c_str(), 50);
			}
			std::cout << "Loaded keymap " << cm.eventName <<
				"=" << cm.lookupKey << std::endl;
			this->controls.push_back(cm);
		}
	}
}

void ControllerMap::AddHook(const char *eventName, void (*callback)())
{
	// EVENTMAPPING em = {eventName, callback};
	if(!callback || strlen(eventName) < 1)
	{
		std::cout << "Malformed control hook, not using it." 
			<< std::endl;
	}
	EVENTMAPPING em;
	strncpy(em.eventName, eventName, 50);
	em.callback = callback;
	std::cout << "Loaded hook " << eventName << std::endl;
	this->events.push_back(em);
}

void ControllerMap::DispatchEvents()
{
	//no point processing events if we're locked out
	if(!this->engaged)
		return;
	//first, construct our lookup key
	SDL_Event e;
	std::stringstream lookupKey;
	lookupKey.str("");
	while(SDL_PollEvent(&e) != 0)
	{
		switch(e.type)
		{
			/* I apologise profusely for the fing enormous
				switch-case that is about to follow.*/

			case SDL_KEYDOWN:
			{
				lookupKey << "KB:";
				switch(e.key.keysym.sym)
				{
					case SDLK_BACKSPACE: lookupKey << "backspace"; break;
					case SDLK_TAB: lookupKey << "tab"; break;
					case SDLK_CLEAR: lookupKey << "clear"; break;
					case SDLK_RETURN: lookupKey << "return"; break;
					case SDLK_PAUSE: lookupKey << "pause"; break;
					case SDLK_ESCAPE: lookupKey << "escape"; break;
					case SDLK_SPACE: lookupKey << "space"; break;
					case SDLK_EXCLAIM: lookupKey << "exclaim"; break;
					case SDLK_QUOTEDBL: lookupKey << "quotedbl"; break;
					case SDLK_HASH: lookupKey << "hash"; break;
					case SDLK_DOLLAR: lookupKey << "dollar"; break;
					case SDLK_AMPERSAND: lookupKey << "ampersand"; break;
					case SDLK_QUOTE: lookupKey << "quote"; break;
					case SDLK_LEFTPAREN: lookupKey << "leftparen"; break;
					case SDLK_RIGHTPAREN: lookupKey << "rightparen"; break;
					case SDLK_ASTERISK: lookupKey << "asterisk"; break;
					case SDLK_PLUS: lookupKey << "plus"; break;
					case SDLK_COMMA: lookupKey << "comma"; break;
					case SDLK_MINUS: lookupKey << "minus"; break;
					case SDLK_PERIOD: lookupKey << "period"; break;
					case SDLK_SLASH: lookupKey << "slash"; break;
					case SDLK_0: lookupKey << "0"; break;
					case SDLK_1: lookupKey << "1"; break;
					case SDLK_2: lookupKey << "2"; break;
					case SDLK_3: lookupKey << "3"; break;
					case SDLK_4: lookupKey << "4"; break;
					case SDLK_5: lookupKey << "5"; break;
					case SDLK_6: lookupKey << "6"; break;
					case SDLK_7: lookupKey << "7"; break;
					case SDLK_8: lookupKey << "8"; break;
					case SDLK_9: lookupKey << "9"; break;
					case SDLK_COLON: lookupKey << "colon"; break;
					case SDLK_SEMICOLON: lookupKey << "semicolon"; break;
					case SDLK_LESS: lookupKey << "less"; break;
					case SDLK_EQUALS: lookupKey << "equals"; break;
					case SDLK_GREATER: lookupKey << "greater"; break;
					case SDLK_QUESTION: lookupKey << "question"; break;
					case SDLK_AT: lookupKey << "at"; break;
					case SDLK_LEFTBRACKET: lookupKey << "leftbracket"; break;
					case SDLK_BACKSLASH: lookupKey << "backslash"; break;
					case SDLK_RIGHTBRACKET: lookupKey << "rightbracket"; break;
					case SDLK_CARET: lookupKey << "caret"; break;
					case SDLK_UNDERSCORE: lookupKey << "underscore"; break;
					case SDLK_BACKQUOTE: lookupKey << "backquote"; break;
					case SDLK_a: lookupKey << "a"; break;
					case SDLK_b: lookupKey << "b"; break;
					case SDLK_c: lookupKey << "c"; break;
					case SDLK_d: lookupKey << "d"; break;
					case SDLK_e: lookupKey << "e"; break;
					case SDLK_f: lookupKey << "f"; break;
					case SDLK_g: lookupKey << "g"; break;
					case SDLK_h: lookupKey << "h"; break;
					case SDLK_i: lookupKey << "i"; break;
					case SDLK_j: lookupKey << "j"; break;
					case SDLK_k: lookupKey << "k"; break;
					case SDLK_l: lookupKey << "l"; break;
					case SDLK_m: lookupKey << "m"; break;
					case SDLK_n: lookupKey << "n"; break;
					case SDLK_o: lookupKey << "o"; break;
					case SDLK_p: lookupKey << "p"; break;
					case SDLK_q: lookupKey << "q"; break;
					case SDLK_r: lookupKey << "r"; break;
					case SDLK_s: lookupKey << "s"; break;
					case SDLK_t: lookupKey << "t"; break;
					case SDLK_u: lookupKey << "u"; break;
					case SDLK_v: lookupKey << "v"; break;
					case SDLK_w: lookupKey << "w"; break;
					case SDLK_x: lookupKey << "x"; break;
					case SDLK_y: lookupKey << "y"; break;
					case SDLK_z: lookupKey << "z"; break;
					case SDLK_DELETE: lookupKey << "delete"; break;
					case SDLK_KP_0: lookupKey << "kp0"; break;
					case SDLK_KP_1: lookupKey << "kp1"; break;
					case SDLK_KP_2: lookupKey << "kp2"; break;
					case SDLK_KP_3: lookupKey << "kp3"; break;
					case SDLK_KP_4: lookupKey << "kp4"; break;
					case SDLK_KP_5: lookupKey << "kp5"; break;
					case SDLK_KP_6: lookupKey << "kp6"; break;
					case SDLK_KP_7: lookupKey << "kp7"; break;
					case SDLK_KP_8: lookupKey << "kp8"; break;
					case SDLK_KP_9: lookupKey << "kp9"; break;
					case SDLK_KP_PERIOD: lookupKey << "kp_period"; break;
					case SDLK_KP_DIVIDE: lookupKey << "kp_divide"; break;
					case SDLK_KP_MULTIPLY: lookupKey << "kp_multiply"; break;
					case SDLK_KP_MINUS: lookupKey << "kp_minus"; break;
					case SDLK_KP_PLUS: lookupKey << "kp_plus"; break;
					case SDLK_KP_ENTER: lookupKey << "kp_enter"; break;
					case SDLK_KP_EQUALS: lookupKey << "kp_equals"; break;
					case SDLK_UP: lookupKey << "up"; break;
					case SDLK_DOWN: lookupKey << "down"; break;
					case SDLK_RIGHT: lookupKey << "right"; break;
					case SDLK_LEFT: lookupKey << "left"; break;
					case SDLK_INSERT: lookupKey << "insert"; break;
					case SDLK_HOME: lookupKey << "home"; break;
					case SDLK_END: lookupKey << "end"; break;
					case SDLK_PAGEUP: lookupKey << "pageup"; break;
					case SDLK_PAGEDOWN: lookupKey << "pagedown"; break;
					case SDLK_F1: lookupKey << "f1"; break;
					case SDLK_F2: lookupKey << "f2"; break;
					case SDLK_F3: lookupKey << "f3"; break;
					case SDLK_F4: lookupKey << "f4"; break;
					case SDLK_F5: lookupKey << "f5"; break;
					case SDLK_F6: lookupKey << "f6"; break;
					case SDLK_F7: lookupKey << "f7"; break;
					case SDLK_F8: lookupKey << "f8"; break;
					case SDLK_F9: lookupKey << "f9"; break;
					case SDLK_F10: lookupKey << "f10"; break;
					case SDLK_F11: lookupKey << "f11"; break;
					case SDLK_F12: lookupKey << "f12"; break;
					case SDLK_F13: lookupKey << "f13"; break;
					case SDLK_F14: lookupKey << "f14"; break;
					case SDLK_F15: lookupKey << "f15"; break;
					case SDLK_CAPSLOCK: lookupKey << "capslock"; break;
					case SDLK_RSHIFT: lookupKey << "rshift"; break;
					case SDLK_LSHIFT: lookupKey << "lshift"; break;
					case SDLK_RCTRL: lookupKey << "rctrl"; break;
					case SDLK_LCTRL: lookupKey << "lctrl"; break;
					case SDLK_RALT: lookupKey << "ralt"; break;
					case SDLK_LALT: lookupKey << "lalt"; break;
					case SDLK_MODE: lookupKey << "mode"; break;
					case SDLK_HELP: lookupKey << "help"; break;
					case SDLK_SYSREQ: lookupKey << "sysreq"; break;			
				}
			}break;
			case SDL_MOUSEMOTION:
			{
				lookupKey << "MA:";
				//TODO: HTF are we meant to do mouse axes with this system?!
				//tip: use SDL_GetRelativeMouseState
			}break;
			case SDL_MOUSEBUTTONDOWN:
			{
				lookupKey << "MB:";
				int i;
				for(i = 1; i < 4; i++)
					if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(i))
						lookupKey << i;
			}break;
			case SDL_JOYAXISMOTION:
			{
				lookupKey << "JA:" << (int) e.jaxis.which;
				if(e.jaxis.value < DEADZONE)
				{
					lookupKey << "-";
				}else if(e.jaxis.value > DEADZONE){
					lookupKey << "+";
				}
			}break;
			case SDL_JOYHATMOTION:
			{
				lookupKey << "JH:";
				switch(e.jhat.value)
				{
					case SDL_HAT_UP: 		lookupKey << "up"; break;
					case SDL_HAT_RIGHTUP:	lookupKey << "rightup"; break;
					case SDL_HAT_RIGHT:		lookupKey << "right"; break;
					case SDL_HAT_RIGHTDOWN:	lookupKey << "rightdown"; break;
					case SDL_HAT_DOWN:		lookupKey << "down"; break;
					case SDL_HAT_LEFTDOWN:	lookupKey << "downleft"; break;
					case SDL_HAT_LEFT:		lookupKey << "left"; break;
					case SDL_HAT_LEFTUP:	lookupKey << "leftup"; break;
				}
			}break;
			case SDL_JOYBUTTONDOWN:
			{
				lookupKey << "JB:" << (short)e.jbutton.button;
			}break;
		}
		if(lookupKey.str().size() != 0){
#ifdef DEBUG
			std::cout << "Input dispatcher: ";
			std::cout << lookupKey.str() << std::endl;
#endif
			//now find the EVENTMAPPING corresponding with lookupKey
			//and if found, fire its event!
			int len = this->controls.size();
			int nEvents = this->events.size();
			int i;
			for(i = 0; i < len; i++)
			{
				std::string controlName = std::string(this->controls[i].lookupKey);
				std::string controlEventName = std::string(this->controls[i].eventName);
				if(lookupKey.str().compare(controlName) == 0)
				{
					//we got a hit, now find its corresponding event callback
					for(i = 0; i < nEvents; i++)
					{
						std::string eventName = std::string(this->events[i].eventName);
						if(controlEventName.compare(eventName) == 0)
						{
							//woot, we got it!
							void (*EventCallback)() = this->events[i].callback;
							if(EventCallback != NULL)
							{
								EventCallback();
							}
						}
					}
				}
			}
		}
	}
}