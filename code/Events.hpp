/** \addtogroup Atic */
/*@{*/


#ifndef __EVENTS_HPP__
#define __EVENTS_HPP__

#include <Common/event.hpp>
#include <Common/position.hpp>

/** A simple structure to keep information 
	about the target destination when the player is falling. */
struct AticAtacFallingEventData
{
	int _roomNumber;
	QGAMES::Position _position;
};

/** A simple structure to keep information
	to change the room. */
struct AticAtacChangeRoomEventData
{
	int _roomNumber;
	int _originalRoomNumber;
	int _objOrigin;
	QGAMES::Position _position;
};

/** Simply the game finishes. */
struct AticAtacFinishEventData
{
	int _roomNumber;
	QGAMES::Position _position;
};

#endif
  
// End of the file
/*@}*/