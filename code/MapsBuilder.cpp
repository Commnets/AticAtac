#include "MapsBuilder.hpp"
#include "Maps.hpp"
#include "Defs.hpp"

// ---
QGAMES::Map* MapsBuilderAddsOn::createMap (int id, int w, int h, int d,
		const QGAMES::Layers& l, const QGAMES::MapProperties& pties)
{
	QGAMES::Map* result = (id == __ATICATACMAP) 
		? new AticAtacMap (id, l, w, h, pties) // NO depth is needed...
		: QGAMES::ObjectMapBuilderAddsOn::createMap (id, w, h, d, l, pties);
	return (result);
}

