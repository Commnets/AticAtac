/** \addtogroup Atic */
/*@{*/

#ifndef __GENERAL_HPP__
#define __GENERAL_HPP__

#include <Arcade/arcadeinclude.hpp>

/** General basic variables of the game. */
class General
{
	public:
	/** Enum to define the type of structure. */
	typedef enum 
	{
		// Things to eat or to catch...
		__FOOD = 0,
		__AMULET = 1,
		__KILLMUMMY = 2, // The leaf
		__KILLFRANK = 3, // The wrench
		__KILLDEVIL = 4, // The cross
		__KILLDRACULA = 5, // Not possible to kill?
		__KILLHUNCH = 6, // The frog - leg
		__GREENKEY = 7,
		__YELLOWKEY = 8,
		__REDKEY = 9,
		__WHITEKEY = 10,
		__ACGKEY1 = 11,
		__ACGKEY2 = 12,
		__ACGKEY3 = 13,
		// Things to kill...
		__MUMMY = 14, 
		__FRANK = 15,
		__DEVIL = 16,
		__DRACULA = 17,
		__HUNCH = 18,
		__MUSHROOM = 19,
		// Types of doors...
		__NORMALDOR = 20,
		__GREENDOOR = 21,
		__YELLOWDOOR = 22,
		__REDDOOR = 23,
		__WHITEDOOR = 24,
		__TRAPDOOR = 25,
		__BARRELDOOR = 26,
		__BOOKCASEDOOR = 27,
		__CLOCKDOOR = 28,
		__EXITDOOR = 29,
		// The generic things...
		__GENERICBKOBJECT = 30,
		__GENERICTHING = 31,
		// The tobmstone...
		__TOMBSTONE = 32,
		// Other things...
		__BOTTLE = 33,
		__FACECOIN = 34,
		__WING = 35,
		__WHIP = 36,
		__DIAMOND = 37,
		__BAGOFMONEY = 38,
		__SKULL = 39,
		__TABLE = 40,
		__BARRELS = 41,
		__SKELETON = 42
	} WhatIs;

	/** Enum to define where the form is located. */
	typedef enum 
	{
		__NONE = 0,
		__RIGHT = 1,
		__LEFT = 2,
		__UP = 3,
		__DOWN = 4,
	} Orientation;

	/** Th eonly attribute of this type. */
	static General _e;

	// Constructors
	/** Just to create an empty instance. */
	General ();

	// To initialize the internal data
	// This method is called at the main procedure...
	void initialize ();

	public:
	/** The lines to draw a room. */
	struct LineToDraw
	{
		LineToDraw ()
			: _origin (QGAMES::Position::_cero), _end (QGAMES::Position::_cero) 
							{ }

		/** The starting point */
		QGAMES::Position _origin; 
		/** The ending point */
		QGAMES::Position _end; 
	};

	// To simplify definition...
	/** The definition of a room is just a couple of lines to be drawn. */
	typedef std::vector <LineToDraw> RoomPicture;
	/** A map with all the lines to be drawn per room is defined to simplify later options. */
	typedef std::map <int, RoomPicture> RoomTypes;

	/** The form of an object. */
	struct FormDefinition
	{
		FormDefinition ()
			: _id (0),
			  _form (0), _initialFrame (0), _finalFrame (0),
			  _orientation (__NONE),
			  _adjust (QGAMES::Position::_cero)
							{ }

		/** An id. */
		int _id;
		/** The number of the form defining something. */
		int _form;
		/** The initial frame into the form definition for this object. */
		int _initialFrame;
		/** The final one. */
		int _finalFrame;
		/** The orientation of the form.
			This is where the form is located: Right/Left/Up/Down. */
		Orientation _orientation;
		/** How much the original position of the object should be adjust
			taking into account the current form? */
		QGAMES::Position _adjust;
	};

	// TO simplify further definitions...
	/** To simplify the definition of a list of forms. */
	typedef std::vector <FormDefinition> FormsDefinitionList;

	/** The definition of any object in a room. */
	struct ObjectDefinition
	{
		ObjectDefinition ()
			: _id (0),
			  _whatIs (__GENERICBKOBJECT),
			  _inRoom (0), _forms (), _isConnected (false), 
			  _connectedToObj (0), _connectedToRoom (0), _position (QGAMES::Position::_cero),
			  _blockMovement (false),
			  _time (0)
							{ }

