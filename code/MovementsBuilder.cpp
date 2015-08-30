#include "MovementsBuilder.hpp"
#include "Movements.hpp"
#include "Defs.hpp"

// ---
QGAMES::Movement* MovementsBuilder::createMovement 
	(const QGAMES::MovementBuilder::MovementDefinition& def)
{ 
	QGAMES::Movement* result = NULL;

	if (def._id >= __MOVEMENTMONSTERINROOMBASE && 
		def._id < (__MOVEMENTMONSTERINROOMBASE + __NUMBERMAXENTITIESMONSTER))
			result = new MovementMonsterInRoom (def._id, def._variables); 
			// The series of movement related with the monsters...
	else
	if (def._id == __MOVEMENTCHARACTER)
		result = new MovementPlayerInRoom (def._id, def._variables);
	else
		if (def._id >= __MOVEMENTWEAPON && 
			def._id < (__MOVEMENTWEAPON + __MAXNUMBEROFSIMULTANEOUSSHOOTS))
		result = new MovementWeaponInRoom (def._id, def._variables);
	else
		exit (1);

	return (result);
}
