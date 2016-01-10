#include "Worlds.hpp"
#include "Scenes.hpp"
#include "General.hpp"
#include "Events.hpp"
#include "Maps.hpp"
#include "GameStates.hpp"
#include "Game.hpp"
#include <Arcade/arcadegame.hpp>
#include <Arcade/artist.hpp>
#include <iostream>

// ---
AticAtacWorld::AticAtacWorld (const QGAMES::Scenes& s, const QGAMES::WorldProperties& p)
	: QGAMES::World (__ATICATACWORLD, s, p),
	  _roomNumber (0),
	  _mainCharacter (NULL),
	  _doorCounters (), _foodCounters (), _thingPositions (),
	  _doorCountersSimple (), _foodCountersSimple (), _thingPositionsSimple (),
	  _tombs (), _tombCounter (0)
{ 
	_exitDoorIds [0] = _exitDoorIds [1] = -1;

	QGAMES::Buoys lB;
	lB.insert (QGAMES::Buoys::value_type 
		(__TOCHANGEROOMBUOYID, new AticAtacWorld::ToChangeRoomBuoy));
	lB.insert (QGAMES::Buoys::value_type 
		(__TOFFAILBUOYID, new AticAtacWorld::ToFallBuoy));
	lB.insert (QGAMES::Buoys::value_type 
		(__TOFINISHBUOYID, new AticAtacWorld::ToFinishBuoy));
	setBuoys (lB);
}

// ---
QGAMES::Position AticAtacWorld::centerOfCurrentRoom () const
{
	return ((_activeScene == NULL) // No way to know the center... 
		? QGAMES::Position::_cero : ((AticAtacScene*) _activeScene) -> centerOfRoom ());
}

// ---
void AticAtacWorld::setRoomNumber (int r, int fR, int fO)
{
	assert (r >= 0 && r < __NUMBEROFROOMS__);
	assert (_activeScene);

	// The room number where the player is in...
	_roomNumber = r;

	#ifndef NDEBUG
	std::cout << "Room Number: " << _roomNumber << std::endl;
	#endif

	// The real status of the doors in the selected room...
	AticAtacWorld::DoorCounters dC;
	AticAtacWorld::DoorCountersPerRoom::const_iterator i = _doorCounters.find (_roomNumber);
	if (i != _doorCounters.end ())
		dC = (*i).second;

	// The real stutus of the food in the room...
	AticAtacWorld::FoodCounters fC;
	AticAtacWorld::FoodCountersPerRoom::const_iterator j = _foodCounters.find (_roomNumber);
	if (j != _foodCounters.end ())
		fC = (*j).second;

	// The real things in the room
	// Remember things can be caught and left...
	AticAtacWorld::ThingPositions tP;
	AticAtacWorld::ThingPositionsPerRoom::const_iterator k = _thingPositions.find (_roomNumber);
	if (k != _thingPositions.end ())
		tP = (*k).second;
	// But that things have to be foltered with the things the main character has...
	AticAtacWorld::ThingPositions tPD;
	for (AticAtacWorld::ThingPositions::const_iterator l = tP.begin (); l != tP.end (); l++)
		if (_carrying.find ((*l).first) == _carrying.end ())
			tPD.insert (AticAtacWorld::ThingPositions::value_type ((*l).first, (*l).second));

	// Sets the situation in the scene...
	// With the real situation...
	// The position of the tombs is passed also as parameter
	((AticAtacScene*) _activeScene) -> setRoomNumber (r, dC, fC, tPD, _tombs, fR, fO);
}

// ---
void AticAtacWorld::moveCharacter (const QGAMES::Vector& d)
{
	_mainCharacter -> setMove (d);
}

// ---
void AticAtacWorld::foodEaten (AticAtacFood* f)
{
	assert (f);

	f -> setVisible (false);
	AticAtacWorld::FoodCounters::const_iterator i = _foodCountersSimple.find (f -> aspectId ());
	assert (i != _foodCountersSimple.end ());
	(*i).second -> _eaten = true;
}

