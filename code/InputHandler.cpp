#include "InputHandler.hpp"
#include "GameStates.hpp"
#include "Game.hpp"
#include "Entities.hpp"
#include "Defs.hpp"
#include <iostream>

// ---
void InputHandler::treatKeyboardStatus (const unsigned __int8* k)
{
	if (_game -> activeState () -> type () == __GAMESTATEPLAYING)
	{
		// If the device to control the main character is the joystick, nothing to do here...
		// Keyboard and joystick can not be active at the same time when playing
		// The reason is, no action in any of them generates a no movement of the main character
		// This method is call after processing any event, including keyboard events
		// So, if the joystick is being used to move the character, and a movement is generated
		// when the system enters this method, and the keyboard is not in use, the movement finishes...
		if (_joystick)
			return; 

		int dX = 0; int dY = 0;
		if (k [SDL_SCANCODE_S]) dX = 1;
		if (k [SDL_SCANCODE_A]) dX = -1;
		if (k [SDL_SCANCODE_L]) dY = 1;
		if (k [SDL_SCANCODE_P]) dY = -1;
		((AticAtacWorld*) (((AticAtacGame*) _game) -> activeWorld ())) 
			-> moveCharacter (QGAMES::Vector (__BD dX, __BD dY, __BD 0));
		// If no key is pressed the character stops!
	}
}

// ---
void InputHandler::onJoystickAxisMoveEvent (QGAMES::JoystickMovementEventData* dt)
{
	// Take into account that if the joystick is not moving
	// this method will not be called, so there must be an exit option programmed
	// to stop the main character when the joystick is not being moved!

	// Only one joystick available...
	if (dt -> _numberJoystick == 0) 
	{
		if (_game -> activeState () -> type () == __GAMESTATEPLAYING)
		{
			// If the device to control the character is not joystick when playing, nothing to do here...
			// Keyboard and joystick can not be active at the same time when playing
			// The reason is, no action in any of them generates a no movement of the main character
			// This method is call after processing any event, including keyboard events
			// So, if the joystick is being used to move the character, and a movement is generated
			// when the system enters this method, and the keyboard is not in use, the movement finishes...
			if (!_joystick)
				return; 

			// Notice that the movemnt is always incremental respect the previous one...
			// The joysticks points a position in the space. When move to right (i.e) 
			// the position starts to move towrds max position in the X. 
			// When the joystick is released, the position moves back to 0
			// If the joystick is maintained at the right, the position doesn't change.
			// This behaviour is used to determinate the direction of the main character
			// When is is the right and bigger than a fouth of the max position the position is 
			// incremented in one, and reduce in one if it isn't.
			// The x axis is the number 0
			if (dt -> _numberAxis == 0) 
			{
				if (dt -> _deadLine > 0)
				{
					if (dt -> _deadLine > (maxPosJoystick (dt -> _numberAxis) / 4))
						_mX += (_mX < 1) ? 1 : 0;
					else
						_mX -= (_mX > 0) ? 1 : 0;
				}
				else
				if (dt -> _deadLine < 0)
				{
					if (dt -> _deadLine < (minPosJoystick (dt -> _numberAxis) / 4))
						_mX += (_mX > -1) ? -1 : 0;
					else
						_mX -= (_mX < 0) ? -1 : 0;
				}
			}
			else
			// The y axis is the number 1
			if (dt -> _numberAxis == 1) 
			{
				if (dt -> _deadLine > 0)
				{
					if (dt -> _deadLine > (maxPosJoystick (dt -> _numberAxis) / 4))
						_mY += (_mY < 1) ? 1 : 0;
					else
						_mY -= (_mY > 0) ? 1 : 0;
				}
				else
				if (dt -> _deadLine < 0)
				{
					if (dt -> _deadLine < (minPosJoystick (dt -> _numberAxis) / 4))
						_mY += (_mY > -1) ? -1 : 0;
					else
						_mY -= (_mY < 0) ? -1 : 0;
				}
			}

			// Moves the main character...
			((AticAtacWorld*) (((AticAtacGame*) _game) -> activeWorld ())) -> 
				moveCharacter (QGAMES::Vector (__BD _mX, __BD _mY, __BD 0));
		}
		else
		if (_game -> activeState () -> type () == __GAMESTATESELECT)
		{
			if (dt -> _numberAxis == 1) // Moving in the y axis to select...
			{
				// The system will move focust to next or previous position
				// Just if is not already changing! (see dcalaration of AticAtacGameStateSelect class.
				// Joysticks don't work like in old times!
				// Now they return the position int the axis of a "virtual" cursor being move 
				// with the joystick. This axis
				AticAtacGameStateSelect* st = (AticAtacGameStateSelect*) _game -> activeState ();
				if (dt -> _deadLine > (maxPosJoystick (dt -> _numberAxis) / 2)) st -> nextOption ();
				else if (dt -> _deadLine < (minPosJoystick (dt -> _numberAxis) / 2)) st -> previousOption ();
			}
		}
	}
}