		/** And id. */
		int _id;
		/** What it is. */
		WhatIs _whatIs;
		/** The room number this object is located */
		int _inRoom; 
		/** The forms componing the object */
		FormsDefinitionList _forms; 
		/** To know wheter it is connected or not... */
		bool _isConnected; 
		/** The object this other one is connected to...if any (0 if not) */
		int _connectedToObj; 
		/** The room where the object this one is connected is in...if any (0 if not) */
		int _connectedToRoom;
		/** Where the object should be placed **/
		QGAMES::Position _position; 
		/** When it is a door (or similar: a clock i.e.) the time it is open / closed */
		int _time;
		/** When it is a table or something in the middle of the room. 
			The player collision with it and can not move over it. */
		bool _blockMovement;
		// Objects are everything including the doors...
	};

	// To simplify further definitions...
	/** A way to simplify the definition of a list of objects. */
	typedef std::vector <ObjectDefinition> Objects;
	/** The object can be located per room. */
	typedef std::map <int, Objects> ObjectsPerRoom;

	struct ThingDefinition
	{
		public:
		ThingDefinition ()
			: _id (0),
			  _whatIs (__GENERICTHING),
			  _inRoom (0),
			  _forms (),
			  _position (QGAMES::Position::_cero),
			  _time (0)
							{ }

		/** An id. */
		int _id;
		/** What it is. */
		WhatIs _whatIs;
		/** The room number this thing is located */
		int _inRoom; 
		/** The forms componing the object */
		FormsDefinitionList _forms; 
		/** Where the thing should be placed **/
		QGAMES::Position _position;
		/** When it is food time to re - appear when eaten. */
		int _time; 
	};

	// To simplify further accesses...
	/** A list f things. */
	typedef std::vector <ThingDefinition> Things;

	/** The definition of a concrete room. */
	struct RoomDefinition
	{
		RoomDefinition ()
			: _id (0),
			  _color (QGAMES::Color (0,0,0)), _type (0),
			  _cx (0), _cy (0), _w (0), _h (0),
			  _objects (), _thingsToEat (), _thingsToCatch (), _monsters ()
							{ }

		/** An id. */
		int _id;
		/** The color of the lines to draw the room */
		QGAMES::Color _color; 
		/** The type of room the concrete room is (see above) */
		int _type; 
		/** The dimensions of the room... */
		int _cx, _cy, _w, _h; 
		/** The object componing the room */
		Objects _objects; 
		/** The list of possible things to be eaten */
		Things _thingsToEat; 
		/** The list of possible things to be caught */
		Things _thingsToCatch; 
		/** The list of monsters */
		Things _monsters; 
	};

	// To simplify further accesses...
	/** A vector to simplify the rooms of the game. */
	typedef std::vector <RoomDefinition> Rooms;
	/** A vector to simplify the access to a list of rooms */
	typedef std::vector <int> RoomNumbers;
	/** A vector to know where the exit keys can be located. */
	typedef std::vector <RoomNumbers> ExitKeyRoomsPossible;

	// And, after definitions, the objects to play with
	RoomTypes _roomTypes; // The types of room...
	ObjectsPerRoom _objectsPerRoom; // The objects per room...
	Objects _objects; // All the objects one after other...
	Things _things; // All the things one after other...
	Rooms _rooms; // The rooms...
	RoomNumbers _greenKeyRooms, _yellowKeyRooms, _redKeyRooms, _cyanKeyRooms;
	ExitKeyRoomsPossible _exitKeyRooms;
	ThingDefinition _mummy;
	ThingDefinition _devil;
	ThingDefinition _dracula;
	ThingDefinition _hunchback;
	ThingDefinition _frankestein;
	ThingDefinition _mushrooms;
	ThingDefinition _appearing;
	ThingDefinition _dieing;
	ThingDefinition _appearingMonstersLeft;
	ThingDefinition _appearingMonstersRight;
	ObjectDefinition _tombStone;

	private:
	void readRoomDefinition ();
	void readRoomObjects ();
	void readRooms ();
	void readThings ();
	void createSpecialTnings ();
	void assignKeys ();

	// Implementation
	QGAMES::Position convertPoint (const QGAMES::Position& p);
	QGAMES::Color convertColor (int c);
	FormsDefinitionList convertForm (int f, int fg);
	FormDefinition form (int id, int f, int iF, int fF, Orientation o = __NONE,
		const QGAMES::Position& a = QGAMES::Position::_cero);
};

#endif
  
// End of the file
/*@}*/