// ---
void AticAtacWorld::thingCaught (AticAtacThingToCatch* t)
{
	assert (t);

	// Double check if we have already
	// The max number of things allowed..
	if (_carrying.size () >= __MAXNUMBEROFCARRYTHINGS)
		return; // It is...no more things are allowed...so it returns
	// Before getting another one, one has to be left

	// The main character has to have the element...
	_mainCharacter -> getThing (t -> whatIs ());
	// The element is already part of the elements to show in the score frame...
	AticAtacWorld::ThingPositions::const_iterator i = 
		_thingPositionsSimple.find (t -> aspectId ());
	assert (i != _thingPositionsSimple.end ());
	_carrying.insert (AticAtacWorld::ThingPositions::value_type ((*i).first, (*i).second));
	// ...and finally the element is not longer visible in that screen...
	t -> setVisible (false); 
}

// ---
void AticAtacWorld::thingLeft ()
{
	assert (_mainCharacter);
	assert (_activeScene);

	AticAtacWorld::ThingPositions::const_reverse_iterator i = _carrying.rbegin ();
	if (i != _carrying.rend ())
	{
		AticAtacWorld::ThingPosition* t = (*i).second;

		// The thing can be cought in a room different than it was got...
		// So the element is extracted from the list of objects per room...
		// ...and the end of the method inserted it back in the new right room
		AticAtacWorld::ThingPositionsPerRoom::const_iterator j = _thingPositions.find (t -> _room);
		assert (j != _thingPositions.end ()); // The element has to exist (just to double check)
		AticAtacWorld::ThingPositions tPR = (*j).second; // Get the current elements of the room
		_thingPositions.erase (j); // Erase the full list of elements of the room
		AticAtacWorld::ThingPositions::const_iterator k = tPR.find (t -> _id);
		assert (k != tPR.end ()); // The element has to exist (just to double check)
		tPR.erase (k); // Erase the specific element from the list of objects in that old room
		_thingPositions.insert (AticAtacWorld::ThingPositionsPerRoom::value_type (t -> _room, tPR));
		// ...and insert back a new list of objects in the room, without the old element

		// The thing is left in the current room...
		t -> _room = _roomNumber; 
		// The thing is left at the current player position...
		t -> _position = _mainCharacter -> position (); 
		// The player has not the thing any more...
		_mainCharacter -> leaveThing (General::_e._things [t -> _id]._whatIs);
		// The way to erase something using a reverse iterator...
		_carrying.erase ((++i).base ());

		// The thing is inserted back in the right new room...
		j = _thingPositions.find (t -> _room);
		tPR = (j == _thingPositions.end ()) ? AticAtacWorld::ThingPositions () : (*j).second;
		if (j != _thingPositions.end ()) _thingPositions.erase (j);
		tPR.insert (AticAtacWorld::ThingPositions::value_type (t -> _id, t));
		_thingPositions.insert (AticAtacWorld::ThingPositionsPerRoom::value_type (t -> _room, tPR));
		
		// The thing is set up into the current room...
		((AticAtacScene*) _activeScene) -> setUpThing (General::_e._things [t -> _id],
			_mainCharacter -> position ());
	}
}

// ---
AticAtacWorld::ThingPositions AticAtacWorld::thingsInCurrentRoom () const
{
	AticAtacWorld::ThingPositions result;
	AticAtacWorld::ThingPositionsPerRoom::const_iterator i = 
		_thingPositions.find (_roomNumber);
	if (i != _thingPositions.end ())
		result = (*i).second;
	return (result);
}

// ---
void AticAtacWorld::addTomb ()
{
	assert (_mainCharacter);

	AticAtacWorld::ThingPosition* nT = new ThingPosition;
	nT -> _id = _tombCounter++;
	nT -> _room = _roomNumber;
	nT -> _position = _mainCharacter -> position (); // Where the player was...or is still!...
	_tombs.insert (AticAtacWorld::ThingPositions::value_type (nT -> _id, nT));
}

// ---
void AticAtacWorld::openExitDoor ()
{
	assert (_exitDoorIds [0] != -1 && _exitDoorIds [1] != -1);

	AticAtacWorld::DoorCounter* dC1 = (*_doorCountersSimple.find (_exitDoorIds [0])).second;
	AticAtacWorld::DoorCounter* dC2 = (*_doorCountersSimple.find (_exitDoorIds [1])).second;
	if (dC1 -> _openClose && dC2 -> _openClose)
		return; // It is already open...

	dC1 -> _openClose = dC1 -> _openClose = true;
	if (dC1 -> _room == _roomNumber)
		((AticAtacScene*) _activeScene)
			-> actualizeDoorStatus (dC1 -> _id, dC1 -> _openClose);
	if (dC2 -> _room == _roomNumber)
		((AticAtacScene*) _activeScene)
			-> actualizeDoorStatus (dC2 -> _id, dC2 -> _openClose);
	QGAMES::Game::game () -> sound (__EXITDOOROPENSOUND) -> play (__OPENCLOSEDOORCHANNEL);
}

