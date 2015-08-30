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
		}
	}

	// What is standard...
	QGAMES::ObjectMap::updatePositions ();
}

// ---
void AticAtacMap::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::ObjectMap::drawOn (s, p);

	// It is time to draw the lines of the room the player is in...
	General::RoomDefinition room = General::_e._rooms [_roomNumber];
	General::RoomPicture picture = General::_e._roomTypes [room._type];

	// Draw the room
	// The aspect will depend on the room number...
	// In the case the roomis the falling one, only a couple of lines 
	// will be drawn in each iteration...
	if (_roomNumber == __FALLINGROOM)
	{
		if (_fallingLines > ((int) picture.size () - 4)) _fallingLines = 0;
		for (int i = 0; i < 4; i++)
			s -> drawLine (picture [i + _fallingLines]._origin, picture [i + _fallingLines]._end, 
				room._color);
	}
	else
		for (General::RoomPicture::const_iterator i = picture.begin ();
				i != picture.end (); i++)
			s -> drawLine ((*i)._origin, (*i)._end, room._color);

	#ifndef NDEBUG
	// Draw something at the center...
	// Only in debug mode...
	QGAMES::Position c = QGAMES::Position (__BD General::_e._rooms [_roomNumber]._cx,
		__BD General::_e._rooms [_roomNumber]._cy);
	QGAMES::Position leftUp = c - QGAMES::Position (__BD (General::_e._rooms [_roomNumber]._w), 
		__BD (General::_e._rooms [_roomNumber]._h), __BD 0);
	QGAMES::Position leftDown = c - QGAMES::Position (__BD (General::_e._rooms [_roomNumber]._w), 
		-__BD (General::_e._rooms [_roomNumber]._h), __BD 0);
	QGAMES::Position rightUp = c + QGAMES::Position (__BD (General::_e._rooms [_roomNumber]._w), 
		-__BD (General::_e._rooms [_roomNumber]._h), __BD 0);
	QGAMES::Position rightDown = c + QGAMES::Position (__BD (General::_e._rooms [_roomNumber]._w), 
		__BD (General::_e._rooms [_roomNumber]._h), __BD 0);
	s -> drawLine (c - QGAMES::Position (__BD 0, __BD 30, __BD 0), 
		c + QGAMES::Position (__BD 0, __BD 30), __GRAYCOLOR);
	s -> drawLine (c - QGAMES::Position (__BD 30, __BD 0, __BD 0), 
		c + QGAMES::Position (__BD 30, __BD 0, __BD 0), __GRAYCOLOR);
	s -> drawLine (leftUp, leftUp + QGAMES::Position (__BD 30, __BD 0, __BD 0), __GRAYCOLOR);
	s -> drawLine (leftUp, leftUp + QGAMES::Position (__BD 0, __BD 30, __BD 0), __GRAYCOLOR);
	s -> drawLine (leftDown, leftDown + QGAMES::Position (__BD 30, __BD 0, __BD 0), __GRAYCOLOR);
	s -> drawLine (leftDown, leftDown - QGAMES::Position (__BD 0, __BD 30, __BD 0), __GRAYCOLOR);
	s -> drawLine (rightUp, rightUp - QGAMES::Position (__BD 30, __BD 0, __BD 0), __GRAYCOLOR);
	s -> drawLine (rightUp, rightUp + QGAMES::Position (__BD 0, __BD 30, __BD 0), __GRAYCOLOR);
	s -> drawLine (rightDown, rightDown - QGAMES::Position (__BD 30, __BD 0, __BD 0), __GRAYCOLOR);
	s -> drawLine (rightDown, rightDown - QGAMES::Position (__BD 0, __BD 30, __BD 0), __GRAYCOLOR);
	#endif
}
