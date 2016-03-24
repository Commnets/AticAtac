/** \addtogroup Atic */
/*@{*/

#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <Arcade/arcadegame.hpp>
#include "GameImplementation.hpp"
#include "Entities.hpp"
#include "ScObjects.hpp"

/** Main class of the game. 
	There can only be one instance of this class.
	This class controls the main loop of the game. */
class AticAtacGame : public QGAMES::ArcadeGame
{
	public:
	struct Level
	{
		Level ()
			: _level (0), 
			  _maxTimeToAppearMonsters (0),
			  _maxNumberOfAppearingMonsters (0),
			  _percentUpPerFood (0),
			  _pointsUpPerFood (0),
			  _percentDownPerMonster (0),
			  _pointsUpWhenKillMonster (0)
							{ }

		int _level;
		int _maxTimeToAppearMonsters;
		int _maxNumberOfAppearingMonsters;
		int _percentUpPerFood;
		int _pointsUpPerFood;
		int _percentDownPerMonster;
		int _pointsUpWhenKillMonster;
	};

	// To define a list of levels...
	typedef std::vector <Level> Levels;

	// Constructors & Destructors
	AticAtacGame ();
	// No specific destructor is needed...

	/** Reset the initial values of the game. 
		Any time the game starts, this value is invoked. 
		The score elements have to had been created befrore. 
		Otherwise it won't work. */
	void resetGame ();
	
	/** Information about a level.
		Assertion of the level required is done before. */
	const Level& levelInfo () const
							{ return (_levels [_level]); }
	/** To know the current level. */
	int level () const
							{ return (_level); }
	/** To set a new level. */
	void setLevel (int l);

	/** To set whether the game is or not controlled by joystick. */
	void setJoystick (bool j);

	/** To know the current room number. */
	int roomNumber () const;
	/** To set the room number.
		This method is not usually used. The only user so far is the state "falling". */
	void setRoomNumber (int nR, int fR = -1, int fO = -1);
	/** To get the center of the room where the player is in.
		This method i.e is used to create the player at the beginning of the state called "arriving". 
		If there is no active world, the position returned will be cero. */
	QGAMES::Position centerOfCurrentRoom () const;

	/** Want to catch. 
		This method is used by the input handler only...so far. */
	void setWantToCatch ()
							{ _wantToCatch = true; }

	/** To know the energy available. */
	int energyAvailable () const
							{ return (_energyPercentage); }
	/** To set the energy available. */
	void setEnergyAvailable (int e);

	/** To know the number of lives. */
	int numberOfLives () const
							{ return (_numberOfLives); }
	/** To set the number of lives. */
	void setNumberOfLives (int nL);

	/** To know the current aspect of the player. */
	AticAtacCharacter::Type playerType ()
							{ return (_playerType); }
	/** To set the current aspect of the player. */
	void setPlayerType (AticAtacCharacter::Type t);

	/** To know the number of seconds from the beggining. */
	int numberOfSeconds () const
							{ return (_numberOfSeconds); }
	/** To set the number of seconds. */
	void setNumberOfSeconds (int nS);

	/** To know the number of points. */
	int numberOfPoints () const
							{ return (_scoreNumber); }
	/** To set the number of points. */
	void setNumberOfPoints (int nP); 

	/** To shoot one weapon more. */
	void shoot ();
	/** To stop shooting the last weapon. */
	void noShoot ();
	/** To stop shooting anything. */
	void noShootAtAll ();

	/** To know the things caught. */
	const AticAtacWorld::ThingPositionsList& thingsCaught () const
							{ return (_carrying); }
	/** To get something. */
	void toCatch (AticAtacThingToCatch* t);
	/** To left the last thing caught. */
	void toLeaveLast ();
	/** To leave all things. */
	void toLeaveAll ();
	/** To know if the main character can or not exit.
		It happens just when the things being carrying are the exit keys
		and they are located in the list in the right order. */
	bool couldMainCharacterExit () const;

	virtual void updatePositions ();
	virtual void addScoreObjects ();
	virtual void removeScoreObjects ();
	virtual void additionalTasks ()
							{ /** Nothing...the default behaviour is deleted. */ }

	virtual void processEvent (const QGAMES::Event& e);

	private:
	// The methods to create the builders...
	/** see @parent */
	virtual QGAMES::FormBuilder* createFormBuilder ();
	virtual QGAMES::ObjectBuilder* createObjectBuilder ();
	virtual QGAMES::EntityBuilder* createEntityBuilder ();
	virtual QGAMES::MovementBuilder* createMovementBuilder ();
	virtual QGAMES::SoundBuilder* createSoundBuilder ();
	virtual QGAMES::Timer* createTimer ();
	virtual QGAMES::Screens createScreens ();
	virtual QGAMES::WorldBuilder* createWorldBuilder ();
	virtual QGAMES::MapBuilder* createMapBuilder ();

	/** To initialize the types of things. */
	virtual void initialize ();
	/** To finalize the game. */
	virtual void finalize ();
	/** To detect the collisions among the basic objects in the game. */
	virtual void detectCollisions ();

	// Implementation
	/** When player collisions with a monster. */
	void playerCollisionWithMonster (AticAtacMonster* m);
	/** When player collisions with something to eat. */
	void playerCollisionWithFood (AticAtacFood* f);
	/** When player collision with something to catch. */
	void playerCollisionWithSomethingToCatch (AticAtacThingToCatch* t);
	/** A weapon has collision with a monster. */
	void weaponCollisionWithMonster (AticAtacWeapon* w, AticAtacMonster* m);
	/** When the player dies. */
	void playerDies ();

	private:
	/** The levels. */
	Levels _levels;
	/** The number of the level. */
	int _level;
	/** The frame rounding the score objects. */
	AticAtacScoreFrame* _scoreFrame;
	/** The chicken. */
	AticAtacChicken* _chicken;
	/** The object to reflect the lives. */
	AticAtacLives* _lives;
	/** The things the player has (score object) */
	AticAtacThings* _things;
	/** The time counter. */
	AticAtacTimeCounter* _timeCounter;
	/** The score counter. */
	AticAtacScoreCounter* _scoreCounter;
	/** Percentage of energy. */
	int _energyPercentage;
	/** The number of lives. */
	int _numberOfLives;
	/** The things being carrying. */
	AticAtacWorld::ThingPositionsList _carrying;
	/** The type of the player. */
	AticAtacCharacter::Type _playerType;
	/** Number of weapons are flying. */
	int _shooting;
	/** The current number of seconds. */
	int _numberOfSeconds;
	/** THe current score number. */
	int _scoreNumber;

	// Implementation...
	/** An attribute to indicate whether the user wants or not to catch 
		what he is on. */
	bool _wantToCatch;
	/** To detect when something has been caught. */
	bool _somethingCaught;
	/** To count down the energy. */
	int _countEnergy;
	/** A counter to count when to change a second. */
	int _counterPerSecond;
	/** To count in debug mode the number of collisions. */
	int _n;
};

#endif
  
// End of the file
/*@}*/