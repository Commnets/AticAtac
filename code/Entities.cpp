#include "Entities.hpp"
#include "Movements.hpp"

// ---
AticAtacCharacter::AticAtacCharacter (int id, const QGAMES::Forms& f, 
		const QGAMES::Entity::Data& d) 
	: QGAMES::Artist (id, f, d),
	  _type (__KNIGHT),
	  _things (),
	  _counter (0),
	  _initialFrameRight (0), _finalFrameRight (0),
	  _initialFrameLeft (0), _finalFrameLeft (0),
	  _cInitialFrame (0), _cFinalFrame (0),
	  _lastOrientation (QGAMES::Vector::_cero),
	  _doors (), _blockingElements ()
{
	// Visible by default...
	setVisible (true);

	/** Nothing else to do */ 
}

// ---
bool AticAtacCharacter::canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	assert (_currentMovement);
	assert (_currentForm);

	// Calculates a couple of positions, really useful 
	// To the next two/three groups of checkings...
	// p1, p2 are the reference cornes to check the position of the player...
	// Those depend on the direction of the movement...
	QGAMES::Rectangle cZone = collisionZone () + d;
	QGAMES::Position leftUp = cZone.pos1 ();
	QGAMES::Position rightDown = cZone.pos2 ();
	QGAMES::Position leftDown (leftUp.posX (), rightDown.posY ());
	QGAMES::Position rightUp (rightDown.posX (), leftUp.posY ());
	QGAMES::Position p1 = leftUp; QGAMES::Position p2 = rightDown;
	if (d.posX () == __BD 1)  p1 = rightUp;
	if (d.posX () == __BD -1) p2 = leftDown;
	if (d.posY () == __BD 1) p1 = leftDown;
	if (d.posY () == __BD -1) p2 = rightUp;

	// Test whether the player is collisioning or not 
	// with a blocking entity in the room
	// If it is, the movement is nor possible...
	bool result = true; // By default the movement is possible (it is the most probable option)
	for (BackgroundEntities::const_iterator i = _blockingElements.begin (); 
			i != _blockingElements.end () && result; i++)
	{
		QGAMES::Rectangle iZone = (*i) -> influenceZone ();
		if ((*i) -> blockMovement () && (iZone.hasIn (p1) || iZone.hasIn (p2)))
			result = false;
	}

	if (!result)
		return (result); // Something is blocking the movement...

	// First of all it verifies if the entity is in the room totally...
	result = false; // It this case the default option is the movement is not possible..
	QGAMES::Position min = ((MovementPlayerInRoom*) _currentMovement) -> minPosition ();
	QGAMES::Position maxPlus = ((MovementPlayerInRoom*) _currentMovement) -> maxPosition ();
	QGAMES::Position max = maxPlus - 
		QGAMES::Vector (__BD _currentForm -> frameWidth (), 
			__BD _currentForm -> frameHeight (), __BD 0);
	QGAMES::Position next = position () + d;
	if (next.posX () > min.posX () && next.posX () < max.posX () &&
		next.posY () > min.posY () && next.posY () < max.posY ())
			result = true;

	if (result)
		return (result); // Nothing is blocking the entity and it is totally in the room...

	// If the character is not totally in the room,
	// it verifies is at least in the influence area of a background door...
	for (BackgroundEntities::const_iterator i = _doors.begin (); 
			i != _doors.end () && !result; i++)
	{
		bool canEnter = 
			// Whatever the "normal" type is (or it is the exit door), and it is open...
			((*i) -> isDoorOpen () && 
				((*i) -> whatIs () == General::WhatIs::__NORMALDOR || 
				 (*i) -> whatIs () == General::WhatIs::__GREENDOOR ||
				 (*i) -> whatIs () == General::WhatIs::__YELLOWDOOR ||
				 (*i) -> whatIs () == General::WhatIs::__REDDOOR ||
				 (*i) -> whatIs () == General::WhatIs::__WHITEDOOR ||
				 (*i) -> whatIs () == General::WhatIs::__EXITDOOR)) || 
			// It is open, is a clock and you're a wizard...
			 ((*i) -> isDoorOpen () && _type == AticAtacCharacter::Type::__WIZARD && 
				(*i) -> whatIs () == General::WhatIs::__CLOCKDOOR) || 
			// similar case for a knight
			((*i) -> isDoorOpen () && _type == AticAtacCharacter::Type::__KNIGHT && 
				(*i) -> whatIs () == General::WhatIs::__BOOKCASEDOOR) || 
			// similar case for a servant...
			((*i) -> isDoorOpen () && _type == AticAtacCharacter::Type::__SERVANT && 
				(*i) -> whatIs () == General::WhatIs::__BARRELDOOR) ||
			// ..or if the door is closed but it can be open with any of the 
			// elements carrying by the player!
			(!(*i) -> isDoorOpen () && (*i) -> canDoorBeOpenedWith (_things));

		QGAMES::Rectangle iZone = (*i) -> influenceZone ();
		if (iZone.hasIn (p1) && iZone.hasIn (p2) && canEnter)
			result = true; // The points that marks the direction are in the door...
	}

	if (result)
		return (result); // The entity has nothing in front, 
						 // it is entering a door, and it can be opened...

	// If the entity is exiting a door, the the same points tested before should be now
	// In the room again...
	if (p1.posX () > min.posX () && p1.posX () < maxPlus.posX () &&
		p1.posY () > min.posY () && p1.posY () < maxPlus.posY () &&
		p2.posX () > min.posX () && p2.posX () < maxPlus.posX () &&
		p2.posY () > min.posY () && p2.posY () < maxPlus.posY ())
			result = true;

	return (result);
}

