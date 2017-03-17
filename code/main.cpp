#include "stdafx.h"
#include "Game.hpp"

#ifndef _CONSOLE
#include <SDL.h>
#ifdef __cplusplus
#define C_LINKAGE "C"
#else
#define C_LINKAGE
#endif /* __cplusplus */
extern C_LINKAGE int main (int argc, char *argv [])
#else
int _tmain (int argc, char *argv [])
#endif /* _CONSOLE */
{
	// Initialize the generic data...
	General::_e.initialize ();

	AticAtacGame game;
	game.setLinkDrawToFrameRate (true); 
	// The normal thing is to draw at the same pace than the frame rate...
	game.setPreLoad (true); 
	// The normal thing is to preload all componenets...
	game.exec ();
	return (0);
}