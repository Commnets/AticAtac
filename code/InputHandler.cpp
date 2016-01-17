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
		int dX = 0; int dY = 0;
		if (k [SDL_SCANCODE_S]) dX = 1;
		if (k [SDL_SCANCODE_A]) dX = -1;
		if (k [SDL_SCANCODE_L]) dY = 1;
		if (k [SDL_SCANCODE_P]) dY = -1;
		((AticAtacWorld*) (((AticAtacGame*) _game) -> activeWorld ())) 
			-> moveCharacter (QGAMES::Vector (__BD dX, __BD dY, __BD 0));
	}
}

// ---
void InputHandler::onJoystickAxisMoveEvent (QGAMES::JoystickMovementEventData* dt)
{
	// TO DO
}

// ---
void InputHandler::onJoystickButtonEvent (QGAMES::JoystickButtonEventData* dt)
{
	// TO DO
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

