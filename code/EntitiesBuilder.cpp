#include "EntitiesBuilder.hpp"
#include "Entities.hpp"
#include "Defs.hpp"

// ---
QGAMES::Entity* EntitiesBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{ 
	QGAMES::Entity* result = NULL;
	if ((def._id >= __ENTITYBACKGROUNDBASE && 
			def._id < (__ENTITYBACKGROUNDBASE + __NUMBERMAXENTITIESBK)) ||
		(def._id >= (10 * __ENTITYBACKGROUNDBASE + 1) && 
			def._id < (10 * __ENTITYBACKGROUNDBASE + __NUMBERMAXENTITIESBK + 1))) 
			result = new AticAtacBackgroundEntity (def._id);
	else
	if (def._id >= __ENTITYMONSTERBASE && 
			def._id < (__ENTITYMONSTERBASE + __NUMBERMAXENTITIESMONSTER))
		result = new AticAtacMonster (def._id);
	else
	if (def._id >= __ENTITYEATENBASE && 
			def._id < (__ENTITYEATENBASE + __NUMBERMAXENTITIESEAT))
		result = new AticAtacFood (def._id);
	else
	if (def._id >= __ENTITYCATCHBASE && 
			def._id < (__ENTITYCATCHBASE + __NUMBERMAXENTITIESCATCH))
		result = new AticAtacThingToCatch (def._id);
	else
	if (def._id == __ENTITYPLAYER)
		result = new AticAtacCharacter (def._id);
	else
	if (def._id > __ENTITYPLAYER &&
		def._id < (__ENTITYPLAYER + __NUMBERMAXENTITIESPLAYER))
		result = new AticAtacWeapon (def._id);
	else
			exit (1);
	return (result);
}