// ---
void AticAtacWorld::closeExitDoor ()
{

	AticAtacWorld::DoorCounter* dC1 = (*_doorCountersSimple.find (_exitDoorIds [0])).second;
	AticAtacWorld::DoorCounter* dC2 = (*_doorCountersSimple.find (_exitDoorIds [1])).second;
	if (!dC1 -> _openClose && !dC2 -> _openClose)
		return; // It is already close...

	dC1 -> _openClose = dC1 -> _openClose = false;
	if (dC1 -> _room == _roomNumber)
		((AticAtacScene*) _activeScene)
			-> actualizeDoorStatus (dC1 -> _id, dC1 -> _openClose);
	if (dC2 -> _room == _roomNumber)
		((AticAtacScene*) _activeScene)
			-> actualizeDoorStatus (dC2 -> _id, dC2 -> _openClose);
	QGAMES::Game::game () -> sound (__EXITDOORCLOSESOUND) -> play (__OPENCLOSEDOORCHANNEL);
}

// ---
void AticAtacWorld::shoot (int nS)
{
	assert (_activeScene);

	((AticAtacScene*) _activeScene) -> shoot (nS);
}

// ---
void AticAtacWorld::noShoot (int nS)
{
	assert (_activeScene);

	((AticAtacScene*) _activeScene) -> noShoot (nS);
}

// ---
void AticAtacWorld::initialize ()
{
	// The initial scene...
	setScene (__ATICATACSCENE);

	// Set up all the entities in the world
	// The elements in background...
	for (int i = __ENTITYBACKGROUNDBASE;
			i < (__ENTITYBACKGROUNDBASE + __NUMBERMAXENTITIESBK); i++)
		_activeScene -> addEntity (__AT (__AGM QGAMES::Game::game ()) -> artist (i));

	// Set up all the monsters...
	for (int i = __ENTITYMONSTERBASE;
		i < (__ENTITYMONSTERBASE + __NUMBERMAXENTITIESMONSTER); i++)
		_activeScene -> addEntity (__AT (__AGM QGAMES::Game::game ()) -> artist (i));

	// Set up all the entities to be eaten...
	for (int i = __ENTITYEATENBASE;
		i < (__ENTITYEATENBASE + __NUMBERMAXENTITIESEAT); i++)
		_activeScene -> addEntity (__AT (__AGM QGAMES::Game::game ()) -> artist (i));

	// Set up all the entities to be caught...
	for (int i = __ENTITYCATCHBASE;
		i < (__ENTITYCATCHBASE + __NUMBERMAXENTITIESCATCH); i++)
		_activeScene -> addEntity (__AT (__AGM QGAMES::Game::game ()) -> artist (i));

	// Set up the main character...
	// It will be manage during the game
	_mainCharacter = 
		(AticAtacCharacter*) (__AGM QGAMES::Game::game ()) 
			-> artist (__ENTITYPLAYER);
	_mainCharacter -> setType (AticAtacCharacter::Type::__KNIGHT); // By default it is a knight...
	_mainCharacter -> setCurrentMovement (__MOVEMENTCHARACTER);
	_activeScene -> addEntity (_mainCharacter);

	// Adds the rest of the elements in the scene but
	// But they are not visible...
	for (int i = __ENTITYPLAYER + 1; i < __ENTITYPLAYER + __NUMBERMAXENTITIESPLAYER; i++)
	{
		QGAMES::Artist* ep =
			__AT (__AGM QGAMES::Game::game ()) -> artist (i);
		AticAtacWeapon* w = (AticAtacWeapon*) ep;
		w -> setType (AticAtacWeapon::Type::__WEAPONKNIGHT); // By default is the weapon of a knight...
		w -> setCurrentMovement (__MOVEMENTWEAPON + i - __ENTITYPLAYER - 1);
		_activeScene -> addEntity (ep);
	}

	// Initialize the things
	// This method will be also invoked when the game starts back...
	initializeThings ();

	// The world is then initialized, 
	// and all scenes, world, and entities with it...
	QGAMES::World::initialize ();

	// When the standard word is istialized, the initialization
	// of the scene is called, and the the map assigned.
	// So, here it is the time to add the map to the entities...
	// If you do that before, a null map wil be added...
	for (int i = __ENTITYBACKGROUNDBASE;
			i < (__ENTITYBACKGROUNDBASE + __NUMBERMAXENTITIESBK); i++)
		(__AT (__AGM QGAMES::Game::game ()) -> artist (i)) -> setMap (_activeScene -> activeMap ());
	for (int i = __ENTITYMONSTERBASE;
		i < (__ENTITYMONSTERBASE + __NUMBERMAXENTITIESMONSTER); i++)
		(__AT (__AGM QGAMES::Game::game ()) -> artist (i)) -> setMap (_activeScene -> activeMap ());
	for (int i = __ENTITYEATENBASE;
		i < (__ENTITYEATENBASE + __NUMBERMAXENTITIESEAT); i++)
		(__AT (__AGM QGAMES::Game::game ()) -> artist (i)) -> setMap (_activeScene -> activeMap ());
	for (int i = __ENTITYCATCHBASE;
		i < (__ENTITYCATCHBASE + __NUMBERMAXENTITIESCATCH); i++)
		(__AT (__AGM QGAMES::Game::game ()) -> artist (i)) -> setMap (_activeScene -> activeMap ());
	_mainCharacter -> setMap (_activeScene -> activeMap ());
	for (int i = __ENTITYPLAYER + 1; i < __ENTITYPLAYER + __NUMBERMAXENTITIESPLAYER; i++)
		(__AT (__AGM QGAMES::Game::game ()) -> artist (i)) -> setMap (_activeScene -> activeMap ());

	// After everything the initial room is set to 0
	// Only the right thing in that room will be visible...
	setRoomNumber (__FIRSTROOM);
}

