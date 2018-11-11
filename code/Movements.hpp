/** \addtogroup Atic */
/*@{*/

#ifndef __MOVEMENTS_HPP__
#define __MOVEMENTS_HPP__

#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

/** Class to move an entity in the room. */
class MovementEntityInRoom : public QGAMES::MovementOneByOne
{
	public:
	// Constructors & Destructors
	MovementEntityInRoom (int id, std::map <int, double> v)
		: QGAMES::MovementOneByOne (id, v),
		  _minPosition (QGAMES::Position::_cero),
		  _maxPosition (QGAMES::Position::_cero)
							{ }

	/** To set and to know the limitis of the movement. */
	void setLimits (const QGAMES::Position& min, const QGAMES::Position& max)
							{ _minPosition = min; _maxPosition = max; }
	void setLimits (const QGAMES::Rectangle& r)
							{ setLimits (r.pos1 (), r.pos2 ()); }
	const QGAMES::Position& minPosition () const
							{ return (_minPosition); }
	const QGAMES::Position& maxPosition () const
							{ return (_maxPosition); }

	virtual QGAMES::Vector direction () const
							{ return (QGAMES::Vector::_cero); }
	virtual QGAMES::Vector acceleration () const
							{ return (QGAMES::Vector::_cero); }

	private:
	virtual void moveOne (const QGAMES::Vector& d, const QGAMES::Vector& a, 
			QGAMES::Entity* e);

	private:
	/** The limits. */
	QGAMES::Position _minPosition, _maxPosition;
};

/** Class to represent the movement of a monster in a room.
	The room has limits to maintain, but the monster moves one pixel by one pixel. */
class MovementMonsterInRoom : public MovementEntityInRoom
{
	public:
	// Constructors & Destructors
	MovementMonsterInRoom (int id, std::map <int, double> v)
		: MovementEntityInRoom (id, v)
							{ }

	virtual Movement* clone () const;

	virtual void initialize ();
};

/** Class to represent the movement of the main character in the room. */
class MovementPlayerInRoom : public MovementEntityInRoom
{
	public:
	// Constructors & Destructors
	MovementPlayerInRoom (int id, std::map <int, double> v)
		: MovementEntityInRoom (id, v)
							{ }

	virtual Movement* clone () const;

	virtual void initialize ();
};

/** Class to represent the movement of a weapon. */
class MovementWeaponInRoom : public MovementEntityInRoom
{
	public:
	// Constructors & Destructors
	MovementWeaponInRoom (int id, std::map <int, double> v)
		: MovementEntityInRoom (id, v)
							{ }

	virtual Movement* clone () const;

	virtual void initialize ();
};

#endif
  
// End of the file
/*@}*/