// ---
void AticAtacCharacter::setType (AticAtacCharacter::Type t)
{
	_type = t;

	switch (_type)
	{
		case AticAtacCharacter::Type::__KNIGHT:
			_initialFrameLeft = __KNIGHTLEFTINIT; _finalFrameLeft = __KNIGHTLEFTEND;
			_initialFrameRight = __KNIGHTRIGHTINIT; _finalFrameRight = __KNIGHTRIGHTEND;
			_initialFrameUp = __KNIGHTUPINIT; _finalFrameUp = __KNIGHTUPEND;
			_initialFrameDown = __KNIGHTDOWNINIT; _finalFrameDown = __KNIGHTDOWNEND;
			break;

		case AticAtacCharacter::Type::__WIZARD:
			_initialFrameLeft = __WIZARDLEFTINIT; _finalFrameLeft = __WIZARDLEFTEND;
			_initialFrameRight = __WIZARDRIGHTINIT; _finalFrameRight = __WIZARDRIGHTEND;
			_initialFrameUp = __WIZARDUPINIT; _finalFrameUp = __WIZARDUPEND;
			_initialFrameDown = __WIZARDDOWNINIT; _finalFrameDown = __WIZARDDOWNEND;
			break;

		case AticAtacCharacter::Type::__SERVANT:
			_initialFrameLeft = __SERVANTLEFTINIT; _finalFrameLeft = __SERVANTLEFTEND;
			_initialFrameRight = __SERVANTRIGHTINIT; _finalFrameRight = __SERVANTRIGHTEND;
			_initialFrameUp = __SERVANTUPINIT; _finalFrameUp = __SERVANTUPEND;
			_initialFrameDown = __SERVANTDOWNINIT; _finalFrameDown = __SERVANTDOWNEND;
			break;

		default:
			break;
	}

	// Set up the aspect and the _initialFrame...
	// Initially it is considered the character goes to the right...
	_cInitialFrame = _initialFrameRight; _cFinalFrame = _finalFrameRight;
	setCurrentForm (__CHARACTERFORM);
	setCurrentAspect (_cInitialFrame);
}

// ---
void AticAtacCharacter::leaveAllThings ()
{
	_things = std::vector <bool> ();
	_things.resize (100);
}

// ---
void AticAtacCharacter::setMovingLimits (const QGAMES::Position& min, const QGAMES::Position& max)
{
	if (_currentMovement != NULL && 
		(_currentMovement -> id () == __MOVEMENTCHARACTER))
		((MovementMonsterInRoom*) _currentMovement) -> setLimits (min, max);
}

// ---
void AticAtacCharacter::initialize ()
{
	QGAMES::Artist::initialize ();

	setType (AticAtacCharacter::Type::__KNIGHT); // By default...
	leaveAllThings ();
	setVisible (true); // By default...

	_counter = 0;
	_lastOrientation = QGAMES::Vector::_cero;
}

