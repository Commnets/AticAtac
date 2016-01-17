#include "Maps.hpp"
#include "General.hpp"
#include "Defs.hpp"
#include <Common/game.hpp>
#include <Common/screen.hpp>

// ---
void AticAtacMap::setRoomNumber (int r)
{
	assert (r >= 0 && r < __NUMBEROFROOMS__);

	_roomNumber = r;
	if (_roomNumber == __FALLINGROOM)
	{
		_counter = 0;
		_fallingLines = 0;
	}
}

// ---
QGAMES::Layers AticAtacMap::relevantLayers ()
{
	return (QGAMES::Layers ()); // No relevant layers...
}

// ---
void AticAtacMap::initialize ()
{
	QGAMES::ObjectMap::initialize ();

	_roomNumber = __FIRSTROOM;
	_counter = 0;
	_fallingLines = 0;
}

// ---
void AticAtacMap::updatePositions ()
{
	// If the map has the form of the falling room...
	if (_roomNumber == __FALLINGROOM)
	{
		if (_counter++ > ((QGAMES::Game::game () -> framesPerSecond ()) / 120))
		{
			_counter = 0;
			_fallingLines += 4; // Every box is made up of four lines...
			if (_fallingLines > 
				((int) General::_e._roomTypes [General::_e._rooms [_roomNumber]._type].size () - 4)) 
					_fallingLines = 0; // Start again if the limit of lines has been reached...
		}
	}

	// What is standard...
	QGAMES::ObjectMap::updatePositions ();
}

// ---
void AticAtacMap::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::ObjectMap::drawOn (s, p);

	General::RoomDefinition room = General::_e._rooms [_roomNumber];
	General::RoomPicture picture = General::_e._roomTypes [room._type];

	// Draw the room
	// The aspect will depend on the room number...
	// In the case the roomis the falling one, only a couple of lines 
	// will be drawn in each iteration...
	if (_roomNumber == __FALLINGROOM)
	{
		for (int i = 0; i < 4; i++)
			s -> drawLine (picture [i + _fallingLines]._origin, picture [i + _fallingLines]._end, 
				room._color);
	}
	else
	{
		for (General::RoomPicture::const_iterator i = picture.begin ();
				i != picture.end (); i++)
			s -> drawLine ((*i)._origin, (*i)._end, room._color);
	}

	#ifndef NDEBUG
	// Only in debug mode...
	// Draw something at the center...
	// and something in the limits of the floor of the room...
	QGAMES::Position c = QGAMES::Position (__BD room._cx, __BD room._cy);
	QGAMES::Position leftUp = c - QGAMES::Position (__BD room._w, __BD room._h, __BD 0);
	QGAMES::Position leftDown = c - QGAMES::Position (__BD room._w,	-__BD room._h, __BD 0);
	QGAMES::Position rightUp = c + QGAMES::Position (__BD room._w, -__BD room._h, __BD 0);
	QGAMES::Position rightDown = c + QGAMES::Position (__BD room._w, __BD room._h, __BD 0);
	QGAMES::Position sHLine (__BD 30, __BD 0, __BD 0);
	QGAMES::Position sVLine (__BD 0, __BD 30, __BD 0);
	s -> drawLine (c - sVLine, c + sVLine, __GRAYCOLOR);
	s -> drawLine (c - sHLine, c + sHLine, __GRAYCOLOR);
	s -> drawLine (leftUp, leftUp + sHLine, __GRAYCOLOR);
	s -> drawLine (leftUp, leftUp + sVLine, __GRAYCOLOR);
	s -> drawLine (leftDown, leftDown + sHLine, __GRAYCOLOR);
	s -> drawLine (leftDown, leftDown - sVLine, __GRAYCOLOR);
	s -> drawLine (rightUp, rightUp - sHLine, __GRAYCOLOR);
	s -> drawLine (rightUp, rightUp + sVLine, __GRAYCOLOR);
	s -> drawLine (rightDown, rightDown - sHLine, __GRAYCOLOR);
	s -> drawLine (rightDown, rightDown - sVLine, __GRAYCOLOR);
	#endif
}
