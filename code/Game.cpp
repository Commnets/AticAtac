#include "Game.hpp"
#include "EntitiesBuilder.hpp"
#include "MovementsBuilder.hpp"
#include "Movements.hpp"
#include "Timer.hpp"
#include "MapsBuilder.hpp"
#include "WorldsBuilder.hpp"
#include "GameStates.hpp"
#include "InputHandler.hpp"
#include "Defs.hpp"
#include "Events.hpp"
#include <SDL/sdlformbuilder.hpp>
#include <SDL/sdlsoundbuilder.hpp>
#include <iostream>

// ---
AticAtacGame::AticAtacGame ()
	: QGAMES::ArcadeGame (new GameImplementation (), QGAMES::Worlds ()),
	  _levels (),
	  _level (0), // The level by default...
	  _scoreFrame (NULL), _chicken (NULL), _lives (NULL), _things (NULL),
	  _timeCounter (NULL), _scoreCounter (NULL),
	  _energyPercentage (0),
	  _numberOfLives (0),
	  _carrying (),
	  _playerType (AticAtacCharacter::Type::__KNIGHT),
	  _shooting (0),
	  _numberOfSeconds (0),
	  _scoreNumber (0),
	  _wantToCatch (false),
	  _somethingCaught (false),
	  _countEnergy (0),
	  _counterPerSecond (0),
	  _n (0)
{ 
	// Creates the levels...
	_levels.resize (3);
	
	Level l1;
	l1._level = 1;
	l1._maxTimeToAppearMonsters = 7;
	l1._maxNumberOfAppearingMonsters = 3;
	l1._percentUpPerFood = 20;
	l1._pointsUpPerFood = 20;
	l1._percentDownPerMonster = 1;
	l1._pointsUpWhenKillMonster = 150;
	_levels [0] = l1;
	
	Level l2;
	l2._level = 1;
	l2._maxTimeToAppearMonsters = 5;
	l2._maxNumberOfAppearingMonsters = 4;
	l2._percentUpPerFood = 15;
	l2._pointsUpPerFood = 20;
	l2._percentDownPerMonster = 1;
	l2._pointsUpWhenKillMonster = 150;
	_levels [1] = l2;
	
	Level l3;
	l3._level = 1;
	l3._maxTimeToAppearMonsters = 4;
	l3._maxNumberOfAppearingMonsters = 5;
	l3._percentUpPerFood = 10;
	l3._pointsUpPerFood = 20;
	l3._percentDownPerMonster = 2;
	l3._pointsUpWhenKillMonster = 150;
	_levels [2] = l3;
}

// ---
void AticAtacGame::resetGame ()
{
	assert (_chicken);
	assert (_scoreFrame);
	assert (_things);
	assert (_timeCounter);
	assert (_scoreCounter);
	assert (_activeWorld);

	// The internal status variables are set to their initial values...
	_numberOfLives = __INITIALNUMBEROFLIVES;
	_energyPercentage = 0;
	_carrying = AticAtacWorld::ThingPositionsList ();
	((AticAtacWorld*) _activeWorld) -> thingsAllLeft ();
	_shooting = 0; // No weapons flying...

	// The level doesn't change...

	// The score objects are also initialized...
	_chicken -> setPercentage (_energyPercentage);
	_scoreFrame -> setColor (0); // The first...it is cyclic...
	_lives -> setLives (_numberOfLives);
	_lives -> setPlayerType ((int) _playerType); // It is not reinitialized...
	_things -> setThings (_carrying);

	// Initialize the things (set tke keys...), and the room number...
	((AticAtacWorld*) _activeWorld) -> initializeThings ();
	((AticAtacWorld*) _activeWorld) -> setRoomNumber (__FIRSTROOM);
	// ...the main character is not carrying anything...
	((AticAtacCharacter*) artist (__ENTITYPLAYER)) -> leaveAllThings ();
	// There is no weapons fyling...
	for (int i = (__ENTITYPLAYER + 1); i < (__ENTITYPLAYER + __NUMBERMAXENTITIESPLAYER); i++)
		((AticAtacWeapon*) artist (i)) -> setVisible (false);

	// Initialize the time...
	_numberOfSeconds = 0; // At the beggning no time has been consumed... 
	_timeCounter -> setSeconds (_numberOfSeconds);

	// Initialize the score..
	_scoreNumber = 0; // At the beggining there is no puntuation...
	_scoreCounter -> setNumber (_scoreNumber);

	// Very internal attributes
	_wantToCatch = false;
	_somethingCaught = false;
	_countEnergy = 0;
	_counterPerSecond = 0;
	_n = 0;
}