// ---
void AticAtacWorld::initializeThings ()
{
	clearThings ();

	// Initialize the door status...
	for (int i = 0; i < __NUMBEROFOBJECTS__; i++)
	{
		General::ObjectDefinition obj = General::_e._objects [i];
		if (obj._whatIs == General::WhatIs::__NORMALDOR ||
			obj._whatIs == General::WhatIs::__TRAPDOOR ||
			obj._whatIs == General::WhatIs::__GREENDOOR ||
			obj._whatIs == General::WhatIs::__YELLOWDOOR ||
			obj._whatIs == General::WhatIs::__REDDOOR ||
			obj._whatIs == General::WhatIs::__WHITEDOOR ||
			obj._whatIs == General::WhatIs::__BOOKCASEDOOR ||
			obj._whatIs == General::WhatIs::__BARRELDOOR ||
			obj._whatIs == General::WhatIs::__CLOCKDOOR ||
			obj._whatIs == General::WhatIs::__EXITDOOR) 
		{
			AticAtacWorld::DoorCounter* dC = 
				new AticAtacWorld::DoorCounter;
			dC -> _id = obj._id;
			dC -> _room = obj._inRoom;
			dC -> _canOpenClose = // Only the door type normal, trap, or clock can open / close automatically...
				(obj._whatIs == General::__NORMALDOR || 
				 obj._whatIs == General::__TRAPDOOR ||
				 obj._whatIs == General::__CLOCKDOOR) ? true : false;
			dC -> _openClose = false; // By default the doors are all closed...
			dC -> _timer = obj._time; // This parameter defines the time the door opens and close...
			dC -> _counter = 0;

			// If the doors are special ones...
			// That is: Barrel door, or bookcase door,
			// They are always open...
			// The user will be able to entre one or other depending 
			// the aspect he has...
			// Let's notice that the clock door is the only
			// special door that can be open and closed an it 
			// remains closed at the beginning of the game...
			if (obj._whatIs == General::__BOOKCASEDOOR ||
				obj._whatIs == General::__BARRELDOOR)
				dC -> _openClose = true;

			// The id's of the exit doors are kept...
			// This is needed o locate later quicker the exit doors
			// once the main character has the three parts of the exit key...
			if (obj._whatIs == General::__EXITDOOR)
				if (_exitDoorIds [0] == -1) _exitDoorIds [0] = dC -> _id;
				else if (_exitDoorIds [1] == -1) _exitDoorIds [1] = dC -> _id;

			// This piece of code appears only under debug mode
			// to permanetly open the exit door and simplify the debugging of the game
			#ifndef NDEBUG
//			if (obj._whatIs == General::__EXITDOOR)
//				dC -> _openClose = true;
			#endif

			addDoorCounter (dC);
		}
	}

	// Initialize the food status, and the position of the things...
	// This is done in the same loop because the things in general are defined
	// in the same structure...
	for (int i = 0; i < (__NUMBEROFTHINGS - 7); i++)
	{
		General::ThingDefinition tD = General::_e._things [i];
		if (tD._whatIs == General::WhatIs::__FOOD)
		{
			AticAtacWorld::FoodCounter* fC = new AticAtacWorld::FoodCounter;
			fC -> _id = tD._id;
			fC -> _room = tD._inRoom;
			fC -> _eaten = false;
			fC -> _timer = tD._time;
			fC -> _counter = 0;
			addFoodCounter (fC);
		}
		else
		if (tD._whatIs == General::WhatIs::__KILLMUMMY ||
			tD._whatIs == General::WhatIs::__KILLFRANK ||
			tD._whatIs == General::WhatIs::__KILLDEVIL ||
			tD._whatIs == General::WhatIs::__KILLDRACULA ||
			tD._whatIs == General::WhatIs::__KILLHUNCH ||
			tD._whatIs == General::WhatIs::__BOTTLE ||
			tD._whatIs == General::WhatIs::__FACECOIN ||
			tD._whatIs == General::WhatIs::__WING ||
			tD._whatIs == General::WhatIs::__WHIP ||
			tD._whatIs == General::WhatIs::__DIAMOND ||
			tD._whatIs == General::WhatIs::__BAGOFMONEY ||
			tD._whatIs == General::WhatIs::__SKULL ||
			tD._whatIs == General::WhatIs::__GENERICTHING) 
			// The rest of the things (GENERIC) are not useful... they are only to distract!
		{
			AticAtacWorld::ThingPosition* tP = new AticAtacWorld::ThingPosition;
			tP -> _id = tD._id;
			tP -> _room = tD._inRoom;
			tP -> _position = tD._position;
			addThingPosition (tP);
		}
	}

	// Set up the keys...
	// First the green one...
	AticAtacWorld::ThingPosition* gKP = new AticAtacWorld::ThingPosition;
	gKP -> _id = General::_e._things [__GREENKEYID]._id;
	gKP -> _room = General::_e._greenKeyRooms [rand () % General::_e._greenKeyRooms.size ()];
	gKP -> _position = centerOfCurrentRoom (); // Always at the center...
	addThingPosition (gKP);

	// The yellow one...
	AticAtacWorld::ThingPosition* yKP = new AticAtacWorld::ThingPosition;
	yKP -> _id = General::_e._things [__YELLOWKEYID]._id;
	yKP -> _room = General::_e._yellowKeyRooms [rand () % General::_e._yellowKeyRooms.size ()];
	yKP -> _position = centerOfCurrentRoom ();
	addThingPosition (yKP);

	// The red one...
	AticAtacWorld::ThingPosition* rKP = new AticAtacWorld::ThingPosition;
	rKP -> _id = General::_e._things [__REDKEYID]._id;
	rKP -> _room = General::_e._redKeyRooms [rand () % General::_e._redKeyRooms.size ()];
	rKP -> _position = centerOfCurrentRoom ();
	addThingPosition (rKP);

	// The cyan (white) one...
	AticAtacWorld::ThingPosition* cKP = new AticAtacWorld::ThingPosition;
	cKP -> _id = General::_e._things [__CYANKEYID]._id;
	cKP -> _room = General::_e._cyanKeyRooms [rand () % General::_e._cyanKeyRooms.size ()];
	cKP -> _position = centerOfCurrentRoom ();
	addThingPosition (cKP);

	// The exit keys...
	int pe = rand () % 8;
	for (int i = 0; i < 3; i++)
	{
		AticAtacWorld::ThingPosition* eKP = new AticAtacWorld::ThingPosition;
		eKP -> _id = General::_e._things [__EXITKEYID + i]._id;
		eKP -> _room = General::_e._exitKeyRooms [pe][i];
		eKP -> _position = centerOfCurrentRoom ();
		addThingPosition (eKP);
	}
}

