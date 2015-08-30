#include "Entities.hpp"
#include "Movements.hpp"
#include <Arcade/arcadegame.hpp>
#include <iostream>

// ---
void AticAtacFood::setAspectTo (const General::ThingDefinition& df)
{
	_dObjDefinition = df;

	General::FormsDefinitionList fs = df._forms;
	General::FormDefinition f = fs [rand () % fs.size ()]; // The food can have several forms...
	setCurrentForm (f._form);
	setCurrentAspect (f._initialFrame); // Only one frame...
	setPosition (df._position + f._adjust ); 

	_isVisible = true; 
}

// ---
void AticAtacFood::updatePositions ()
{
	if (!_isVisible)
		return; // Only when visible...

	QGAMES::Artist::updatePositions ();
}

// ---
void AticAtacFood::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!_isVisible)
		return; // Only when visible...

	QGAMES::Artist::drawOn (s, p);

	#ifndef NDEBUG
	QGAMES::Rectangle iZone = collisionZone ();
	QGAMES::Position leftUp = iZone.pos1 ();
	QGAMES::Position rightDown = iZone.pos2 ();
	QGAMES::Position leftDown (leftUp.posX (), rightDown.posY ());
	QGAMES::Position rightUp (rightDown.posX (), leftUp.posY ());
	s -> drawLine (leftUp, rightUp, __GRAYCOLOR);
	s -> drawLine (rightUp, rightDown, __GRAYCOLOR);
	s -> drawLine (rightDown, leftDown, __GRAYCOLOR);
	s -> drawLine (leftDown, leftUp, __GRAYCOLOR);
	#endif
}
