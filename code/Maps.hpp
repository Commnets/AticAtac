#ifndef __MAPS_HPP__
#define __MAPS_HPP__

#include <Arcade/map.hpp>

/** This class represents a map in Atic Atac Game.
	In this case, the map, adopt the form of the room the player is in. */
class AticAtacMap : public QGAMES::ObjectMap
{
	public:
	AticAtacMap (int c, const QGAMES::Layers& l, int w, int h,
		const QGAMES::MapProperties& p = QGAMES::MapProperties ())
		: QGAMES::ObjectMap (c, l, w, h, p),
		  _roomNumber (0),
		  _counter (0),
		  _fallingLines (0)
							{ }

	/** To know the room the map is representing. */
	int roomNumber () const
							{ return (_roomNumber); }
	/** To set the aspect of the map. */
	void setRoomNumber (int r);

	/** In this type of map there are no relevant layers. */
	virtual QGAMES::Layers relevantLayers ();

	virtual void initialize ();
	/** When the room is the falling room only a part of the room is draw
		in each iteration, This method is to actualize whic lines are drawn. */
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s, 
		const QGAMES::Position& p = QGAMES::Position::_noPoint);

	/** The entities are ordered from the beginning.
		It is not necessary to order at all. */
	virtual QGAMES::Entities orderEntitiesToDraw (const QGAMES::Entities& e)
							{ return (e); }

	private:
	/** The number of the room the player is now in.
		Initially this map is 0. */
	int _roomNumber; // Initially it is 0;

	// Implementation
	/** To count before changing the lines to draw in a falling room. */
	int _counter;
	/** The lines that are being draw in a falling room. */
	int _fallingLines;
};

#endif