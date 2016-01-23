/** \addtogroup Atic */
/*@{*/

#ifndef __BKENTITIES_HPP__
#define __BKENTITIES_HPP__

#include <Common/entity.hpp>
#include <Common/rectangle.hpp>
#include <Arcade/artist.hpp>
#include "General.hpp"
#include "Defs.hpp"

class AticAtacBackgroundEntity;
typedef std::vector <AticAtacBackgroundEntity*> BackgroundEntities;

/** This class represents a background object within the game.
	The final aspect and locations will depend on the room the player is in. */
class AticAtacBackgroundEntity : public QGAMES::Artist
{
	public:
	// Constructors
	AticAtacBackgroundEntity (int id, const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ()) 
			: QGAMES::Artist (id, f, d),
			  _isVisible (false),
			  _canCollision (false),
			  _dobjDefinition (),
			  _counter (0),
			  _counterFrames (),
			  _isDoorOpen (false),
			  _canOpenClose (true)
							{ /** Nothing else to do . */ }

	virtual Entity* clone ()
							{ return (new AticAtacBackgroundEntity (_id, _forms, _data)); }

	/** To know the zone of influence. */
	QGAMES::Rectangle influenceZone () const;

	/** To set the aspect depending on the object definition. */
	void setAspectTo (const General::ObjectDefinition& obj);
	/** To get the id of the final aspect. */
	int aspectId () const
							{ return (_dobjDefinition._id); }
	/** To know what it is. */
	General::WhatIs whatIs () const
							{ return (_dobjDefinition._whatIs); }
	/** To know the orientation in terms of a vector. */
	QGAMES::Vector frameOrientation () const;
	/** To know whether the background object is or not visible now. */
	bool isVisible () const
							{ return (_isVisible); }
	/** To activate or desactivate if the background object is visible. 
		In a room (scene) there would be different background objects (up to a maximum of 8).
		The objects in the scene will be always the "same" entities.
		Their aspects and positions will change according to the room the player is in. 
		They can be activated. */
	void setVisible (bool v)
							{ _isVisible = v; }
	/** To know whether the background object can or not collision with other. 
		If it can collision or not will depend on the object it is. */
	bool canCollision () const
							{ return (_canCollision); }

	/** To determinate whether the object is or not a door.
		The object is a door if it is communicate with another object. */
	bool isADoor () const
							{ return (_dobjDefinition._isConnected); }
	/** To know whether the door open and close automatically. */
	bool doorOpenClose () const
							{ return (_canOpenClose); }
	/** To open or close the door. */
	void openDoor (bool st);
	/** To verify whether the door is or not open. */
	bool isDoorOpen ()
							{ return (_isDoorOpen); }
	/** To verify whether the door can or not be opened with a thing. */
	bool canDoorBeOpenedWith (const std::vector <bool>& t) const;
	/** To know the object this other one is connected to. */
	int roomConnected () const
							{ return (_dobjDefinition._connectedToRoom); }
	/** To know the position of the object this other one is connected to. */
	QGAMES::Position posObjConnected () const;
	/** To know the id of the object connected. */
	int objConnectedId () const
							{ return (_dobjDefinition._connectedToObj); }
	/** To know the id of the object defining this one. */
	int originalObjectId () const
							{ return (_dobjDefinition._id); }
	/** To know whether the object blocks or not the movement of the player. */
	bool blockMovement () const
							{ return (_dobjDefinition._blockMovement); }

	/** The initialization is necessary.
		All the background entities are not visible at the beggining */
	virtual void initialize ();
	/** The position is updated, moving between the initial and the final frame.
		Most of the background elements has no movement at all, so both will be equal. */
	virtual void updatePositions ();
	/** The entity is only drawn when it is visible. 
		The number of forms included can be more than one. */
	virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

	private:
	/** To determinate whether the object is or not visible. */
	bool _isVisible;
	/** To determinate whether the object can or not collision with 
		the player usually. This happens in certain type of
		objects, like doors and other behaving like doors. */
	bool _canCollision;

	// Implementation
	/** To know the last object definition. */
	General::ObjectDefinition _dobjDefinition;
	/** Counter to know when to change the aspect (if any) */
	int _counter;
	/** Counters to kept track of the changes in the form. */
	std::vector <int> _counterFrames;
	/** When it is a door, this variable defines whether the door is or not open. */
	bool _isDoorOpen;
	/** To determinate whether the door (if the object is a door obviously) can change. */
	bool _canOpenClose;
};

#endif
  
// End of the file
/*@}*/