// ---
void AticAtacCharacter::updatePositions ()
{
	// Change the aspect of the character when it is moving...
	if (_data._orientation != QGAMES::Vector::_cero)
	{
		if (_counter++ == 4)
		{
			_counter = 0;
			if (_currentAspect++ >= _cFinalFrame)
				_currentAspect = _cInitialFrame;
			QGAMES::Game::game () -> sound (__PLAYERWALKING) 
				-> play (__WALKINGCHANNELSOUND);
		}
	}

	// The aspect depends on the direction of the movement...
	if (_data._orientation != _lastOrientation)
	{
		_lastOrientation = _data._orientation;

		if (_data._orientation.posX () == 1)
		{
			_cInitialFrame = _initialFrameRight; 
			_cFinalFrame = _finalFrameRight;
		}
		else
		if (_data._orientation.posX () == -1)
		{
			_cInitialFrame = _initialFrameLeft; 
			_cFinalFrame = _finalFrameLeft;
		}
		else
		if (_data._orientation.posY () == 1)
		{
			_cInitialFrame = _initialFrameDown; 
			_cFinalFrame = _finalFrameDown;
		}
		else
		if (_data._orientation.posY () == -1)
		{
			_cInitialFrame = _initialFrameUp; 
			_cFinalFrame = _finalFrameUp;
		}

		setCurrentAspect (_cInitialFrame);
	}

	// Verifies whether the entity is in a door...
	// If it is a notification is thrown
	AticAtacBackgroundEntity* dFound = NULL;
	QGAMES::Rectangle cZone = collisionZone ();
	QGAMES::Vector xt (__BD (_currentForm -> frameWidth () * 0.25),
		__BD (_currentForm -> frameHeight () * 0.25), __BD 0);
	cZone = QGAMES::Rectangle (cZone.pos1 () + xt, cZone.pos2 () - xt);
	for (BackgroundEntities::const_iterator i = _doors.begin (); 
			i != _doors.end () && !dFound; i++)
	{
		if ((*i) -> isDoorOpen () || (*i) -> canDoorBeOpenedWith (_things))
		// Usually, if the player has reached the center of a door...
		// The movement has allowed it. It only happens when the door is open
		// or when it is a trap door.
		// This is the reason to check again the dor is open or it can be opened...
		{
			QGAMES::Rectangle iZone = (*i) -> influenceZone ();
			if (iZone.hasIn (cZone.pos1 ()) && iZone.hasIn (cZone.pos2 ()))
				dFound= (*i); // All the points are in the door...
		}
	}

	// If the entity is in a door...
	// The change has to be notifies
	if (dFound)
		notify (QGAMES::Event (__PLAYERENTERINGDOOR, dFound));

	QGAMES::Artist::updatePositions ();
}

// ---
void AticAtacCharacter::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Artist::drawOn (s, p);

	#ifndef NDEBUG
	// Draw something around the main character...
	// Only in debug mode...
	QGAMES::Position leftUp = position ();
	QGAMES::Position leftDown = position () + 
		QGAMES::Vector (__BD 0, __BD _currentForm -> frameHeight (), __BD 0);
	QGAMES::Position rightUp = position () + 
		QGAMES::Vector (__BD _currentForm -> frameWidth (), __BD 0, __BD 0);
	QGAMES::Position rightDown = position () + 
		QGAMES::Vector (__BD _currentForm -> frameWidth (), 
		__BD _currentForm -> frameHeight (), __BD 0);
	s -> drawLine (leftUp, leftDown, __GRAYCOLOR);
	s -> drawLine (leftDown, rightDown, __GRAYCOLOR);
	s -> drawLine (rightDown, rightUp, __GRAYCOLOR);
	s -> drawLine (rightUp, leftUp, __GRAYCOLOR);
	#endif
}

// ---
void AticAtacWeapon::setType (AticAtacWeapon::Type t)
{
	_type = t;

	switch (_type)
	{
		case AticAtacWeapon::Type::__WEAPONKNIGHT:
			_initialFrameLeft = __WKNIGHTLEFTINI; _finalFrameLeft = __WKNIGHTLEFTEND;
			_initialFrameRight = __WKNIGHTRIGHTINI; _finalFrameRight = __WKNIGHTRIGHTEND;
			break;

		case AticAtacWeapon::Type::__WEAPONWIZARD:
			_initialFrameLeft = __WWIZARDLEFTINI; _finalFrameLeft = __WWIZARDLEFTEND;
			_initialFrameRight = __WWIZARDRIGHTINI; _finalFrameRight = __WWIZARDRIGHTEND;
			break;

		case AticAtacWeapon::Type::__WEAPONSERVANT:
			_initialFrameLeft = __WSERVANTLEFTINI; _finalFrameLeft = __WSERVANTLEFTEND;
			_initialFrameRight = __WSERVANTRIGHTINI; _finalFrameRight = __WSERVANTRIGHTEND;
			break;

		default:
			break;
	}

	// Set up the aspect and the _initialFrame...
	// Initially it is considered the character goes to the right...
	_cInitialFrame = _initialFrameRight; _cFinalFrame = _finalFrameRight;
	setCurrentForm (__WEAPON1FORM + (int) _type);
	setCurrentAspect (_cInitialFrame);
}

