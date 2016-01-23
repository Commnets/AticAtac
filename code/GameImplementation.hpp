/** \addtogroup Atic */
/*@{*/

#ifndef __GAMEIMPLEMENTATION_HPP__
#define __GAMEIMPLEMENTATION_HPP__

#include <SDL/sdlgame.hpp>

class GameImplementation : public SDLGame
{
	public:
	GameImplementation ()
		: SDLGame ()
							{ }

	virtual QGAMES::InputHandler* createInputHandler ();
};


#endif
  
// End of the file
/*@}*/