// ---
void AticAtacGame::setLevel (int l)
{
	assert (l < 3);
	_level = l;
}

// ---
void AticAtacGame::setJoystick (bool j)
{
	assert (_inputHandler);
	((InputHandler*) _inputHandler) -> activateJoystick (j);
}

// ---
int AticAtacGame::roomNumber () const
{
	return ((_activeWorld == NULL) // No way to know the current room number...
		? 0 : ((AticAtacWorld*) _activeWorld) -> roomNumber ());
}

// ---
void AticAtacGame::setRoomNumber (int nR, int fR, int fO)
{
	assert (_activeWorld);

	// No shoots in place...
	_shooting = 0;
	((AticAtacWorld*) _activeWorld) -> setRoomNumber (nR, fR, fO);
	// It is not commng from any other one previous room...
}

// ---
QGAMES::Position AticAtacGame::centerOfCurrentRoom () const
{
	assert (_activeWorld); // No way to know the current center of the room...
	return (((AticAtacWorld*) _activeWorld) -> centerOfCurrentRoom ());
}

// ---
void AticAtacGame::setEnergyAvailable (int e)
{
	assert (_chicken);

	_energyPercentage = e;
	if (_energyPercentage < 0)
		_energyPercentage = 0;
	if (_energyPercentage > 100)
		_energyPercentage = 100;

	_chicken -> setPercentage (_energyPercentage);
}

// ---
void AticAtacGame::setNumberOfLives (int nL)
{
	assert (_lives);

	_numberOfLives = nL;
	if (_numberOfLives < 0) 
		_numberOfLives = 0;
	if (_numberOfLives > __MAXNUMBEROFLIVES) 
		_numberOfLives = __MAXNUMBEROFLIVES;

	_lives -> setLives (_numberOfLives - 1); // Always one less (the other one is being playing...
	_lives -> setPlayerType ((int) _playerType);
}

// ---
void AticAtacGame::setPlayerType (AticAtacCharacter::Type t)
{
	_playerType = t;
	((AticAtacCharacter*) artist (__ENTITYPLAYER)) -> setType (_playerType);
}

// ---
void AticAtacGame::setNumberOfSeconds (int nS)
{
	assert (_timeCounter);

	_numberOfSeconds = nS;
	_timeCounter -> setSeconds (nS);
}

// ---
void AticAtacGame::setNumberOfPoints (int nP)
{
	assert (_scoreCounter);

	_scoreNumber = nP;
	_scoreCounter -> setNumber (nP);
}

// ---
void AticAtacGame::shoot ()
{
	assert (_activeWorld);

	if (_shooting >= __MAXNUMBEROFSIMULTANEOUSSHOOTS)
		return; // No more shootings are allowed...
	((AticAtacWorld*) _activeWorld) -> shoot (_shooting++);
}

// ---
void AticAtacGame::noShoot ()
{
	assert (_activeWorld);

	if (_shooting > 0)
		((AticAtacWorld*) _activeWorld) -> noShoot (--_shooting);
}


// ---
void AticAtacGame::toCatch (AticAtacThingToCatch* t)
{
	assert (t);
	assert (_activeWorld);

	((AticAtacWorld*) _activeWorld) -> thingCaught (t);
	_carrying = ((AticAtacWorld*) _activeWorld) -> thingsInOrder ();
	_things -> setThings (_carrying);
	_somethingCaught = true;

	// If after catching something
	// the main character could exit the maze, then
	// the exit door is open...
	// Otherwise it is closed (just in case)
	if (couldMainCharacterExit ()) ((AticAtacWorld*) _activeWorld) -> openExitDoor ();
	else ((AticAtacWorld*) _activeWorld) -> closeExitDoor ();
}

// ---
void AticAtacGame::toLeaveLast ()
{
	assert (_activeWorld);

	((AticAtacWorld*) _activeWorld) -> thingLeft ();
	_carrying = ((AticAtacWorld*) _activeWorld) -> thingsInOrder ();
	_things -> setThings (_carrying);

	// If after leaving something
	// the main character could still exit the maze, then
	// the exit door would open...
	// Otherwise it close
	// Really this piece of code is not necessary when the 
	// max number of thing to carry is the same of number of pieces of the exit key
	// But just maintain it for compatibility!
	if (couldMainCharacterExit ()) ((AticAtacWorld*) _activeWorld) -> openExitDoor ();
	else ((AticAtacWorld*) _activeWorld) -> closeExitDoor ();
}