// ---
void AticAtacWorld::updatePositions ()
{
	assert (_activeScene);

	// If the state is not "falling"
	// The only thing to actualize is the map...
	if (QGAMES::Game::game () -> activeState () -> type () == __GAMESTATEFALLING)
	{
		((AticAtacMap*) (_activeScene -> activeMap ())) -> updatePositions ();
		return; // Nothing else...the positions of the monster / player are not actualized...
	}

	// Iterates over all the door in the world
	// Opening and closing them, if any...
	for (AticAtacWorld::DoorCounters::const_iterator i = _doorCountersSimple.begin ();
			i != _doorCountersSimple.end (); i++)
	{
		AticAtacWorld::DoorCounter* dC = (*i).second;
		if (dC -> _canOpenClose)
		{
			if (dC -> _counter++ > 
					(dC -> _timer * QGAMES::Game::game () -> framesPerSecond ()))
			{
				dC -> _counter = 0;
				dC -> _openClose = !dC -> _openClose;
				if (dC -> _room == _roomNumber)
				{
					((AticAtacScene*) _activeScene)
						-> actualizeDoorStatus (dC -> _id, dC -> _openClose);
					QGAMES::Game::game () -> sound (dC -> _openClose ? __OPENDOORSOUND : __CLOSEDOORSOUND) 
						-> play (__OPENCLOSEDOORCHANNEL);
				}
			}
		}
	}

	// Iterates over all food that have been eaten
	// Taking the decision whether they have to be "recovered" or not...
	for (AticAtacWorld::FoodCounters::const_iterator i = _foodCountersSimple.begin ();
			i != _foodCountersSimple.end (); i++)
	{
		AticAtacWorld::FoodCounter* fC = (*i).second;
		if (fC -> _eaten)
		{
			if (fC -> _counter++ > 
					(fC -> _timer * QGAMES::Game::game () -> framesPerSecond ()))
			{
				fC -> _eaten = false;
				fC -> _counter = 0;
				// Tschhhhh!....A secret: After eating...the food does appear back in the room
				// Even after a couple of seconds. 
				// The real secret is to leave and came back into the room!...
			}
		}
	}

	// Makes what is standard in the system...
	World::updatePositions ();
}

