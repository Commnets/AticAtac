#include "Entities.hpp"
#include "Movements.hpp"
#include "Game.hpp"

// ---
void AticAtacMonster::Behaviour::changeDirectionConsideringLimits (QGAMES::bdata& x, QGAMES::bdata& y, 
		AticAtacMonster* e)
{
	// e has to exists...
	assert (e);

	// Calculates a potential new orientation, if the monster collisions 
	// with the borders of the screen...
	// To calculate whether the monsters collision or not with the borders
	// of the screen, the size of the monster frame is taken into account...
	MovementMonsterInRoom* m = (MovementMonsterInRoom*) e -> currentMovement ();
	assert (m);
	QGAMES::Position pos = e -> position ();
	QGAMES::Form* f = e -> currentForm ();
	if (pos.posX () >=  (m -> maxPosition ().posX () - f -> frameWidth ())) x = __BD -1;
	if (pos.posY () >=  (m -> maxPosition ().posY () - f -> frameHeight ())) y = __BD -1;
	if (pos.posX () <= m -> minPosition ().posX ()) x = __BD 1;
	if (pos.posY () <= m -> minPosition ().posY ()) y = __BD 1;
}

// ---
AticAtacMonster::AticAtacMonster (int id, const QGAMES::Forms& f, const QGAMES::Entity::Data& d) 
	: QGAMES::Artist (id, f, d),
	  _status (Status::__APPEARING),
	  _behaviour (NULL),
	  _mtDefinition (),
	  _formDefined (),
	  _numberFormDefined (0),
	  _counter (0),
	  _counterChanges (0)
{ 
	// Invisible by default...
	setVisible (false);
}

// ---
bool AticAtacMonster::canDie () const
{
	// If no behaviour...it can't dir...
	return ((_behaviour == NULL) ? false : _behaviour -> canDie ());
}

// ---
void AticAtacMonster::setAspectTo (const General::ThingDefinition& df, 
	AticAtacMonster::Status st)
{
	_mtDefinition = df;

	_numberFormDefined = rand () % _mtDefinition._forms.size ();
	_formDefined = _mtDefinition._forms [_numberFormDefined];
	setCurrentForm (_formDefined._form);
	setCurrentAspect (_formDefined._initialFrame);

	// then the position received is taken into account...
	// ...and also adjusted to the form defined.
	// Only if the previous statrus is none
	// If not,m it is supossed that there was a previous position to maintain...
	if (_status == AticAtacMonster::Status::__NONE)
		setPosition (_mtDefinition._position + _formDefined._adjust );

	_counter = 0; // To count when to change the aspect
	_status = st; // The status selected...
	_counterChanges = 
		(_status == AticAtacMonster::Status::__DIEING) ? 0 : rand () % 5; 
	// To count the changes of aspect before appearing...
	// In case of dieing just only one interaction!
	setCurrentMovement ((_status == AticAtacMonster::Status::__MOVING) 
		? __MOVEMENTMONSTERINROOMBASE + (_id - __ENTITYMONSTERBASE) : __MININT__); 
	// Start to move or not...

	// Setting up the aspect becomes the monster visible...
	setVisible (true); 
}

// ---
void AticAtacMonster::changeAspectTo (const General::ThingDefinition& df)
{
	_mtDefinition = df;

	_formDefined = _mtDefinition._forms [_numberFormDefined];
	setCurrentForm (_formDefined._form);
	setCurrentAspect (_formDefined._initialFrame);
}

// ---
void AticAtacMonster::setBehaviour (Behaviour* b, const std::vector <void*>& prms)
{
	assert (b);

	_behaviour = b;
	b -> setMonster (this);
	b -> setParameters (prms);
}

// ---
void AticAtacMonster::setMovingLimits (const QGAMES::Position& min, const QGAMES::Position& max)
{
	if (_currentMovement != NULL && 
		(_currentMovement -> id () >= __MOVEMENTMONSTERINROOMBASE && 
		 _currentMovement -> id () < (__MOVEMENTMONSTERINROOMBASE + __NUMBERMAXENTITIESMONSTER)))
		((MovementMonsterInRoom*) _currentMovement) -> setLimits (min, max);
}

// ---
QGAMES::Rectangle AticAtacMonster::collisionZone () const
{
	QGAMES::Rectangle z = QGAMES::Artist::collisionZone ();
	QGAMES::Vector d = z.pos2 () - z.pos1 ();
	return (QGAMES::Rectangle (z.pos1 () + (0.25 * d), z.pos2 () - (0.25 * d)));
	// Something simply 50% of the total available zone...
	// Let's say it referes the heart of monster's visible zone!
}

