/** \addtogroup Atic */
/*@{*/


#ifndef __ENTITIES_HPP__
#define __ENTITIES_HPP__

#include "BkEntities.hpp"
#include "MtEntities.hpp"
#include "EtEntities.hpp"
#include "CtEntities.hpp"

/** This class represents the main character in atic atac. */
class AticAtacCharacter : public QGAMES::Artist
{
	public:
	typedef enum { __KNIGHT = 0, __WIZARD = 1, __SERVANT = 2 } Type;

	// Constructors
	AticAtacCharacter (int id, const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ()); 

	virtual Entity* clone ()
							{ return (new AticAtacCharacter (_id, _forms, _data)); }
	/** To determinate whether the player can or not move.
		This method is used by the movement classes.
		The content is quite complicated, as the player can move into the doors
		when they are opened or he has the right key to open them. */
	virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a);

	/** To set and to know the type. */
	void setType (Type t);
	Type type () const
							{ return (_type); }

	/** To know whether the entity has or not a thing. */
	bool hasThing (General::WhatIs t) const
							{ return (((int) _things.size () < (int) t) ? _things [(int) t] : false); }
	/** To know whether the entity has or not the exit key. */
	bool hasExitKey () const
							{ return (hasThing (General::__ACGKEY1) && 
								hasThing (General::__ACGKEY2) && hasThing (General::__ACGKEY3)); }
	/** To get a thing. */
	void getThing (General::WhatIs t)
							{ if ((int) _things.size () > (int) t) _things [(int) t] = true; }
	/** To leave a thing. */
	void leaveThing (General::WhatIs t)
							{ if ((int) _things.size () > (int) t) _things [(int) t] = false; }
	/** To leave all things. */
	void leaveAllThings ();

	/** Set the limits to move the character....when it moves
		Two versions of the method... */
	void setMovingLimits (const QGAMES::Position& min, const QGAMES::Position& max);
	void setMovingLimits (const QGAMES::Rectangle& l)
							{ setMovingLimits (l.pos1 (), l.pos2 ()); }
	/** A reference to the entities being door in the room. */
	void setDoors (const BackgroundEntities& d)
							{ _doors = d; }
	/** A reference to the blovking entities in the room. */
	void setBlockingElements (const BackgroundEntities& b)
							{ _blockingElements = b; }

	/** The initialization is necessary,
		at least to set up the aspect. */
	virtual void initialize ();
	/** The position is updated, moving between the initial and the final frame. */
	virtual void updatePositions ();
	/** To draw the character. */
	void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

	private:
	/** The type of character we are playing with. */
	Type _type;
	/** The things the entity has. */
	std::vector <bool> _things;

	// Implementation
	/** Counter to count steps to change the aspect. */
	int _counter;
	/** The initial and final frame.
		It is something that depens on the aspect of the character. */
	int _initialFrameRight, _finalFrameRight;
	int _initialFrameLeft, _finalFrameLeft;
	int _initialFrameUp, _finalFrameUp;
	int _initialFrameDown, _finalFrameDown;
	int _cInitialFrame, _cFinalFrame;
	/** The last orientation used by the character. */
	QGAMES::Vector _lastOrientation;
	/** The list of the doors in the room closed to the character. */
	BackgroundEntities _doors;
	/** The list of the blocking elements in the room limiting the 
		movement of the player. */
	BackgroundEntities _blockingElements;
};

/** This class represents a weapon. */
class AticAtacWeapon : public QGAMES::Artist
{
	public:
	typedef enum { __WEAPONKNIGHT = 0, __WEAPONWIZARD = 1, __WEAPONSERVANT = 2 } Type;

	// Constructors
	AticAtacWeapon (int id, const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ()) 
			: QGAMES::Artist (id, f, d),
			  _type (__WEAPONKNIGHT),
			  _counter (0), _counterMov (0), _maxSecondsMov (0),
			  _initialFrameRight (0), _finalFrameRight (0),
			  _cInitialFrame (0), _cFinalFrame (0),
			  _lastOrientation (QGAMES::Vector::_cero)
							{ setVisible (true); }

	virtual Entity* clone ()
							{ return (new AticAtacWeapon (_id, _forms, _data)); }

	/** To set and to know the type. */
	void setType (Type t);
	Type type () const
							{ return (_type); }
	/** @see parent.
		To activate or desactivate if the entity object is visible.  */
	virtual void setVisible (bool v);

	/** Set the limits to move the weapon....when it moves
		Two versions of the method... */
	void setMovingLimits (const QGAMES::Position& min, const QGAMES::Position& max);
	void setMovingLimits (const QGAMES::Rectangle& l)
							{ setMovingLimits (l.pos1 (), l.pos2 ()); }

	/** The initialization is necessary,
		at least to set up the aspect. */
	virtual void initialize ();
	/** The position is updated, moving between the initial and the final frame. */
	virtual void updatePositions ();
	/** To draw the weapon. */
	void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

	private:
	/** The type of weapon we are playing with. */
	Type _type;

	// Implementation
	/** Counter to count steps to change the aspect. */
	int _counter;
	/** Counter to determinate when to finishes the shoot. */
	int _counterMov;
	/** The max number of seconds the shoot is flyfing. */
	int _maxSecondsMov;
	/** The initial and final frame.
		It is something that depens on the aspect of the weapon. */
	int _initialFrameRight, _finalFrameRight;
	int _initialFrameLeft, _finalFrameLeft;
	int _cInitialFrame, _cFinalFrame;
	/** The last orientation used by the weapon. */
	QGAMES::Vector _lastOrientation;
};

#endif
  
// End of the file
/*@}*/