// ---
void AticAtacGame::toLeaveAll ()
{
	assert (_activeWorld);

	((AticAtacWorld*) _activeWorld) -> thingsAllLeft ();
	_carrying = ((AticAtacWorld*) _activeWorld) -> thingsInOrder ();
	_things -> setThings (_carrying);
}

// ---
bool AticAtacGame::couldMainCharacterExit () const
{
	assert (_activeWorld);

	return (_things -> couldMainCharacterExit ());
}

// ---
void AticAtacGame::updatePositions ()
{
	// If the state is not playing or fallin...makes no sense to actualize the states..
	if (_currentGameState -> type () != __GAMESTATEPLAYING &&
		_currentGameState -> type () != __GAMESTATEFALLING)
		return; // This is the reason nothing moves unless you're playing!

	// If the game is falling,
	// Only the world is actualize (no collisions are detected...)
	// In the world (see it) only the map is actualized...
	if (_currentGameState -> type () == __GAMESTATEFALLING)
	{
		((AticAtacWorld*) _activeWorld) -> updatePositions ();
		return;
	}

	// The things are moved...
	QGAMES::ArcadeGame::updatePositions ();
	detectCollisions ();
	// In the default behaviour the detection of the collisions are done
	// after updating the positions in the default arcade behaviour
	// So, when get this place and the SPACE bar has been pressed, the _wantToCatch variable is true
	// But nothing has been really catched...
	// So it is needed to change the default behaviour, and the collisions detected here

	// After detecting the collisions, after catching something...
	// If nothing has been caught, but the button for catching has been pressed...
	// The real meaning is to leave something!
	// What?: What it is first in the list...
	if (_somethingCaught)
		_somethingCaught = false;
	else // He wants really to leave something...
	if (_wantToCatch)
	{
		_wantToCatch = false;
		toLeaveLast ();
	}

	// Counts down the energy...represented in the chicken...
	// a 1 percent is lot per second...
	// This is something constant and can not be change using the configuration level...
	if (_countEnergy++ > (__SPEEDLOSSINGLIVE * framesPerSecond ()))
	{
		_countEnergy = 0;
		setEnergyAvailable (energyAvailable () + 1);
		if (energyAvailable () >= 100) // It is going to die...
			playerDies ();
	}

	// Counts per second...
	if (_counterPerSecond++ > (1 * framesPerSecond ()))
	{
		_counterPerSecond = 0;
		setNumberOfSeconds (numberOfSeconds () + 1);
		if (numberOfSeconds () >= 3600) /** 1 hourt maximum... the game finishes */
			setState (std::string (__GAMESTATEENDNAME));
	}
}

// ---
void AticAtacGame::addScoreObjects ()
{
	if (!_scoreObjects.empty ())
		return; // The object has been already created...

	// Add the frame around all the score elements...
	_scoreFrame = new AticAtacScoreFrame ();
	_scoreFrame -> setPosition (QGAMES::Position (__BD 490, __BD 0, __BD 0));
	_scoreFrame -> initialize ();

	// Adds the chicken of energy...
	_chicken = new AticAtacChicken ();
	_chicken -> setPosition (QGAMES::Position (__BD 525, __BD 250, __BD 0));
	_chicken -> initialize ();
	setEnergyAvailable (0); // Always full when adding...

	// Adds the score object to indicate the lives...
	_lives = new AticAtacLives ();
	_lives -> setPosition (QGAMES::Position (__BD 520, __BD 340, __BD 0));
	_lives -> initialize ();
	setNumberOfLives (_numberOfLives); // Converves the last number of lives declared...

	// Adds the things the player has...
	_things = new AticAtacThings ();
	_things -> setPosition (QGAMES::Position (__BD 520, __BD 75, __BD 0));
	_things -> initialize ();
	_things -> setThings (_carrying); // Conserves what the player was carrying before...

	// Adds the score object to count time...
	_timeCounter = new AticAtacTimeCounter ();
	_timeCounter -> setPosition (QGAMES::Position (__BD 535, __BD 165, __BD 0));
	_timeCounter -> initialize ();
	_timeCounter -> setSpace (2);
	_timeCounter -> setMaxTextLength (5);
	_timeCounter -> setSeconds (0);

	// Adds the score to count points...
	_scoreCounter = new AticAtacScoreCounter ();
	_scoreCounter -> setPosition (QGAMES::Position (__BD 535, __BD 220, __BD 0));
	_scoreCounter -> initialize ();
	_scoreCounter -> setSpace (2);
	_scoreCounter -> setNumberLength (5);
	_scoreCounter -> setNumber (0);

	// Adds all objects to the list...
	_scoreObjects.insert (QGAMES::Entities::value_type (_scoreFrame -> id (), _scoreFrame));
	_scoreObjects.insert (QGAMES::Entities::value_type (_chicken -> id (), _chicken));
	_scoreObjects.insert (QGAMES::Entities::value_type (_lives -> id (), _lives));
	_scoreObjects.insert (QGAMES::Entities::value_type (_things -> id (), _things));
	_scoreObjects.insert (QGAMES::Entities::value_type (_timeCounter -> id (), _timeCounter));
	_scoreObjects.insert (QGAMES::Entities::value_type (_scoreCounter -> id (), _scoreCounter));
}