// ---
void AticAtacWorld::processEvent (const QGAMES::Event& e)
{
	assert (e.data ());
	assert (_mainCharacter);

	if (e.code () == __PLAYERENTERINGDOOR)
	{
		// The parameter received is the door the player has entered
		// The request from it the number of the room connected
		AticAtacBackgroundEntity* eObj = (AticAtacBackgroundEntity*) e.data (); 
		int nRoom = eObj -> roomConnected ();
		// The position of the character is initially the the position (center) of the connected object
		QGAMES::Position nPos = eObj -> posObjConnected ();
		// Keeps the current position of the player
		QGAMES::Position cPPos = _mainCharacter -> position ();
		// Keeps the type of door...
		General::WhatIs tp = eObj -> whatIs ();

		// Whatever the door type the player is comming from is...
		// it has to be opened...
		// The same with the target room...
		// Even if the door needed a key to open it, it remains open
		// However, it is needed to check wehether the original door and the target exists
		// In case the target door is a rug, the target door is not in the door counters...
		// This is the reason it is needed to double check
		if (_doorCountersSimple.find (eObj -> id ()) != _doorCountersSimple.end ())
			_doorCountersSimple [eObj -> id ()] -> _openClose = true;
		if (_doorCountersSimple.find (eObj -> objConnectedId ()) != _doorCountersSimple.end ())
			_doorCountersSimple [eObj -> objConnectedId ()] -> _openClose = true;

		// Get the destination object orientation...
		General::Orientation dOr = 
			General::_e._objects [eObj -> objConnectedId ()]._forms [0]._orientation;
		QGAMES::Vector dOrV = QGAMES::Vector::_cero;
		if (dOr == General::Orientation::__UP) dOrV = QGAMES::Vector (__BD 0, __BD -1, __BD 0);
		if (dOr == General::Orientation::__RIGHT) dOrV = QGAMES::Vector (__BD 1, __BD 0, __BD 0);
		if (dOr == General::Orientation::__DOWN) dOrV = QGAMES::Vector (__BD 0, __BD 1, __BD 0);
		if (dOr == General::Orientation::__LEFT) dOrV = QGAMES::Vector (__BD -1, __BD 0, __BD 0);

		// Calculates the new position of the player...
		QGAMES::Rectangle cZ = eObj -> collisionZone ();
		int lCZX = (int) (cZ.pos2 ().posX () - cZ.pos1 ().posX ());
		int lCZY = (int) (cZ.pos2 ().posY () - cZ.pos1 ().posY ());
		int mLZ = ((lCZX > lCZY) ? lCZX : lCZY) / 2;
		QGAMES::Position mPos = nPos - 
			QGAMES::Vector (__BD (_mainCharacter -> currentForm () -> frameWidth () / 2),
							__BD (_mainCharacter -> currentForm () -> frameHeight () / 2)) - 
				dOrV * (__BD mLZ);
		// The new position of the player takes into account the dimensions of the object
		// he passed through. Really, the size to take into account would be the one of
		// the target object. By we have it created at this stage.
		// Let's suppose the target object is the same than the original one...

		// Two different things can happen when the player enters a door:
		// If the door is a trap door, then a simulation of falling must happen
		// Otherwise simply the new room has to appear.
		// To create a new room, imply  to create new elements and to destroy the old ones
		// This method happens whilst the positions of the main character is being updated
		// So, creating and destroying elements in the middle of that management
		// can alter significantly the behaviour of the code, and wven it can crash
		// So a buoy is created to do the action...
		if (tp == General::WhatIs::__TRAPDOOR)
		{
			AticAtacFallingEventData* eData = new AticAtacFallingEventData;
			eData -> _roomNumber = nRoom;
			eData -> _position = mPos;
			AticAtacWorld::ToFallBuoy* b = 
				(AticAtacWorld::ToFallBuoy*) buoy (__TOFFAILBUOYID);
			b -> setData (eData);
			b -> active (true); // Active the buoy to treat the simulate falling...
		}
		else
		if (tp == General::WhatIs::__EXITDOOR)
		{
			AticAtacFinishEventData* eData = new AticAtacFinishEventData;
			eData -> _roomNumber = nRoom;
			eData -> _position = mPos;
			AticAtacWorld::ToFinishBuoy* b = 
				(AticAtacWorld::ToFinishBuoy*) buoy (__TOFINISHBUOYID);
			b -> setData (eData);
			b -> active (true); // Active the buoy to treat the simulate falling...
		}
		// When the player enters a room, simply...
		else
		{
			AticAtacChangeRoomEventData* eData = new AticAtacChangeRoomEventData;
			eData -> _roomNumber = nRoom;
			eData -> _originalRoomNumber = _roomNumber;
			eData -> _objOrigin = eObj -> originalObjectId ();
			eData -> _position = mPos;
			AticAtacWorld::ToChangeRoomBuoy* b = 
				(AticAtacWorld::ToChangeRoomBuoy*) buoy (__TOCHANGEROOMBUOYID);
			b -> setData (eData);
			b -> active (true); // Active the buoy to treat the change of room...
		}

		// Notify the game to change the color of the score frame...
		notify (QGAMES::Event (__CHANGECOLORFRAME, NULL));
	}
}

