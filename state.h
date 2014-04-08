#ifndef _STATE_H
#define _STATE_H

enum STATES {
	ST_NONE,
	ST_INIT,
	ST_EXIT,
	ST_DUMMY,
	//add your own states below
	ST_SOMETHING
};

void SwitchState(int);

#define APP_TITLE "SDL2Skeleton"

typedef struct STATEDEF
{
        char name[50];
        bool initialised;
        void (*initCallback)(void);
        void (*renderCallback)(void);
        void (*mainCallback)(void);
} STATEDEF;

extern int 	currentState;
extern int	errorCode;
extern bool	errorState;

#endif
