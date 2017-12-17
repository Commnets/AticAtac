#include "Entities.hpp"
#include "Movements.hpp"

// ---
void AticAtacFood::setAspectTo (const General::ThingDefinition& df)
{
	_dObjDefinition = df;

	General::FormsDefinitionList fs = df._forms;
	General::FormDefinition f = fs [rand () % fs.size ()]; // The food can have several forms...
	setCurrentForm (f._form);
	setCurrentAspect (f._initialFrame); // Only one frame...
	setPosition (df._position + f._adjust ); 

	setVisible (true); 
}

// ---
QGAMES::Rectangle AticAtacFood::collisionZone () const
{
	QGAMES::Rectangle z = QGAMES::Artist::collisionZone ();
	QGAMES::Vector d = z.pos2 () - z.pos1 ();
	return (QGAMES::Rectangle (z.pos1 () + (0.25 * d), z.pos2 () - (0.25 * d)));
	// Something simply 50% of the total available zone...
	// Let's say it referes the heart of food's visible zone!
}

// ---
void AticAtacFood::updatePositions ()
{
	if (!isVisible ())
		return; // Only when visible...

	QGAMES::Artist::updatePositions ();
}

// ---
void AticAtacFood::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Artist::drawOn (s, p);

	#ifndef NDEBUG
	// Draw something around the food...
	// Only in debug mode...
	QGAMES::Position leftUp = position ();
	QGAMES::Position leftDown = position () + 
		QGAMES::Vector (__BD 0, __BD _currentForm -> frameHeight (), __BD 0);
	QGAMES::Position rightUp = position () + 
		QGAMES::Vector (__BD _currentForm -> frameWidth (), __BD 0, __BD 0);
	QGAMES::Position rightDown = position () + 
		QGAMES::Vector (__BD _currentForm -> frameWidth (), 
		__BD _currentForm -> frameHeight (), __BD 0);
	s -> drawLine (leftUp, leftDown, __GRAYCOLOR);
	s -> drawLine (leftDown, rightDown, __GRAYCOLOR);
	s -> drawLine (rightDown, rightUp, __GRAYCOLOR);
	s -> drawLine (rightUp, leftUp, __GRAYCOLOR);

	// The collision zone is also drawn...
	QGAMES::Rectangle iZone = collisionZone ();
	QGAMES::Position cLeftUp = iZone.pos1 ();
	QGAMES::Position cRightDown = iZone.pos2 ();
	QGAMES::Position cLeftDown (cLeftUp.posX (), cRightDown.posY ());
	QGAMES::Position cRightUp (cRightDown.posX (), cLeftUp.posY ());
	s -> drawLine (cLeftUp, cRightUp, __GRAYCOLOR);
	s -> drawLine (cRightUp, cRightDown, __GRAYCOLOR);
	s -> drawLine (cRightDown, cLeftDown, __GRAYCOLOR);
	s -> drawLine (cLeftDown, cLeftUp, __GRAYCOLOR);
	#endif
}
