#include "stdafx.h"
#include "Game.hpp"

// ---
int _tmain (int argc, _TCHAR* argv[])
{
	// The rectangles are simple...
	QGAMES::Rectangle::_basic2DRectangle = true;

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