// ---
void AticAtacWorld::clearThings ()
{
	for (AticAtacWorld::DoorCounters::const_iterator i = _doorCountersSimple.begin ();
			i != _doorCountersSimple.end (); i++)
		delete ((*i).second); // Clear the door status...
	for (AticAtacWorld::FoodCounters::const_iterator i = _foodCountersSimple.begin ();
			i != _foodCountersSimple.end (); i++)
		delete ((*i).second); // Clear the food status...
	for (AticAtacWorld::ThingPositions::const_iterator i = _thingPositionsSimple.begin ();
			i != _thingPositionsSimple.end (); i++)
		delete ((*i).second); // Clear where the things are...
	for (AticAtacWorld::ThingPositions::const_iterator i = _tombs.begin ();
			i != _tombs.end (); i++)
		delete ((*i).second); // Clear the tombs...

	// Clean up the list of things...
	_carrying.clear (); // The objects from this list are already included in other list of things...
	_doorCounters.clear ();
	_doorCountersSimple.clear ();
	_foodCounters.clear ();
	_foodCountersSimple.clear ();
	_thingPositions.clear ();
	_thingPositionsSimple.clear ();
	_tombs.clear (); _tombCounter = 0;
	_exitDoorIds [0]= _exitDoorIds [1] = -1;
}

// ---
void AticAtacWorld::addThingPosition (ThingPosition* tP)
{
	AticAtacWorld::ThingPositions tPs;
	AticAtacWorld::ThingPositionsPerRoom::const_iterator i = _thingPositions.find (tP -> _room);
	if (i != _thingPositions.end ())
		tPs = (*i).second; 
	// a list of objects at that room already exists...
	// Verify the thing doesn't exist already in the list...otherwise error!
	assert (tPs.find (tP -> _id) == tPs.end ()); 
	// Inserts the new element...
	tPs.insert (AticAtacWorld::ThingPositions::value_type (tP -> _id, tP)); 
	if (i != _thingPositions.end ())
		_thingPositions.erase (i); 
	// ...as the list already exists, the new one has to replace the old one... 
	// Inserts the list of objects in the list of of objects per room, and in the simple list...
	_thingPositions.insert 
		(AticAtacWorld::ThingPositionsPerRoom::value_type (tP -> _room, tPs));
	_thingPositionsSimple.insert (AticAtacWorld::ThingPositions::value_type (tP -> _id, tP));
}