// --- 
void AticAtacMonster::initialize ()
{
	QGAMES::Artist::initialize ();

	// The aspect is set later...
	// The aspect can't be null (but it should be at this point)

	_counter = 0;
	_counterChanges = rand () % 5; // 5 max cycles to appear!
	_status = 
		AticAtacMonster::Status::__NONE;
	setCurrentMovement (__MININT__); // No movement when appearing...

	// No behaviour by default...
	_behaviour = NULL;

	setVisible (false);
}

// ---
void AticAtacMonster::updatePositions ()
{
	if (!isVisible ())
		return; // Only when visible...

	// Correct the speed 
	// With some monsters it is needed...
	if (_currentMovement != NULL && 
		dynamic_cast <MovementMonsterInRoom*> (_currentMovement) != NULL)
	((MovementMonsterInRoom*) _currentMovement) -> 
		setSpeed (_behaviour -> correctSpeed (
			((MovementMonsterInRoom*) _currentMovement) -> speed (), 
			((MovementMonsterInRoom*) _currentMovement) -> baseSpeed ()));

	// Change the aspect of the monster when it is moving...
	if (_counter++ == 6)
	{
		_counter = 0;
		if (_currentAspect++ >= _formDefined._finalFrame)
		{
			_currentAspect = _formDefined._initialFrame;
			if (_status == AticAtacMonster::Status::__APPEARING ||
				_status == AticAtacMonster::Status::__DIEING)
			{
				if (_counterChanges-- <= 0) // The number of changed has been reached...
				{
					_counterChanges = rand () % 5; // The initial number back...
					notify (QGAMES::Event
						((_status == AticAtacMonster::Status::__APPEARING) 
							? __MONSTERAPPEARED : __MONSTERDIED, this)); // Another aspect?...
				}
			}
		}
	}

	// It is time to decide whether the monster is reaching the limits or not...
	if (_currentMovement != NULL && 
		(_currentMovement -> id () >= __MOVEMENTMONSTERINROOMBASE && 
		 _currentMovement -> id () < (__MOVEMENTMONSTERINROOMBASE + __NUMBERMAXENTITIESMONSTER)))
		actualizePositionWhenMove (); // This method can be different depending on the type of monster...
	
	// Move the monster itself...
	QGAMES::Artist::updatePositions ();
}

// ---
void AticAtacMonster::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Artist::drawOn (s, p);

	#ifndef NDEBUG
	// Draw something around the monster...
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
	QGAMES::Rectangle cZone = collisionZone ();
	QGAMES::Position cLeftUp = cZone.pos1 ();
	QGAMES::Position cRightDown = cZone.pos2 ();
	QGAMES::Position cLeftDown = QGAMES::Position (cLeftUp.posX (), cRightDown.posY ());
	QGAMES::Position cRightUp = QGAMES::Position (cRightDown.posX (), cLeftUp.posY ());
	s -> drawLine (cLeftUp, cLeftDown, __GRAYCOLOR);
	s -> drawLine (cLeftDown, cRightDown, __GRAYCOLOR);
	s -> drawLine (cRightDown, cRightUp, __GRAYCOLOR);
	s -> drawLine (cRightUp, cLeftUp, __GRAYCOLOR);
	#endif
}

// ---
void AticAtacMonster::actualizePositionWhenMove ()
{
	assert (_behaviour); // A behaviour has to exist...

	// Calculates a potential new orientation for the next movement...
	QGAMES::Vector newDirection = _behaviour -> potentialNewOrientation (this);
	if (newDirection != orientation ())
	{
		setOrientation (newDirection); // This will be set up also after move...
		changeAspectTo (_behaviour -> aspectToOrientation (this));
		setMove (newDirection); // The movement changes...
	}
}

// ---
QGAMES::Vector MonsterAppearingBehaviour::potentialNewOrientation (AticAtacMonster* e)
{
	// First of all, it gets the current orientation...
	QGAMES::bdata dX = e -> orientation ().posX (); 
	QGAMES::bdata dY = e -> orientation ().posY (); 

	// In some cases, 
	// the monster can change the direction in the middle of the "fly"
	// The possibilities are 10 among 1000
	if ((rand () % 1000) < 10)
	{
		dX = ((rand () % 2) == 0 ? 1 : -1) * dX;
		dY = ((rand () % 2) == 0 ? 1 : -1) * dY;
	}

	// Calculates a potential new orientation, if the monster collisions 
	// with the borders of the screen...
	// To calculate whether the monsters collision or not with the borders
	// of the screen, the size of the monster frame is taken into account...
	changeDirectionConsideringLimits (dX, dY, e);

	// Returns the new potential direction
	// If nothing changed, the value returned will be same...
	return (QGAMES::Vector (dX, dY, __BD 0));
}