// ---
void AticAtacWeapon::setVisible (bool v)
{ 
	QGAMES::Artist::setVisible (v);
	if (isVisible ())
	{
		_counterMov = 0;
		_maxSecondsMov = 4 + (rand () % 4); // Max 8 seconds...and 4 min
	}
}

// ---
void AticAtacWeapon::setMovingLimits (const QGAMES::Position& min, const QGAMES::Position& max)
{
	if (_currentMovement != NULL && 
		(_currentMovement -> id () == __MOVEMENTWEAPON))
		((MovementMonsterInRoom*) _currentMovement) -> setLimits (min, max);
}

// ---
void AticAtacWeapon::initialize ()
{
	QGAMES::Artist::initialize ();

	setType (AticAtacWeapon::Type::__WEAPONKNIGHT); // By default...
	setVisible (true); // By default...

	_counter = 0;
	_lastOrientation = QGAMES::Vector::_cero;
}

// ---
void AticAtacWeapon::updatePositions ()
{
	// If it is not visible, the is doesn't move...
	if (!isVisible ())
		return;

	QGAMES::Artist::updatePositions ();

	// Change the aspect of the weapon when it is moving...
	if (_data._orientation != QGAMES::Vector::_cero)
	{
		if (_counter++ == 4)
		{
			_counter = 0;
			if (_currentAspect++ >= _cFinalFrame)
				_currentAspect = _cInitialFrame;
		}
	}

	// The aspect depends on the direction of the movement...
	if (_data._orientation != _lastOrientation)
	{
		_lastOrientation = _data._orientation;

		if (_data._orientation.posX () == 1)
		{
			_cInitialFrame = _initialFrameRight; 
			_cFinalFrame = _finalFrameRight;
		}
		else
		if (_data._orientation.posX () == -1)
		{
			_cInitialFrame = _initialFrameLeft; 
			_cFinalFrame = _finalFrameLeft;
		}

		setCurrentAspect (_cInitialFrame);
	}

	// Depnding on the position, it would have to change the orientation 
	// of the movement of the weapon...
	QGAMES::Position min = ((MovementWeaponInRoom*) _currentMovement) -> minPosition ();
	QGAMES::Position maxPlus = ((MovementWeaponInRoom*) _currentMovement) -> maxPosition ();
	QGAMES::Position max = maxPlus - 
		QGAMES::Vector (__BD _currentForm -> frameWidth (), 
			__BD _currentForm -> frameHeight (), __BD 0);

	QGAMES::Vector d = _data._orientation;
	if (position ().posX () <= min.posX () || position ().posX () >= max.posX ())
		d = QGAMES::Vector (-1 * d.posX (), d.posY (), d.posZ ());
	if (position ().posY () <= min.posY () || position ().posY () >= max.posY ())
		d = QGAMES::Vector (d.posX (), -1 * d.posY (), d.posZ ());
	setMove (d); // Change the movement...

	// The waepon has to fly for only a couple of seconds...
	if (_counterMov++ > (_maxSecondsMov * QGAMES::Game::game () -> framesPerSecond ()))
		notify (QGAMES::Event (__SHOOTFINISH, this));
}

// ---
void AticAtacWeapon::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Artist::drawOn (s, p);

	#ifndef NDEBUG
	// Draw something around the main character...
	// Only in debug mode...
	QGAMES::Position leftUp = position ();
	QGAMES::Position leftDown = position () + 
		QGAMES::Vector (__BD 0, __BD _currentForm -> frameHeight (), __BD 0);
	QGAMES::Position rightUp = position () + 
		QGAMES::Vector (__BD _currentForm -> frameWidth (), __BD 0, __BD 0);
	QGAMES::Position rightDown = position () + 
		QGAMES::Vector (__BD _currentForm -> frameWidth (), 
		__BD _currentForm -> frameHeight (), __BD 0);
	s -> drawLine (leftUp, leftDown, __GRAYCOLOR);
	s -> drawLine (leftDown, rightDown, __GRAYCOLOR);
	s -> drawLine (rightDown, rightUp, __GRAYCOLOR);
	s -> drawLine (rightUp, leftUp, __GRAYCOLOR);
	#endif
}
