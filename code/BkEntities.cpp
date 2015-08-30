#include "Entities.hpp"
#include "Movements.hpp"
#include <Arcade/arcadegame.hpp>
#include <Common/form.hpp>
#include <iostream>

// ---
QGAMES::Rectangle AticAtacBackgroundEntity::influenceZone () const
{
	// The influence zone in a door is normally the last form, as it is usually the door itself.
	// In the case of the clock the size is to small to verify two corners of the player are inside.
	// So it is better to get the first one, even considering the rest is to big...
	// General::FormDefinition fBD = _dobjDefinition._forms [_dobjDefinition._forms.size () - 1]; 
	General::FormDefinition fBD = _dobjDefinition._forms [0]; 
	QGAMES::Form* fB = (*_forms.find (fBD._form)).second;
	QGAMES::Position leftUp = _data._position + fBD._adjust;
	QGAMES::Position rightDown = leftUp + QGAMES::Vector (__BD fB -> frameWidth (), 
		__BD fB -> frameHeight (), __BD 0);
	return (QGAMES::Rectangle (leftUp, rightDown));
}

// ---
void AticAtacBackgroundEntity::setAspectTo (const General::ObjectDefinition& obj)
{
	_dobjDefinition = obj;
	_isVisible = true; // Setting up the aspect becomes the object visible...

	// The number of counters to count changes in
	// The aspect will depend on the number of different forms..
	_counter = 0;
	_counterFrames.resize (_dobjDefinition._forms.size ());
	for (int i = 0; i < (int) _counterFrames.size (); i++)
		_counterFrames [i] = _dobjDefinition._forms [i]._initialFrame;

	// Initially the door is close..if any...
	_isDoorOpen = false;
	_canOpenClose = (_dobjDefinition._time != -1); 
	// Only a cople of them can open and close automatically...

	// The place where the object is...
	setPosition (_dobjDefinition._position); 
}

// ---
QGAMES::Vector AticAtacBackgroundEntity::frameOrientation () const
{
	General::FormDefinition mF = _dobjDefinition._forms [0];
	QGAMES::Vector result = QGAMES::Vector::_cero;
	if (mF._orientation == General::Orientation::__UP)
		result = QGAMES::Vector (__BD 0, __BD -1, __BD 0);
	if (mF._orientation == General::Orientation::__RIGHT)
		result = QGAMES::Vector (__BD 1, __BD 0, __BD 0);
	if (mF._orientation == General::Orientation::__DOWN)
		result = QGAMES::Vector (__BD 0, __BD 1, __BD 0);
	if (mF._orientation == General::Orientation::__LEFT)
		result = QGAMES::Vector (__BD -1, __BD 0, __BD 0);
	return (result);
}

// ---
void AticAtacBackgroundEntity::openDoor (bool st)
{
	if (!isADoor ())
		return; // Only it is a door, it can be opened...
	_isDoorOpen = st;
}

// ---
bool AticAtacBackgroundEntity::canDoorBeOpenedWith (const std::vector <bool>& t) const
{
	bool result = false;
	if (whatIs () == General::__GREENDOOR && t[General::__GREENKEY]) result = true;
	if (whatIs () == General::__YELLOWDOOR && t[General::__YELLOWKEY]) result = true;
	if (whatIs () == General::__REDDOOR && t[General::__REDKEY]) result = true;
	if (whatIs () == General::__WHITEDOOR && t[General::__WHITEKEY]) result = true;
	if (whatIs () == General::__EXITDOOR && (t[General::__ACGKEY1] && 
		t[General::__ACGKEY2] && t[General::__ACGKEY3])) result = true;
	return (result);
}

// ---
QGAMES::Position AticAtacBackgroundEntity::posObjConnected () const
{
	// Gets the object itself...
	General::ObjectDefinition obj = General::_e._objects [_dobjDefinition._connectedToObj];
	// Get the form of the new object...
	General::FormDefinition fD = *obj._forms.begin ();
	QGAMES::Form* f = QGAMES::Game::game () -> form (fD._form);
	return (obj._position + fD._adjust + 
		QGAMES::Vector (__BD (f -> frameWidth () / 2), __BD (f -> frameHeight () / 2)));
}

// ---
void AticAtacBackgroundEntity::initialize ()
{
	_isVisible = false; // The entity is initially not visible...
	_canCollision = false; // None of then can collision with none...
	_dobjDefinition = General::ObjectDefinition (); // With no form...

	_counter = 0;
	_counterFrames = std::vector <int> ();
	_isDoorOpen = false;
	_canOpenClose = true;
}

// ---
void AticAtacBackgroundEntity::updatePositions ()
{
	if (!_isVisible)
		return; // Only when visible...

	// Counters whether is or not time to change the aspect
	// This is loop gives the game the possibility to move event the backgroundobjects
	// It is not normal, but the possibility exists...
	if (_counter++ > 6)
	{
		_counter = 0;
		for (int i = 0; i < (int) _counterFrames.size (); i++)
		{
			_counterFrames [i] = _counterFrames [i] + 1;
			if (_counterFrames [i] > _dobjDefinition._forms [i]._finalFrame)
				_counterFrames [i] = _dobjDefinition._forms [i]._initialFrame;
		}
	}

	// The standard things are done...
	QGAMES::Artist::updatePositions ();
}

// ---
void AticAtacBackgroundEntity::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!_isVisible)
		return; // Only when it is visible...

	// When it is visible the background object can have several forms
	// ..and all of them have to be drawn...
	General::FormsDefinitionList fL = _dobjDefinition._forms;
	for (int i = 0; i < (int) fL.size (); i++)
	{
		bool toPaint = true;
		if (isADoor () && isDoorOpen () && i == ((int) fL.size () - 1))
			toPaint = false; // The door is always the last form...
		if (toPaint)
		{
			setCurrentForm (fL [i]._form);
			setCurrentAspect (fL [i]._initialFrame);
			QGAMES::Artist::drawOn (s, _data._position + fL [i]._adjust); 
			// It has to be drawn taking into account possible adjustments depending
			// on the form received as parameter...
		}
	}

	// Just to debug the limits are drawn
	// when the system is in debug mode and the object is a door...
	#ifndef NDEBUG
	if (isADoor () || blockMovement ())
	{
		QGAMES::Rectangle iZone = influenceZone ();
		QGAMES::Position leftUp = iZone.pos1 ();
		QGAMES::Position rightDown = iZone.pos2 ();
		QGAMES::Position leftDown (leftUp.posX (), rightDown.posY ());
		QGAMES::Position rightUp (rightDown.posX (), leftUp.posY ());
		s -> drawLine (leftUp, rightUp, __GRAYCOLOR);
		s -> drawLine (rightUp, rightDown, __GRAYCOLOR);
		s -> drawLine (rightDown, leftDown, __GRAYCOLOR);
		s -> drawLine (leftDown, leftUp, __GRAYCOLOR);
	}
	#endif
}
