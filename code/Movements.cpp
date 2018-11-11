#include "Movements.hpp"

// ---
void MovementEntityInRoom::moveOne (const QGAMES::Vector& d, const QGAMES::Vector& a, 
			QGAMES::Entity* e)
{
	e -> setPosition (e -> position () + d);
	e -> setOrientation (d);
}

// ---
QGAMES::Movement* MovementMonsterInRoom::clone () const
{
	QGAMES::Movement* result = new MovementMonsterInRoom (_id, _variables); 
	result -> initialize (); 
	return (result); 
}

// ---
void MovementMonsterInRoom::initialize ()
{
	QGAMES::MovementOneByOne::initialize ();

	// Re-set up the speed, using a random number...
	// But maintaining a minimum one...
	// and with the possibility of moving faster than the base speed... (take care)
	_speed = _speed + (rand () % ((int) ((_baseSpeed - _speed) * 1.5)));
	_speed = _baseSpeed;
}

// ---
QGAMES::Movement* MovementPlayerInRoom::clone () const
{
	QGAMES::Movement* result = new MovementPlayerInRoom (_id, _variables); 
	result -> initialize (); 
	return (result); 
}

// ---
void MovementPlayerInRoom::initialize ()
{
	QGAMES::MovementOneByOne::initialize ();

	// Faster than none...
	_speed = 4 * _baseSpeed;
}

// ---
QGAMES::Movement* MovementWeaponInRoom::clone () const
{
	QGAMES::Movement* result = new MovementWeaponInRoom (_id, _variables); 
	result -> initialize (); 
	return (result); 
}

// ---
void MovementWeaponInRoom::initialize ()
{
	QGAMES::MovementOneByOne::initialize ();

	// Faster event than the main character...
	_speed = 10 * _baseSpeed;
}