// ---
void AticAtacGame::removeScoreObjects ()
{
	QGAMES::ArcadeGame::removeScoreObjects ();

	delete _scoreFrame;
	_scoreFrame = NULL;
	delete _chicken;
	_chicken = NULL;
	delete _lives;
	_lives = NULL;
	delete _timeCounter;
	_timeCounter = NULL;
	delete _scoreCounter;
	_scoreCounter = NULL;
	// The objects are set to NULL, because this method can be
	// invoked twice in a programming mistake...
}

// ---
void AticAtacGame::processEvent (const QGAMES::Event& e)
{
	assert (_scoreFrame);

	if (e.code () == __CHANGECOLORFRAME)
		_scoreFrame -> setColor (_scoreFrame -> color () + 1);
	else
	if (e.code () == __SHOOTFINISH)
		noShoot ();
	else
	// When the user enters a new room...
	// Thw shoots fylings becomnes 0, they starts again...
	if (e.code () == __PLAYERENTERINGDOOR)
		_shooting = 0;
}

// ---
QGAMES::FormBuilder* AticAtacGame::createFormBuilder ()
{ 
	return (new SDL2DSpriteBuilder (std::string (__FORMSFILE__), 
		(SDLScreen*) mainScreen ())); 
}

// ---
QGAMES::ObjectBuilder* AticAtacGame::createObjectBuilder ()
{
	return (new QGAMES::ObjectBuilder (std::string (__OBJECTSFILE__)));
}

// ---
QGAMES::EntityBuilder* AticAtacGame::createEntityBuilder ()
{ 
	return (new EntitiesBuilder (__ENTITIESFILE__, 
		formBuilder (), movementBuilder ())); 
}

// ---
QGAMES::MovementBuilder* AticAtacGame::createMovementBuilder () 
{ 
	return (new MovementsBuilder (__MOVEMENTSFILE__)); 
}

// ---
QGAMES::SoundBuilder* AticAtacGame::createSoundBuilder () 
{ 
	return (new SDLSoundBuilder (__SOUNDSFILE__)); 
}

// ---
QGAMES::Timer* AticAtacGame::createTimer ()
{ 
	return (new Timer ()); 
}

// ---
QGAMES::Screens AticAtacGame::createScreens ()
{ 
	QGAMES::Screens r;
	QGAMES::Screen* scr = new SDLScreen (__GAMESNAME__, 
		 QGAMES::Position (0,0), __SCREENWIDTH__, __SCREENHEIGHT__, __SCREENXPOS__, __SCREENYPOS__);
	r.insert (QGAMES::Screens::value_type (__QGAMES_MAINSCREEN__, scr));
	return (r); 
}

// ---
QGAMES::WorldBuilder* AticAtacGame::createWorldBuilder ()
{
	return (new WorldsBuilder (std::string (__WORLDSFILE__), mapBuilder ()));
}

// ---
QGAMES::MapBuilder* AticAtacGame::createMapBuilder ()
{
	QGAMES::MapBuilder* result = new QGAMES::MapBuilder (std::string (__MAPSFILE__));
	result -> addAddsOn (new MapsBuilderAddsOn (objectBuilder ()));
	return (result);
}

