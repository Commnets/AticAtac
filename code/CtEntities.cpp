#include "Entities.hpp"
#include "Movements.hpp"

// ---
void AticAtacThingToCatch::setAspectTo (const General::ThingDefinition& df)
{
	_dObjDefinition = df;
	General::FormDefinition f = *(df._forms.begin ());
	setCurrentForm (f._form);
	setCurrentAspect (f._initialFrame); // Only one frame...
	setPosition (df._position + f._adjust ); 

	setVisible (true); 
}

// ---
void AticAtacThingToCatch::updatePositions ()
{
	if (!isVisible ())
		return; // Only when it is visible...

	QGAMES::Artist::updatePositions ();
}

// ---
void AticAtacThingToCatch::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Artist::drawOn (s, p);

	// Just to debug the limits are drawn
	// when the system is in debug mode and the object is a door...
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