void AticAtacWorld::addFoodCounter (FoodCounter* fC)
{
	AticAtacWorld::FoodCounters fCs;
	AticAtacWorld::FoodCountersPerRoom::const_iterator i = _foodCounters.find (fC -> _room);
	if (i != _foodCounters.end ())
		fCs = (*i).second;
	// a list of foods at that room already exists...
	// Verify the food doesn't exist already in the list...otherwise error!
	assert (fCs.find (fC -> _id) == fCs.end ()); 
	// Inserts the new element...
	fCs.insert (AticAtacWorld::FoodCounters::value_type (fC -> _id, fC));
	if (i != _foodCounters.end ())
		_foodCounters.erase (i);
	// ...as the list already exists, the new one has to replace the old one... 
	// Inserts the list of food in the list of of food per room, and in the simple list...
	_foodCounters.insert 
		(AticAtacWorld::FoodCountersPerRoom::value_type (fC -> _room, fCs));
	_foodCountersSimple.insert (AticAtacWorld::FoodCounters::value_type (fC -> _id, fC));
}

void AticAtacWorld::addDoorCounter (DoorCounter* dC)
{
	AticAtacWorld::DoorCounters dCs;
	AticAtacWorld::DoorCountersPerRoom::const_iterator i = _doorCounters.find (dC -> _room);
	if (i != _doorCounters.end ())
		dCs = (*i).second;
	// a list of foods at that room already exists...
	// Verify the food doesn't exist already in the list...otherwise error!
	assert (dCs.find (dC -> _id) == dCs.end ()); 
	// Inserts the new element...
	dCs.insert (AticAtacWorld::DoorCounters::value_type (dC -> _id, dC));
	if (i != _doorCounters.end ())
		_doorCounters.erase (i);
	// ...as the list already exists, the new one has to replace the old one... 
	// Inserts the list of door counters in the list of of counters per room, and in the simple list...
	_doorCounters.insert 
		(AticAtacWorld::DoorCountersPerRoom::value_type (dC -> _room, dCs));
	_doorCountersSimple.insert (AticAtacWorld::DoorCounters::value_type (dC -> _id, dC));
}

// ---
void* AticAtacWorld::ToChangeRoomBuoy::treatFor (QGAMES::Element* e)
{
	assert (e);

	AticAtacWorld* w = (AticAtacWorld*) e;
	w -> setRoomNumber (_data ->_roomNumber, _data -> _originalRoomNumber, _data ->_objOrigin);
	w -> _mainCharacter -> setPosition (_data -> _position);
	delete _data;
	_data = NULL; // The buoy has been treated...

	// Sounds entering the new room...
	QGAMES::Game::game () -> sound (__ENTERINGTHESCENESOUND) 
		-> play (__GENERICMUSICCHANNELSOUND);

	return (this);
}

// ---
void* AticAtacWorld::ToFallBuoy::treatFor (QGAMES::Element* e)
{
	assert (e);

	QGAMES::Game::game () -> setState (std::string (__GAMESTATEFALLINGNAME));
	((AticAtacGameStateFalling*) QGAMES::Game::game () -> activeState ()) -> 
		setRommNumberTarget (_data -> _roomNumber);
	((AticAtacGameStateFalling*) QGAMES::Game::game () -> activeState ()) -> 
		setPlayerPosition (_data -> _position); 
	// Those things can be set before changing the room, as the state starts falling...
	delete _data;
	_data = NULL; // The buoy has been treated...

	return (this);
}

// ---
void* AticAtacWorld::ToFinishBuoy::treatFor (QGAMES::Element* e)
{
	assert (e);
	((AticAtacGame*) QGAMES::Game::game ()) -> setRoomNumber (_data -> _roomNumber); // usually the outthere room...
	QGAMES::Game::game () -> setState (std::string (__GAMESTATEWINNAME));
	delete _data;
	_data = NULL; // The buoy has been treated...
	return (this);
}
