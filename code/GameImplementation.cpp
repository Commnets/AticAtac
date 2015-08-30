#include "GameImplementation.hpp"
#include "InputHandler.hpp"

// ---
QGAMES::InputHandler* GameImplementation::createInputHandler ()
{ 
	return (new InputHandler (_game)); 
}
