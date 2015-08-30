#ifndef __INPUTHANDLER_HPP__
#define __INPUTHANDLER_HPP__

#include <SDL/sdlinputhandler.hpp>

class InputHandler : public SDLInputHandler
{
	public:
	InputHandler (QGAMES::Game* g)
		: SDLInputHandler (g),
		  _lastKey (-1)
							{ }

	// To treat when the key are pressed...
	virtual void treatKeyboardStatus (const unsigned __int8* k);

	virtual void onJoystickAxisMoveEvent (QGAMES::JoystickMovementEventData* dt);
	virtual void onJoystickButtonEvent (QGAMES::JoystickButtonEventData* dt);
	virtual void onKeyboardEvent (QGAMES::KeyboardEventData* dt);
	virtual void onMouseMovementEvent (QGAMES::MouseMovementEventData* dt);
	virtual void onMouseButtonEvent (QGAMES::MouseButtonEventData* dt);

	private:
	void manageKeyOnInitialState (int k);
	void manageKeyOnSelectState (int k);
	void manageKeyOnDemoState (int k);
	void manageKeyOnPlaying (int k);
	void manageKeyOnEnd (int k);
	void manageKeyOnWin (int k);

	private:
	// Implementation
	int _lastKey;
};

#endif