#ifndef __WORLDS_HPP__
#define __WORLDS_HPP__

#include <Arcade/world.hpp>
#include "Entities.hpp"
#include "Defs.hpp"
#include "Events.hpp"
#include <map>

/** This class represents the world where the play is happening. */
class AticAtacWorld : public QGAMES::World
{
	public:
	/** An structure to know where the things that can be caught are in
		every moment. */
	struct ThingPosition
	{
		ThingPosition ()
			: _room (0), _id (0), _position (QGAMES::Position::_cero)
							{ }

		int _room;
		int _id;
		QGAMES::Position _position;
	};

	/** To manage a map of positions. */
	typedef std::map <int, ThingPosition*, std::less <int>> ThingPositions;
	typedef std::map <int, ThingPositions, std::less <int>> ThingPositionsPerRoom;

	// Implementation
	/** This is a basic counter to allow opening and closing doors even when
		the user is not in the room. */
	struct DoorCounter
	{
		DoorCounter ()
			: _room (0), _id (0), _canOpenClose (true),
			  _openClose (true), _timer (0), _counter (0)
							{ }

		int _room;
		int _id;
		bool _canOpenClose;
		bool _openClose;
		int _timer;
		int _counter;
	};

	/** To simplify the management of a map of doors. */
	typedef std::map <int, DoorCounter*, std::less <int>> DoorCounters;
	typedef std::map <int, DoorCounters, std::less <int>> DoorCountersPerRoom;

	/** This is a basic counter to avoid a user exit and enter back a room
		with food after eat it and the food appears again inmediatly. */
	struct FoodCounter
	{
		FoodCounter ()
			: _room (0), _id (0), _eaten (false), _timer (0), _counter (0)
							{ }

		int _room;
		int _id;
		bool _eaten;
		int _timer;
		int _counter;
	};

	/** To simplify the management of a map of food. */
	typedef std::map <int, FoodCounter*, std::less <int>> FoodCounters;
	typedef std::map <int, FoodCounters, std::less <int>> FoodCountersPerRoom;

	// Constructors & Destructors
	AticAtacWorld (const QGAMES::Scenes& s, const QGAMES::WorldProperties& p);
	/** Clear the things attributes. */
	~AticAtacWorld ()
							{ clearThings (); }

	/** To know the room where the player is in. */
	int roomNumber () const
							{ return (_roomNumber); }
	/** To know the center of the current room. */
	QGAMES::Position centerOfCurrentRoom () const;
	/** To change the room where the player is in.
		The last two parameters indicate the romn and the object someone is comming from...if any.
		This is to set up the form and the aspect of the door the player is comming from. */
	void setRoomNumber (int r, int fR = -1, int fO = -1);

	/** To move the player. 
		The acceleration is not taken into account. */
	void moveCharacter (const QGAMES::Vector& d);

	/** When a food has been eaten. */
	void foodEaten (AticAtacFood* f);
	/** When something has been caught. */
	void thingCaught (AticAtacThingToCatch* t);
	/** When something has been left. 
		What is left is the last element caught. */
	void thingLeft ();
	/** To leave everything- */
	void thingsAllLeft ()
							{ _carrying.clear (); }
	/** To know the things the user has. */
	const ThingPositions& things () const
							{ return (_carrying); }
	/** To know what is in the current room. 
		It returns and object because it can be empty. */
	ThingPositions thingsInCurrentRoom () const;

	/** Add a tomb where the user has die. 
		Receive the position of the tomb. */
	void addTomb ();

	/** To open the exit door.
		This method is called normally when the exit keys are carried by the
		main character. */
	void openExitDoor ();
	/** Same but to close. */
	void closeExitDoor ();

	/** To shoot. */
	void shoot (int nS);
	/** Stop shooting. */
	void noShoot (int nS);

	/** The world moves the status of the doors of the castle and
		reinstate the food where it has been eaten.
		This method sets all those things to 0, and move the
		things to the original position (someone of them ramdom). */
	virtual void initialize ();
	/** To initialize the movile things in the game (keys,...) */
	void initializeThings ();
	/** Moving doors and reinstating food where it has been eaten. */
	virtual void updatePositions ();

	virtual void processEvent (const QGAMES::Event& e);

	private:
	/** To clear the things.
		A very internal method just called by the initialize method. */
	void clearThings ();
	/** To add a thing. 
		A very internal method just used during the creation of the world. */
	void addThingPosition (ThingPosition* tP);
	/** To add a food counter 
		A very internal method just used during the creation of the world. */
	void addFoodCounter (FoodCounter* fC);
	/** To add a door counter. 
		A very internal method during the creation of the world. */
	void addDoorCounter (DoorCounter* dC);

	private:
	/** The number of the room the player is now in.
		Initially this map is 0. */
	int _roomNumber; // Initially it is 0;
	/** The main character. */
	AticAtacCharacter* _mainCharacter;
	/** Things the main character is carring out. */
	ThingPositions _carrying;

	// Define a set of important buoys..
	/** Define a buoy to change the room. */
	class ToChangeRoomBuoy : public QGAMES::Buoy
	{
		public:
		ToChangeRoomBuoy ()
			: QGAMES::Buoy (__TOCHANGEROOMBUOYID, __BD 0)
							{ }

		void setData (AticAtacChangeRoomEventData* dt)
			{ _data = dt; }

		virtual void* treatFor (QGAMES::Element* e);

		private:
		AticAtacChangeRoomEventData* _data;
	};

	/** Define a buoy to change the status of the game. */
	class ToFallBuoy : public QGAMES::Buoy
	{
		public:
		ToFallBuoy ()
			: QGAMES::Buoy (__TOFFAILBUOYID, __BD 0)
							{ }

		void setData (AticAtacFallingEventData* dt)
			{ _data = dt; }

		virtual void* treatFor (QGAMES::Element* e);

		private:
		AticAtacFallingEventData* _data;
	};

	/** Define a buoy to finihes the game. */
	class ToFinishBuoy : public QGAMES::Buoy
	{
		public:
		ToFinishBuoy ()
			: QGAMES::Buoy (__TOFINISHBUOYID, __BD 0)
							{ }

		void setData (AticAtacFinishEventData* dt)
			{ _data = dt; }

		virtual void* treatFor (QGAMES::Element* e);

		private:
		AticAtacFinishEventData* _data;
	};

	// Implementation...
	/** A Variable to manage doors status. */
	DoorCountersPerRoom _doorCounters;
	/** The same but don't clasified by room. */
	DoorCounters _doorCountersSimple;
	/** A variable to manage food status. */
	FoodCountersPerRoom _foodCounters;
	/** The same but don't clasified by room. */
	FoodCounters _foodCountersSimple;
	/** A variable to manage the position of th things.
		Remember things can be caught and can be left. 
		So, it is important to know where every object is. */
	ThingPositionsPerRoom _thingPositions;
	/** The same but don't clasified by room. */
	ThingPositions _thingPositionsSimple;
	/** The position of the tombs. */
	ThingPositions _tombs;
	/** A counter for the tombs; */
	int _tombCounter;
	/** The door id of the exi doors. */
	int _exitDoorIds [2];
};

#endif
