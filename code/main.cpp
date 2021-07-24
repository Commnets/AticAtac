#include "stdafx.h"
#include "Game.hpp"

#ifndef _CONSOLE
#include <SDL.h>
#ifdef __cplusplus
#define C_LINKAGE "C"
#else
#define C_LINKAGE
#endif /* __cplusplus */
#if _MSC_VER >= 1900
extern C_LINKAGE FILE __iob_func[3] = { *stdin,*stdout,*stderr };
#endif
extern C_LINKAGE int main (int argc, char *argv [])
#else
int _tmain (int argc, char *argv [])
#endif /* _CONSOLE */
{
#ifdef NDEBUG
	// In the release version, the resources are in a zip file
	// This instruction reads them and store in temporaly files when needed!
	// The resource reader is deleted at the end, and all the temporal file with them!
	QGAMES::InZipResourceReader rR 
		(QGAMES::ConfigurationFile (std::string (__GAME_CONFIGURATIONFILE__)).
			property (std::string (__GAME_DATADIRPROPERTYNAME__)));
#endif

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