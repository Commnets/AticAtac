#include "Scenes.hpp"
#include "Maps.hpp"
#include "Entities.hpp"
#include "Movements.hpp"
#include "Game.hpp"
#include "General.hpp"

// ---
AticAtacScene::AticAtacScene (const QGAMES::Maps& m, const QGAMES::Scenes& cn,
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: QGAMES::Scene (__ATICATACSCENE, m, cn, p, ePL),
	  _roomNumber (0),
	  _behaviours (),
	  _numberOfBkObjects (0),
	  _numberOfMonsters (0),
	  _numberOfThingsToEat (0),
	  _numberOfThingsToCatch (0),
	  _monsterStatus ()
{
	_monsterStatus.resize (__MAXNUMBEROFMONSTERSAPPEARING);
}

// ---
AticAtacScene::~AticAtacScene ()
{
	for (AticAtacMonster::Behaviours::const_iterator i = _behaviours.begin ();
			i != _behaviours.end (); i++)
		delete ((*i)); // The scene owns the monster behaviours...
}

// ---
void AticAtacScene::setRoomNumber (int r, 
		const AticAtacWorld::DoorCounters& dC,
		const AticAtacWorld::FoodCounters& fC,
		const AticAtacWorld::ThingPositions& tP,
		const AticAtacWorld::ThingPositions& tbP,
		int fR, int fO)
{
	assert (r >= 0 && r < __NUMBEROFROOMS__);
	assert (_activeMap);

	_roomNumber = r;
	((AticAtacMap*) _activeMap) -> setRoomNumber (r);

	// Set up all the background elements...
	_numberOfBkObjects = setUpBackgroundObjects (dC, tbP, fR, fO); 
	// The situation of the doors is received as parameter...
	// Set up the monsters...
	_numberOfMonsters = setUpMonsters (tP); 
	// The position of the things in the room is received...
	// Set up the things to be eaten...
	_numberOfThingsToEat = setUpThingsToEat (fC); 
	// The real situation of the food in the room is received...
	// Set up the things to catch...
	_numberOfThingsToCatch = setUpThingsToCatch (tP); 
	// The real position of the thinbgs in the room is received...

	// The room starts...
	// And it starts also for the monsters inside...
	// The appearing monsters will take some ramdom couple of seconds to appear...
	for (int i = 0; 
		i < ((AticAtacGame*) QGAMES::Game::game ()) -> levelInfo ()._maxNumberOfAppearingMonsters; i++)
	{
		_monsterStatus [i]._entity = __ENTITYMONSTERBASE + _numberOfMonsters++;
		// All monsters will take less than 7 seconds to appear...
		_monsterStatus [i]._maxCount = 2 + (rand () % 
			(((AticAtacGame*) QGAMES::Game::game ()) -> levelInfo ()._maxTimeToAppearMonsters - 2));  
		// from 2 to "what-configuration-says" seconds to appear...
		_monsterStatus [i]._count = 0;
	}

	// Get the doors and the blocking elements from the background entities...
	BackgroundEntities doors, blockElements;
	for (int i = 0; i < _numberOfBkObjects; i++)
	{
		AticAtacBackgroundEntity* bk = 
			(AticAtacBackgroundEntity*) _entities [__ENTITYBACKGROUNDBASE + i];
		if (bk -> isADoor ())
		{
			doors.push_back (bk);
			// If the object connected is where the player is comming from, 
			// ...it has to be opened...if it is a door that open and close automatically
			// and even if the status is to be closed...
			if (fO != -1 && bk -> objConnectedId () == fO && bk -> doorOpenClose ()) 
				bk -> openDoor (true); // This is also has done before, but just in case...
		}
		else
		if (bk -> blockMovement ())
			blockElements.push_back (bk);
	}

	// The main character is set (initially) at the middle of the screen...
	// And the limits of the movement are also set...
	AticAtacCharacter* mChar = (AticAtacCharacter*) _entities [__ENTITYPLAYER]; 
	((MovementPlayerInRoom*) mChar -> currentMovement ()) -> setLimits (roomLimits ());
	mChar -> setPosition (centerOfRoom () - 
			QGAMES::Vector (__BD (mChar -> currentForm () -> frameWidth () / 2),
				__BD (mChar -> currentForm () -> frameHeight () / 2), __BD 0));
	// The entity has to take into account where the doors are
	// In other to determinate if he can or not moves...
	// See the player entity for further details...
	mChar -> setDoors (doors);
	// The entity has to take into account where the blocking elements are
	// In other to determinate whether it can or not move
	// In a concrete position...
	mChar -> setBlockingElements (blockElements);

	// Assings the limit to the movement of the weapon,
	// ...and set them invisible..
	for (int i = (__ENTITYPLAYER + 1); i < (__ENTITYPLAYER + __NUMBERMAXENTITIESPLAYER); i++)
	{
		AticAtacWeapon* w = (AticAtacWeapon*) _entities [i]; 
		((MovementWeaponInRoom*) w -> currentMovement ()) 
			-> setLimits (roomLimits ()); 
		((AticAtacWeapon*) _entities [i]) -> setVisible (false); 
	}
}

// ---
void AticAtacScene::actualizeDoorStatus (int iD, bool oC)
{
	for (int i = 0; i < _numberOfBkObjects; i++)
	{
		AticAtacBackgroundEntity* e = (AticAtacBackgroundEntity*) 
			_entities [__ENTITYBACKGROUNDBASE + i];
		if (e -> aspectId () == iD && oC != e -> isDoorOpen ())
			e -> openDoor (oC); // The counter is put back to 0 at entity level...
	}
}

// ---
QGAMES::Position AticAtacScene::centerOfRoom () const
{
	QGAMES::Rectangle rL = roomLimits ();
	return (rL.pos1 () + ((rL.pos2 () - rL.pos1 ()) / 2));
}

// ---
void AticAtacScene::createAppearingMonster (int e)
{
	AticAtacMonster* m = (AticAtacMonster*) _entities [e];
	m -> setAspectTo (General::_e._appearing); // The monster starts to appear...
	m -> setPosition (randomPositionAtRoomFor (m));
	// ...in a random position in the scene...
}

// ---
void AticAtacScene::setUpThing (const General::ThingDefinition& t, const QGAMES::Position& p)
{
	int wF = -1;
	for (int i = 0; i < _numberOfThingsToCatch && wF == -1; i++)
		if (!((AticAtacThingToCatch*) _entities [__ENTITYCATCHBASE + i]) -> isVisible ())
			wF = i; // a hole has found!...
	AticAtacThingToCatch* th = 
		(AticAtacThingToCatch*) _entities [__ENTITYCATCHBASE +  ((wF != -1) 
			? wF : _numberOfThingsToCatch++)]; 
	// If no hole was found in the current thigns to catch, then a new thing to catch apperar...
	th -> setAspectTo (t);
	th -> setPosition (p);
}

// ---
void AticAtacScene::shoot (int nS)
{
	AticAtacWeapon* e = 
		(AticAtacWeapon*) _entities [__ENTITYPLAYER + 1 + nS];
	AticAtacCharacter* mC = 
		(AticAtacCharacter*) _entities [__ENTITYPLAYER];
	e -> setPosition (mC -> position ());
	e -> setType ((mC -> type () == AticAtacCharacter::__KNIGHT) ? AticAtacWeapon::__WEAPONKNIGHT
		: ((mC -> type () == AticAtacCharacter::__WIZARD) ? AticAtacWeapon::__WEAPONWIZARD : AticAtacWeapon::__WEAPONSERVANT));
	// The aspect of the weapon will depend on the type of character playing...

	// Initially the weapon will follow the direction of the player
	// unless the player is stopped. 
	// In that case, the direction will be random...
	QGAMES::Vector o = mC -> orientation ();
	int pOX = ((rand () % 2) == 0) ? 1 : -1;
	int pOY = ((rand () % 2) == 0) ? 1 : -1; 
	e -> setMove ((o == QGAMES::Vector::_cero) ? 
		QGAMES::Vector (__BD pOX, __BD pOY, __BD 0)  : mC -> orientation ());

	// Starts to move when visible...
	e -> setVisible (true); 

	// Fire!
	QGAMES::Game::game () -> sound (__FIRESOUND) -> play (__SHOOTCHANNLESOUND);
}

// ---
void AticAtacScene::noShoot (int nS)
{
	// Weapon stops moving when get invisible...
	((AticAtacWeapon*) _entities [__ENTITYPLAYER + 1 + nS]) -> setVisible (false);
}

// ---
void AticAtacScene::initialize ()
{
	setMap (__ATICATACMAP);
	QGAMES::Scene::initialize ();

	_roomNumber = 0;
	_numberOfBkObjects = _numberOfMonsters = 
		_numberOfThingsToCatch = _numberOfThingsToEat = 0;

	// Creates the behaviours...if they have not been created already...
	if (_behaviours.empty ())
	{
		_behaviours.resize (__NUMBEROFMONSTERBAHAVIOURS);
		_behaviours [__APPEARINGBEHAVIOUR] = new MonsterAppearingBehaviour ();
		_behaviours [__MUMMYBEHAVIOUR] = new MonsterMummyBehaviour ();
		_behaviours [__DEVILBEHAVIOUR] = new MonsterDevilBehaviour ();
		_behaviours [__DRACULABEHAVIOUR] = new MonsterDraculaBehaviour ();
		_behaviours [__HUNCHBACKBEHAVIOUR] = new MonsterHunchbackBehaviour ();
		_behaviours [__FRANKESTEINBEHAVIOUR] = new MonsterFrankesteinBehaviour ();
		_behaviours [__MUSHROOMBEHAVIOUR] = new MonsterMushroomBehaviour ();
	}
}

// ---
void AticAtacScene::updatePositions ()
{
	// The appearing monsters moves...
	for (int i = 0; 
		i < ((AticAtacGame*) QGAMES::Game::game ()) -> levelInfo ()._maxNumberOfAppearingMonsters; i++)
	{
		AticAtacMonster* m = (AticAtacMonster*) _entities [_monsterStatus [i]._entity];
		if (!m -> isVisible ())
		{
			if (_monsterStatus [i]._count++ >= 
					(_monsterStatus [i]._maxCount * QGAMES::Game::game () -> framesPerSecond ()))
			{
				_monsterStatus [i]._count = 0;
				createAppearingMonster (_monsterStatus [i]._entity);
			}
		}
	}

	// Does what it is standard...
	QGAMES::Scene::updatePositions ();
}

// ---
void AticAtacScene::processEvent (const QGAMES::Event& e)
{
	assert (e.data ());

	// When a monster has appeared...
	// Then it starts to move!
	if (e.code () == __MONSTERAPPEARED)
	{
		// Sounds the monster...
		QGAMES::Game::game () -> sound (__MONSTERAPPEARINGBASESOUND + (rand () % 5)) 
			-> play (__MONSTERAPPEARINGCHANNELSOUND);

		// When the monster appears the direction of the movement
		// is something random...
		AticAtacMonster* m = (AticAtacMonster*) e.data ();
		int dX = ((rand () % 3) == 2) ? -1 : 1; // The direction in the x axis...
		int dY = ((rand () % 3) == 2) ? -1 : 1; // The direction in the y axis...
		m -> setAspectTo ((dX == 1) 
				? General::_e._appearingMonstersRight : General::_e._appearingMonstersLeft,
			AticAtacMonster::Status::__MOVING);
		m -> setBehaviour (_behaviours [__APPEARINGBEHAVIOUR]);
		m -> setOrientation (QGAMES::Vector (__BD dX, __BD dY, __BD 0));
		m -> setMove (QGAMES::Vector (__BD dX, __BD dY, __BD 0)); // The acceleration is 0...
		m -> setMovingLimits (roomLimits ()); // In the room...
	}
	else
	if (e.code () == __MONSTERDIED)
	{
		AticAtacMonster* m = (AticAtacMonster*) e.data ();
		m -> setVisible (false); // The monster becomes invisible...
	}
	else
	if (e.code () == __PLAYERENTERINGDOOR)
		notify (e); // Notify it to the world to change the room...
}

// ---
QGAMES::Position AticAtacScene::randomPositionAtRoom () const
{
	General::RoomDefinition r = General::_e._rooms [_roomNumber];
	return (QGAMES::Position (__BD ((r._cx - r._w) + (rand () % (2 * r._w))),
		__BD ((r._cy - r._h) + (rand () % (2 * r._h)))));
}

// ---
QGAMES::Position AticAtacScene::randomPositionAtRoomFor (QGAMES::Entity* a) const
{
	assert (a);

	// THis a random position but taking into account the size of the monster...
	QGAMES::Position p = randomPositionAtRoom ();
	General::RoomDefinition r = General::_e._rooms [_roomNumber];
	int lX = 0; int lY = 0;
	if (p.posX () > r._cx) 
		lX = a -> currentForm () -> frameWidth ();
	if (p.posY () > r._cy) 
		lY = a -> currentForm () -> frameHeight ();
	return (p - QGAMES::Position (__BD lX, __BD lY, __BD 0));
}

// ---
QGAMES::Rectangle AticAtacScene::roomLimits () const
{
	General::RoomDefinition r = General::_e._rooms [_roomNumber];
	QGAMES::Position p1 (__BD (r._cx - r._w), __BD (r._cy - r._h), __BD 0);
	QGAMES::Position p2 = p1 + QGAMES::Vector (__BD (2 * r._w), __BD (2 * r._h), __BD 0);
	return (QGAMES::Rectangle (p1, p2));
}

// ---
int AticAtacScene::setUpBackgroundObjects (const AticAtacWorld::DoorCounters& dC, 
	const AticAtacWorld::ThingPositions& tbP, int fR, int fO)
{
	// Set up all the background elements...
	int nBkObjects = 0;
	General::Objects objs = General::_e._rooms [_roomNumber]._objects;
	for (General::Objects::const_iterator i = objs.begin (); i != objs.end (); i++)
	{
		General::ObjectDefinition oDef = (*i);
		AticAtacBackgroundEntity* e = (AticAtacBackgroundEntity*) 
			_entities [__ENTITYBACKGROUNDBASE + nBkObjects++];
		e -> setAspectTo (oDef);
		AticAtacWorld::DoorCounters::const_iterator j = dC.find (oDef._id);
		if (j != dC.end ())
			e -> openDoor ((*j).second -> _openClose);
	}

	// Adds the tombs...
	// that there are in the room.
	for (AticAtacWorld::ThingPositions::const_iterator j = tbP.begin (); 
			j != tbP.end (); j++)
	{
		if ((*j).second -> _room == _roomNumber)
		{
			AticAtacBackgroundEntity* e = (AticAtacBackgroundEntity*) 
				_entities [__ENTITYBACKGROUNDBASE + nBkObjects++];
			General::ObjectDefinition t = General::_e._tombStone;
			t._id = (*j).second -> _id;
			t._position = (*j).second -> _position;
			e -> setAspectTo (t);
		}
	}

	// The rest of the background objects are not visible...
	for (int i = nBkObjects; i < __NUMBERMAXENTITIESBK; i++)
		((AticAtacBackgroundEntity*) _entities [__ENTITYBACKGROUNDBASE + i]) -> setVisible (false);

	// Return the number of Bk Objects set up...
	return (nBkObjects);
}

// ---
int AticAtacScene::setUpMonsters (const AticAtacWorld::ThingPositions& tP)
{
	int nMonsters = 0;
	for (int i = 0; i < __NUMBERMAXENTITIESMONSTER; i++)
		((AticAtacMonster*) _entities [__ENTITYMONSTERBASE + i]) -> initialize (); // Everything is initialized...

	// The mummy is always where the red key is...
	// ...and if the red key is not in the room already, the mummy doesn't appear!
	if (tP.find (General::_e._things [__REDKEYID]._id) != tP.end ()) 
	{	
		AticAtacMonster* mnt = 
			(AticAtacMonster*) _entities [__ENTITYMONSTERBASE + nMonsters++];
		std::vector <void*> prms; prms.push_back ((void*) _entities [__ENTITYPLAYER]);
		mnt	-> setAspectTo (General::_e._mummy, AticAtacMonster::Status::__MOVING);
		mnt -> setBehaviour (_behaviours [__MUMMYBEHAVIOUR], prms);
	}

	// Other monsters...
	General::Things thsM = General::_e._rooms [_roomNumber]._monsters;
	for (General::Things::const_iterator i = thsM.begin (); i != thsM.end (); i++)
	{
		if ((*i)._whatIs != General::WhatIs::__MUMMY) // The mummy was set before...
		{
			AticAtacMonster* mnt = 
				(AticAtacMonster*) _entities [__ENTITYMONSTERBASE + nMonsters++];
			std::vector <void*> prms; prms.push_back ((void*) _entities [__ENTITYPLAYER]);
			// A reference to the main player is always a parameter needed...
			if ((*i)._whatIs == General::WhatIs::__DEVIL)
				mnt -> setBehaviour (_behaviours [__DEVILBEHAVIOUR], prms);
			else
			if ((*i)._whatIs == General::WhatIs::__DRACULA)
				mnt -> setBehaviour (_behaviours [__DRACULABEHAVIOUR], prms);
			else
			if ((*i)._whatIs == General::WhatIs::__HUNCH)
				mnt -> setBehaviour (_behaviours [__HUNCHBACKBEHAVIOUR], prms);
			else
			if ((*i)._whatIs == General::WhatIs::__FRANK)
				mnt -> setBehaviour (_behaviours [__FRANKESTEINBEHAVIOUR], prms);
			else
				mnt -> setBehaviour (_behaviours [__MUSHROOMBEHAVIOUR], prms);
			mnt -> setAspectTo ((*i), AticAtacMonster::Status::__MOVING);
		}
	}

	// The rest of the potential monsters are not visible...so far...
	for (int i = nMonsters; i < __NUMBERMAXENTITIESMONSTER; i++)
		((AticAtacMonster*) _entities [__ENTITYMONSTERBASE + i]) -> setVisible (false);

	// The monsters can't move out of the room limits...
	for (int i = 0; i < nMonsters; i++)
		((AticAtacMonster*) _entities [__ENTITYMONSTERBASE + i]) -> setMovingLimits (roomLimits ());

	// Returns the number of monsters set up...
	return (nMonsters);
}

// ---
int AticAtacScene::setUpThingsToEat (const AticAtacWorld::FoodCounters& fC)
{
	int nThingsToEat = 0;
	General::Things thsE = General::_e._rooms [_roomNumber]._thingsToEat;
	for (General::Things::const_iterator i = thsE.begin (); i != thsE.end (); i++)
	{
		General::ThingDefinition tD = (*i);
		AticAtacFood* f = (AticAtacFood*) _entities [__ENTITYEATENBASE + nThingsToEat++];
		AticAtacWorld::FoodCounters::const_iterator j = fC.find (tD._id);
		f -> setAspectTo (tD);
		if (j != fC.end ()) // it can't be other thing...
		{
			// If the food has been eaten, then it is not visible, 
			// even having an aspect already defined...
			if ((*j).second -> _eaten)
				f -> setVisible (false); 
		}
	}

	// The rest of the things to eat are not visible...
	for (int i = nThingsToEat; i < __NUMBERMAXENTITIESEAT; i++)
		((AticAtacFood*) _entities [__ENTITYEATENBASE + i]) -> setVisible (false);

	// Returns the number of things set up...
	return (nThingsToEat);
}

// ---
int AticAtacScene::setUpThingsToCatch (const AticAtacWorld::ThingPositions& tP)
{
	// The things in the room are what have been passed as parameter...
	// However the definitions are general...
	int nThingsToCatch = 0;
	for (AticAtacWorld::ThingPositions::const_iterator i = tP.begin (); i != tP.end (); i++)
	{
		AticAtacWorld::ThingPosition* p = (*i).second;
		General::ThingDefinition tD = General::_e._things [p -> _id];
		AticAtacThingToCatch* e = 
			(AticAtacThingToCatch*) _entities [__ENTITYCATCHBASE + nThingsToCatch++];
		tD._position = p -> _position; // The position is the one where the object was left...
		e -> setAspectTo (tD); // The aspect is the one of the thing... 
	}

	// Then the rest of the things to catch in the room, are definetivley not visible...
	for (int i = nThingsToCatch; i < __NUMBERMAXENTITIESCATCH; i++)
		((AticAtacThingToCatch*) _entities [__ENTITYCATCHBASE + i]) -> setVisible (false);

	// Returns the number of things to catch set up...
	return (nThingsToCatch);
}