// ---
void InputHandler::onJoystickButtonEvent (QGAMES::JoystickButtonEventData* dt)
{
	// Only one joystick can be active at the same time,
	// and the different actions make only sense when the button is releasd
	if (dt -> _numberJoystick == 0 && !dt -> _on)
	{
		if (_game -> activeState () -> type () == __GAMESTATEPLAYING)
		{
			// If the device to control the character is not joystick 
			// when playing, nothing to do here...
			if (!_joystick)
				return; 

			// Normally the fire button...
			if (dt -> _numberButton == 0) 
				((AticAtacGame*) _game) -> shoot ();
			else
			// Normally the frontal button...
			if (dt -> _numberButton == 1) 
				((AticAtacGame*) _game) -> setWantToCatch ();
		}
		else
		if (_game -> activeState () -> type () == __GAMESTATESELECT && dt -> _numberButton == 0)
			((AticAtacGameStateSelect*) _game -> activeState ()) -> optionSelected ();
		else
		if (_game -> activeState () -> type () == __GAMESTATEINITIAL && dt -> _numberButton == 0)
			((AticAtacGameStateInitial*) _game -> activeState ()) -> setWantToExit (true);
		else
		if (_game -> activeState () -> type () == __GAMESTATEDEMO && dt -> _numberButton == 0)
			((AticAtacGameStateDemo*) _game -> activeState ()) -> setWantToExit (true);
		else
		if (_game -> activeState () -> type () == __GAMESTATEEND && dt -> _numberButton == 0)
			((AticAtacGameStateEnd*) _game -> activeState ()) -> setWantToExit (true);
		else
		if (_game -> activeState () -> type () == __GAMESTATEWIN && dt -> _numberButton == 0)
			((AticAtacGameStateWin*) _game -> activeState ()) -> setWantToExit (true);
	}
}

// ---
void InputHandler::onKeyboardEvent (QGAMES::KeyboardEventData* dt)
{
	int kPressed = -1;
	bool pKey = false;
	if (_lastKey != -1)
	{
		if (!dt -> _on)
		{
			kPressed = _lastKey;
			_lastKey = -1;
			pKey = true;
		}
	}
	else
		_lastKey = dt -> _key;

	if (pKey)
	{
		int gS = _game -> activeState () -> type ();
		switch (gS)
		{
			case __GAMESTATEINITIAL:
				manageKeyOnInitialState (kPressed);
				break;

			case __GAMESTATESELECT:
				manageKeyOnSelectState (kPressed);
				break;

			case __GAMESTATEDEMO:
				manageKeyOnDemoState (kPressed);
				break;

			case __GAMESTATEPLAYING:
				manageKeyOnPlaying (kPressed);
				break;

			case __GAMESTATEEND:
				manageKeyOnEnd (kPressed);
				break;

			case __GAMESTATEWIN:
				manageKeyOnWin (kPressed);
				break;

			default:
				break;
		};
	}
}

// ---
void InputHandler::onMouseMovementEvent (QGAMES::MouseMovementEventData* dt)
{
	int gS = _game -> activeState () -> type ();
	switch (gS)
	{
		case __GAMESTATESELECT:
			((AticAtacGameStateSelect*) _game -> activeState ()) -> 
				optionAt (QGAMES::Position (__BD dt -> _x, __BD dt -> _y, __BD 0));
			break;
	}
}