// ---
void AticAtacGame::initialize ()
{
	QGAMES::ArcadeGame::initialize ();

	// Sets the icon to the window...
	mainScreen () -> setIcon (formBuilder () -> form (__ATICATACICONFORM__));

	// Stablish the number of lives...
	_numberOfLives = __INITIALNUMBEROFLIVES;
	// The energy is full...
	_energyPercentage = 0;
	// No things carrying...
	_carrying = AticAtacWorld::ThingPositionsList ();
	// No weapons are flying...
	_shooting = 0;
	// No time consumed...
	_numberOfSeconds = 0;
	// No points on the score...
	_scoreNumber = 0;

	// Very internal attributes
	_wantToCatch = false;
	_somethingCaught = false;
	_countEnergy = 0;
	_counterPerSecond = 0;
	_n = 0;

	// Clean up everything...
	// no artists, no game states, no worlds in place...
	_artists.clear ();
	_gameStates.clear ();
	_worlds.clear ();

	// Inserts the world...
	_worlds.insert (QGAMES::Worlds::value_type (__ATICATACWORLD,
		_worldBuilder -> world (__ATICATACWORLD)));

	// Inserts the artists: 
	// The background elements, 
	for (int i = __ENTITYBACKGROUNDBASE; 
		i < (__ENTITYBACKGROUNDBASE + __NUMBERMAXENTITIESBK); i++)
			addArtist (i);
	// ...The monsters, 
	for (int i = __ENTITYMONSTERBASE; 
		i < (__ENTITYMONSTERBASE + __NUMBERMAXENTITIESMONSTER); i++)
			addArtist (i);
	// ...The food, 
	for (int i = __ENTITYEATENBASE; 
		i < (__ENTITYEATENBASE + __NUMBERMAXENTITIESEAT); i++)
			addArtist (i);
	// The things to be caught, 
	for (int i = __ENTITYCATCHBASE; 
		i < (__ENTITYCATCHBASE + __NUMBERMAXENTITIESCATCH); i++)
			addArtist (i);
	// and the main character, and his weapon...
	for (int i = __ENTITYPLAYER; 
		i < (__ENTITYPLAYER + __NUMBERMAXENTITIESPLAYER); i++)
			addArtist (i);

	// Add the Score Objects...
	addScoreObjects ();

	// Creates the different game states...
	addState (new AticAtacGameStateLoading (this), std::string (__GAMESTATELOADINGNAME));
	addState (new AticAtacGameStateInitial (this), std::string (__GAMESTATEINITIALNAME));
	addState (new AticAtacGameStateSelect (this), std::string (__GAMESTATESELECTNAME));
	addState (new AticAtacGameStateArrive (this), std::string (__GAMESTATESTARTSNAME));
	addState (new AticAtacGameStatePlaying (this), std::string (__GAMESTATEPLAYINGNAME));
	addState (new AticAtacGameStateFalling (this), std::string (__GAMESTATEFALLINGNAME));
	addState (new AticAtacGameStateDie (this), std::string (__GAMESTATEPLAYERDIESNAME));
	addState (new AticAtacGameStateEnd (this), std::string (__GAMESTATEENDNAME));
	addState (new AticAtacGameStateDemo (this), std::string (__GAMESTATEDEMONAME));
	addState (new AticAtacGameStateWin (this), std::string (__GAMESTATEWINNAME));

	// States the world...and initialize it...
	setWorld (__ATICATACWORLD);
	_activeWorld -> initialize ();

	// The initial state...
	setState (std::string (__GAMESTATELOADINGNAME));
}

// ---
void AticAtacGame::finalize ()
{
	removeScoreObjects ();
	// The only thing to do just in case...
}