// ---
General::ThingDefinition& MonsterAppearingBehaviour::aspectToOrientation (AticAtacMonster* e)
{
	return ((e -> orientation ().posX () == 1) 
			? General::_e._appearingMonstersRight : General::_e._appearingMonstersLeft);
}

// ---
QGAMES::Vector MonsterMummyBehaviour::potentialNewOrientation (AticAtacMonster* e)
{
	// At least the first parameter has to exist...
	assert (_parameters.size () > 0);

	// The mummy follows the main character unless
	// the right element (a leaf) is in the room.
	// In this case the mmuy will go where the leaf is...
	// So first thing is to load the main player and where is the leaf...(if it is in the room)
	AticAtacCharacter* p = (AticAtacCharacter*) _parameters [0];
	assert (p); // It has to exist...
	AticAtacWorld::ThingPositions ths = 
		((AticAtacWorld*) (__AGM QGAMES::Game::game ()) -> activeWorld ()) -> thingsInCurrentRoom ();
	bool isThereLeaf = false;
	AticAtacWorld::ThingPositions::const_iterator i = ths.find (General::_e._things [__LEAFID]._id);
	if (i != ths.end ())
		isThereLeaf = true; // There is a leaf...

	// Takeing into account the previous decisions...
	// A target direction is calculated...
	QGAMES::Position tPos = p -> position ();
	if (isThereLeaf)
		tPos = (*i).second -> _position; // Where the leaf is...
	QGAMES::Vector v = tPos - e -> position ();
	QGAMES::Vector oV (v.posX () > 0 ? __BD 1 : ((v.posX () < 0) ? __BD -1 : __BD 0),
		v.posY () > 0 ? __BD 1 : ((v.posY () < 0) ? __BD -1 : __BD 0), __BD 0);
	
	// Now it is time to correct (potentially) the position...
	QGAMES::bdata dX = oV.posX (); 
	QGAMES::bdata dY = oV.posY (); 

	// Calculates a potential new orientation, if the monster collisions 
	// with the borders of the screen...
	// To calculate whether the monsters collision or not with the borders
	// of the screen, the size of the monster frame is taken into account...
	changeDirectionConsideringLimits (dX, dY, e);
	return (QGAMES::Vector (dX, dY, __BD 0));
}

// ---
QGAMES::Vector MonsterDevilBehaviour::potentialNewOrientation (AticAtacMonster* e)
{
	// At least the first parameter has to exist...
	assert (_parameters.size () > 0);

	// The devil follows the main character always
	AticAtacCharacter* p = (AticAtacCharacter*) _parameters [0];
	assert (p); // It has to exist...
	QGAMES::Position tPos = p -> position ();
	QGAMES::Vector v = tPos - e -> position ();
	QGAMES::Vector oV (v.posX () > 0 ? __BD 1 : ((v.posX () < 0) ? __BD -1 : __BD 0),
		v.posY () > 0 ? __BD 1 : ((v.posY () < 0) ? __BD -1 : __BD 0), __BD 0);
	
	// Now it is time to correct (potentially) the position...
	QGAMES::bdata dX = oV.posX (); 
	QGAMES::bdata dY = oV.posY (); 

	// Calculates a potential new orientation, if the monster collisions 
	// with the borders of the screen...
	// To calculate whether the monsters collision or not with the borders
	// of the screen, the size of the monster frame is taken into account...
	changeDirectionConsideringLimits (dX, dY, e);
	return (QGAMES::Vector (dX, dY, __BD 0));
}

// ---
QGAMES::Vector MonsterDraculaBehaviour::potentialNewOrientation (AticAtacMonster* e)
{
	// At least the first parameter has to exist...
	assert (_parameters.size () > 0);

	// The mummy follows the main character unless
	// the right element (a cross) is in the room.
	// In this case the dracula will go where the cross is...
	// So first thing is to load the main player and where is the cross...(if it is in the room)
	AticAtacCharacter* p = (AticAtacCharacter*) _parameters [0];
	assert (p); // It has to exist...
	AticAtacWorld::ThingPositions ths = 
		((AticAtacWorld*) (__AGM QGAMES::Game::game ()) -> activeWorld ()) -> thingsInCurrentRoom ();
	bool isThereCross = false;
	AticAtacWorld::ThingPositions::const_iterator i = ths.find (General::_e._things [__CROSSID]._id);
	if (i != ths.end ())
		isThereCross = true; // There is a leaf...

	// Takeing into account the previous decisions...
	// A target direction is calculated...
	QGAMES::Position tPos = p -> position ();
	if (isThereCross)
		tPos = (*i).second -> _position; // Where the leaf is...
	QGAMES::Vector v = tPos - e -> position ();
	QGAMES::Vector oV (v.posX () > 0 ? __BD 1 : ((v.posX () < 0) ? __BD -1 : __BD 0),
		v.posY () > 0 ? __BD 1 : ((v.posY () < 0) ? __BD -1 : __BD 0), __BD 0);
	
	// Now it is time to correct (potentially) the position...
	QGAMES::bdata dX = oV.posX (); 
	QGAMES::bdata dY = oV.posY (); 

	// Calculates a potential new orientation, if the monster collisions 
	// with the borders of the screen...
	// To calculate whether the monsters collision or not with the borders
	// of the screen, the size of the monster frame is taken into account...
	changeDirectionConsideringLimits (dX, dY, e);
	return (QGAMES::Vector (dX, dY, __BD 0));
}

