/** \addtogroup Atic */
/*@{*/

#ifndef __INPUTHANDLER_HPP__
#define __INPUTHANDLER_HPP__

#include <Arcade/arcadeinclude.hpp>

class InputHandler : public QGAMES::InputHandler::Behaviour
{
	public:
	InputHandler ()
		: QGAMES::InputHandler::Behaviour (),
		  _joystick (false),
		  _lastKey (-1),
		  _mX (0), _mY (0),
		  _dJ (0)
							{ }

	// To control the joystick...
	/** To know whether the joystick is or not active. */
	bool isJoystickActive () const
							{ return (_joystick); }
	/** Activate or desactivate the joystick */
	void activateJoystick (bool st)
							{ _joystick = st; }
	/** To reset the control of the joystick movement. */
	void resetJoystickMovement ()
							{ _mX = _mY = 0; }
	/** To know the joystick current orientation. */
	QGAMES::Vector joystickOrientation () const
							{ return (QGAMES::Vector (__BD _mX, __BD _mY, __BD 0)); }

	/** @see parent. */
	virtual void onJoystickAxisMoveEvent (QGAMES::JoystickMovementEventData* dt);
	virtual void onJoystickButtonEvent (QGAMES::JoystickButtonEventData* dt);
	virtual void onKeyboardEvent (QGAMES::KeyboardEventData* dt);
	virtual void onMouseMovementEvent (QGAMES::MouseMovementEventData* dt);
	virtual void onMouseButtonEvent (QGAMES::MouseButtonEventData* dt);
	virtual void onKeyboardStatus (const std::vector <bool>& kS);

	private:
	void manageKeyOnInitialState (int k);
	void manageKeyOnSelectState (int k);
	void manageKeyOnDemoState (int k);
	void manageKeyOnPlaying (int k);
	void manageKeyOnEnd (int k);
	void manageKeyOnWin (int k);

	private:
	/** Is the joystick the main element to control the main character? */
	bool _joystick;

	// Implementation
	/** Last key pressed to control if it has been released. */
	int _lastKey;
	/** The directions of the last movement in the joystick.
		This is needed because the movement of the joystick is not just only one event.
		If the joystick is in a diagonal two or three events can be generated. 
		an it is necessary to acumulate the position to better control. */
	int _mX, _mY;
	/** To control whether it is necessary to move to the next or previous option
		using a joystick or not.*/
	int _dJ;
};

#endif
  
// End of the file
/*@}*/