#ifndef _STATE_H
#define _STATE_H

#define ST_NONE         0
#define ST_INIT         1
#define ST_EXIT         2 
#define ST_DUMMY	3
//add your own states below
#define ST_SOMETHING	4

void SwitchState(int);

#define APP_TITLE "SDL2Skeleton"

typedef struct STATEDEF
{
        char name[50];
        void (*callback)(void);
} STATEDEF;

extern int 	currentState;
extern int	errorCode;
extern bool	errorState;

#endif