// ---
void AticAtacGame::detectCollisions ()
{
	AticAtacCharacter* player = (AticAtacCharacter*) artist (__ENTITYPLAYER);

	// Iterating over the monster...
	// The main loop is going to detect if there is any collision among them
	// and the player or among the weapons and them...
	for (int i = __ENTITYMONSTERBASE; 
			i < (__ENTITYMONSTERBASE + __NUMBERMAXENTITIESMONSTER); i++)
	{
		AticAtacMonster* m = (AticAtacMonster*) artist (i);

		// ..Any collision with the player?
		if (m -> isVisible () && m -> currentMovement () != NULL &&
				player -> hasCollisionWith (m))
			playerCollisionWithMonster (m);
		// ...any collision with any weapon? (there can be various)
		for (int j = (__ENTITYPLAYER + 1); 
				j < (__ENTITYPLAYER + __NUMBERMAXENTITIESPLAYER); j++)
		{
			AticAtacWeapon* w = (AticAtacWeapon*) artist (j);
			if (w -> isVisible () && m -> isVisible () &&
					m -> currentMovement () != NULL && m -> canDie () && w -> hasCollisionWith (m))
				weaponCollisionWithMonster (w, m);
		}
	}

	// Then it is time to detect whether a 
	// collision with something to eat has been detected
	for (int i = __ENTITYEATENBASE; 
			i < (__ENTITYEATENBASE + __NUMBERMAXENTITIESEAT); i++)
	{
		AticAtacFood* f = (AticAtacFood*) artist (i);
		if (f -> isVisible () && player -> hasCollisionWith (f))
			playerCollisionWithFood (f);
	}

	// Then it is time to detect whether a 
	// collision with something that can be caught has been detected
	_somethingCaught = false;
	for (int i = __ENTITYCATCHBASE; 
			i < (__ENTITYCATCHBASE + __NUMBERMAXENTITIESCATCH); i++)
	{
		AticAtacThingToCatch* t = (AticAtacThingToCatch*) artist (i);
		if (t -> isVisible () && player -> hasCollisionWith (t))
			playerCollisionWithSomethingToCatch (t);
	}
}

// ---
void AticAtacGame::playerCollisionWithMonster (AticAtacMonster* m)
{
	#ifndef NDEBUG
	std::cout << _n++ << " Collision with monster: " << m -> id () << std::endl;
	#endif

	sound (__PLAYERHITSOUND) -> play (__HITTINGCHANNELSOUND);
	// The chicken goes down...
	setEnergyAvailable (energyAvailable () + levelInfo ()._percentDownPerMonster);
}

// ---
void AticAtacGame::playerCollisionWithFood (AticAtacFood* f)
{
	assert (f);

	#ifndef NDEBUG
	std::cout << _n++ << " Collision with food: " << f -> id () << std::endl;
	#endif

	((AticAtacWorld*) _activeWorld) -> foodEaten (f);
	sound (__PLAYEREATSFOOD) -> play (__EATINGCHANNELSOUND);
	// The chicken goes up...
	setEnergyAvailable (energyAvailable () - levelInfo ()._percentUpPerFood);
	// The points goes up...
	setNumberOfPoints (numberOfPoints () + levelInfo ()._pointsUpPerFood);
}

// ---
void AticAtacGame::playerCollisionWithSomethingToCatch (AticAtacThingToCatch* t)
{
	assert (t);

	#ifndef NDEBUG
	std::cout << _n++ << " Collision with thing: " << t -> id () << std::endl;
	#endif

	// Only when the player has the intention to take something...
	if (_wantToCatch)
	{
		sound (__PLAYEREATSFOOD) -> play (__CATCHCHANNELSOUND);
		_wantToCatch = false; // The thing has been caught...
		toCatch (t);
	}
}

// ---
void AticAtacGame::weaponCollisionWithMonster (AticAtacWeapon* w, AticAtacMonster* m)
{
	assert (w);
	assert (m);

	#ifndef NDEBUG
	std::cout << _n++ << " Collision between weapon: " << w -> id () 
		<< " and monster: " << m -> id () << std::endl;
	#endif

	// The sound...
	sound (__WEAPONKILLSOMETHING) -> play (__KILLCHANNELSOUND);
	// The weapon continues...
	// Bu the monster has to die...
	// This is done just changing the status...
	m -> setAspectTo (General::_e._dieing, AticAtacMonster::Status::__DIEING);
	// The points goes up...
	setNumberOfPoints (numberOfPoints () + levelInfo ()._pointsUpWhenKillMonster);
}

// ---
void AticAtacGame::playerDies ()
{
	#ifndef NDEBUG
	std::cout << "Player dies" << std::endl;
	#endif

	// Adds a tombs in the room...
	((AticAtacWorld*) _activeWorld) -> addTomb ();
	// The state of the game is dieing...
	setState (std::string (__GAMESTATEPLAYERDIESNAME));
}