// ---
QGAMES::Vector MonsterHunchbackBehaviour::potentialNewOrientation (AticAtacMonster* e)
{
	// At least the first parameter has to exist...
	assert (_parameters.size () > 0);

	// The hunchback follows the main character unless
	// the right element (a claw) is in the room.
	// In this case the hunch will go where the claw is...
	// So first thing is to load the main player and where is the claw...(if it is in the room)
	AticAtacCharacter* p = (AticAtacCharacter*) _parameters [0];
	assert (p); // It has to exist...
	AticAtacWorld::ThingPositions ths = 
		((AticAtacWorld*) (__AGM QGAMES::Game::game ()) -> activeWorld ()) -> thingsInCurrentRoom ();
	bool isThereClaw = false;
	AticAtacWorld::ThingPositions::const_iterator i = ths.find (General::_e._things [__CLAWID]._id);
	if (i != ths.end ())
		isThereClaw = true; // There is a leaf...

	// Takeing into account the previous decisions...
	// A target direction is calculated...
	QGAMES::Position tPos = p -> position ();
	if (isThereClaw)
		tPos = (*i).second -> _position; // Where the leaf is...
	QGAMES::Vector v = tPos - e -> position ();
	QGAMES::Vector oV (v.posX () > 0 ? __BD 1 : ((v.posX () < 0) ? __BD -1 : __BD 0),
		v.posY () > 0 ? __BD 1 : ((v.posY () < 0) ? __BD -1 : __BD 0), __BD 0);
	
	// Now it is time to correct (potentially) the position...
	QGAMES::bdata dX = oV.posX (); 
	QGAMES::bdata dY = oV.posY (); 

	// Calculates a potential new orientation, if the monster collisions 
	// with the borders of the screen...
	// To calculate whether the monsters collision or not with the borders
	// of the screen, the size of the monster frame is taken into account...
	changeDirectionConsideringLimits (dX, dY, e);
	return (QGAMES::Vector (dX, dY, __BD 0));
}

// ---
QGAMES::Vector MonsterFrankesteinBehaviour::potentialNewOrientation (AticAtacMonster* e)
{
	// At least the first parameter has to exist...
	assert (_parameters.size () > 0);

	// The franky follows the main character unless
	// the right element (a claw) is in the room.
	// In this case the franky will go where the wrench is...
	// So first thing is to load the main player and where is the wrench...(if it is in the room)
	AticAtacCharacter* p = (AticAtacCharacter*) _parameters [0];
	assert (p); // It has to exist...
	AticAtacWorld::ThingPositions ths = 
		((AticAtacWorld*) (__AGM QGAMES::Game::game ()) -> activeWorld ()) -> thingsInCurrentRoom ();
	bool isThereWrench = false;
	AticAtacWorld::ThingPositions::const_iterator i = ths.find (General::_e._things [__WRENCHID]._id);
	if (i != ths.end ())
		isThereWrench = true; // There is a leaf...

	// Takeing into account the previous decisions...
	// A target direction is calculated...
	QGAMES::Position tPos = p -> position ();
	if (isThereWrench)
		tPos = (*i).second -> _position; // Where the leaf is...
	QGAMES::Vector v = tPos - e -> position ();
	QGAMES::Vector oV (v.posX () > 0 ? __BD 1 : ((v.posX () < 0) ? __BD -1 : __BD 0),
		v.posY () > 0 ? __BD 1 : ((v.posY () < 0) ? __BD -1 : __BD 0), __BD 0);
	
	// Now it is time to correct (potentially) the position...
	QGAMES::bdata dX = oV.posX (); 
	QGAMES::bdata dY = oV.posY (); 

	// Calculates a potential new orientation, if the monster collisions 
	// with the borders of the screen...
	// To calculate whether the monsters collision or not with the borders
	// of the screen, the size of the monster frame is taken into account...
	changeDirectionConsideringLimits (dX, dY, e);
	return (QGAMES::Vector (dX, dY, __BD 0));
}