// ---
void InputHandler::onMouseButtonEvent (QGAMES::MouseButtonEventData* dt)
{
	int gS = _game -> activeState () -> type ();
	switch (gS)
	{
		case __GAMESTATEINITIAL:
			if (dt -> _button == SDL_BUTTON_LEFT && !dt -> _on)
				((AticAtacGameStateInitial*) _game -> activeState ()) -> setWantToExit (true);
			break;

		case __GAMESTATEDEMO:
			if (dt -> _button == SDL_BUTTON_LEFT && !dt -> _on)
				((AticAtacGameStateDemo*) _game -> activeState ()) -> setWantToExit (true);
			break;

		case __GAMESTATEEND:
			if (dt -> _button == SDL_BUTTON_LEFT && !dt -> _on)
				((AticAtacGameStateEnd*) _game -> activeState ()) -> setWantToExit (true);
			break;

		case __GAMESTATEWIN:
			if (dt -> _button == SDL_BUTTON_LEFT && !dt -> _on)
				((AticAtacGameStateWin*) _game -> activeState ()) -> setWantToExit (true);
			break;

		case __GAMESTATESELECT:
			if (dt -> _button == SDL_BUTTON_LEFT && !dt -> _on) // To select one option...
				((AticAtacGameStateSelect*) _game -> activeState ()) -> optionSelected ();
			break;

		case __GAMESTATEPLAYING:
			if (dt -> _button == SDL_BUTTON_LEFT && !dt -> _on) // To pause / continue the game...
				_game -> isGamePaused () ? _game -> continueGame () : _game -> pauseGame ();
			break;

		default:
			break;
	}
}

// ---
void InputHandler::manageKeyOnInitialState (int k)
{
	if (k == SDL_SCANCODE_RETURN)
		((AticAtacGameStateInitial*) _game -> activeState ()) -> setWantToExit (true);
}

// ---
void InputHandler::manageKeyOnSelectState (int k)
{
	if (k == SDL_SCANCODE_RETURN)
		((AticAtacGameStateSelect*) _game -> activeState ()) -> optionSelected ();
	else
	if (k == SDL_SCANCODE_DOWN)
		((AticAtacGameStateSelect*) _game -> activeState ()) -> nextOption ();
	else
	if (k == SDL_SCANCODE_UP)
		((AticAtacGameStateSelect*) _game -> activeState ()) -> previousOption ();
}

// ---
void InputHandler::manageKeyOnDemoState (int k)
{
	if (k == SDL_SCANCODE_RETURN)
		((AticAtacGameStateDemo*) _game -> activeState ()) -> setWantToExit (true);
}

// ---
void InputHandler::manageKeyOnPlaying (int k)
{
	AticAtacWorld* w = (AticAtacWorld*) (((AticAtacGame*) _game) -> activeWorld ());
	#ifndef NDEBUG
	if (k == SDL_SCANCODE_LEFT || 
		k == SDL_SCANCODE_RIGHT) // Just to move an screen more or less...
	{
		int rN = w -> roomNumber () + ((k == SDL_SCANCODE_LEFT) ? -1 : 1);
		if (rN >= __NUMBEROFROOMS__) rN = 0;
		if (rN < 0) rN = __NUMBEROFROOMS__ - 1;
		w -> setRoomNumber (rN);
	}
	else
	#endif
	if (k == SDL_SCANCODE_Q) // The q is to catch what is behind...if any...
		((AticAtacGame*) _game) -> setWantToCatch ();
	else
	if (k == SDL_SCANCODE_SPACE) // The space is to shoot the weapon...
		((AticAtacGame*) _game) -> shoot ();
	else
	if (k == SDL_SCANCODE_Z) // To pause/continue the game...
		_game -> isGamePaused () ? _game -> continueGame () : _game -> pauseGame ();
}

// ---
void InputHandler::manageKeyOnEnd (int k)
{
	if (k == SDL_SCANCODE_RETURN)
		((AticAtacGameStateEnd*) _game -> activeState ()) -> setWantToExit (true);
}

// ---
void InputHandler::manageKeyOnWin (int k)
{
	if (k == SDL_SCANCODE_RETURN)
		((AticAtacGameStateWin*) _game -> activeState ()) -> setWantToExit (true);
}

