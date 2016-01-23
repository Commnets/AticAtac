/** \addtogroup Atic */
/*@{*/

#ifndef __SCENES_HPP__
#define __SCENES_HPP__

#include <Arcade/scene.hpp>
#include "Worlds.hpp"
#include "Entities.hpp"
#include "Defs.hpp"

/** The class describing the scene where the game happens. */
class AticAtacScene : public QGAMES::Scene
{
	public:
	// Constrcutors & Destructors
	/** To create an instance of the scene.
		During the game there will be only one instance of this class. 
		The scene during the initializacion, creates the behaviours
		that the monsters will have. */
	AticAtacScene (const QGAMES::Maps& m, const QGAMES::Scenes& cn,
		const QGAMES::SceneProperties& p = QGAMES::SceneProperties (),
		const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ());
	/** To delete the monster's behaviours. */
	~AticAtacScene ();

	/** To get the number of room where the scene is now playing. */
	int roomNumber () const
							{ return (_roomNumber); }
	/** To change the number of the scene where the game is taking place.
		The second, third and fourth parameters indicates the real status of the doors,
		whether if the food has or not to be shown, the real things in the room,
		and the list of tombs in the game.
		The last two parameters indicate the room and the object someone is comming from...if any.
		This is to set up the form and the aspect of the door the player is comming from. */
	void setRoomNumber (int r, 
		const AticAtacWorld::DoorCounters& dC,
		const AticAtacWorld::FoodCounters& fC,
		const AticAtacWorld::ThingPositions& tP,
		const AticAtacWorld::ThingPositions& tbP,
		int fR = -1, int fO = -1);
	/** Actualize the status of the doors. */
	void actualizeDoorStatus (int iD, bool oC);

	/** To get the center of the room. */
	QGAMES::Position centerOfRoom () const;

	/** This method is to set up a new appearing monster.
		The monsters start to appear after a couple of seconds. 
		The mthod determinates the type of monster created. */
	void createAppearingMonster (int e);
	/** To reset a thing in real time.
		This method is usually used by the left procedure. 
		The method receives the thing and a position. */
	void setUpThing (const General::ThingDefinition&, const QGAMES::Position& p);

	/** Shoot. */
	void shoot (int nS);
	/** No shoot. */
	void noShoot (int nS);

	/** @see parent class. Init additionally the internal variables. */
	virtual void initialize ();
	/** @see parent class. Additionally counts seconds to set up appearing monsters. */
	virtual void updatePositions ();
	/** After a monster has appearing and a couple of seconds pass, then
		the monster's figure appears, and the monster starts to move. */
	virtual void processEvent (const QGAMES::Event& e);

	private:
	/** To get a ramdom position in the room. */
	QGAMES::Position randomPositionAtRoom () const;
	/** To get a ramdom position to locate an entity
		taking into account the size of the entity. */
	QGAMES::Position randomPositionAtRoomFor (QGAMES::Entity* a) const;
	/** To get a rectangle with the limits of the active room. */
	QGAMES::Rectangle roomLimits () const;

	// Implementation methods...
	// Used by setRoomNUmber ()
	int setUpBackgroundObjects (const AticAtacWorld::DoorCounters& dC,
		const AticAtacWorld::ThingPositions& tbP, int fR, int fO);
	int setUpMonsters (const AticAtacWorld::ThingPositions& tP);
	int setUpThingsToEat (const AticAtacWorld::FoodCounters& fC);
	int setUpThingsToCatch (const AticAtacWorld::ThingPositions& tP);

	private:
	/** The number of the room the player is now in.
		Initially this map is 0. */
	int _roomNumber; 
	/** The bahviours for the monsters. */
	AticAtacMonster::Behaviours _behaviours;

	// Implementation...
	/** The number of objects in the background. */
	int _numberOfBkObjects;
	/** The number of monsters possible. */
	int _numberOfMonsters;
	/** The number of things to eat. */
	int _numberOfThingsToEat;
	/** The number of things to catch. */
	int _numberOfThingsToCatch;

	// Implementation
	// To control the status of the monsters appearing
	/** Struct simplifying the manipulation of the monsters appearing in the scene. */
	struct MonsterAppearingStatus
	{
		public:
		MonsterAppearingStatus ()
			: _entity (0), _maxCount (0), _count (0)
							{ }

		int _entity;
		int _maxCount;
		int _count;
	};

	/** To simplify the acces to the type of structure defined above. */
	typedef std::vector <MonsterAppearingStatus> MonsterAppearingStatusList;
	/** The status of the monsters appearing. */
	MonsterAppearingStatusList _monsterStatus;
};

#endif
  
// End of the file
/*@}*/