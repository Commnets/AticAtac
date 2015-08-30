#ifndef __MTENTITIES_HPP__
#define __MTENTITIES_HPP__

#include <Common/entity.hpp>
#include <Common/rectangle.hpp>
#include <Arcade/artist.hpp>
#include "General.hpp"
#include "Defs.hpp"

class AticAtacCharacter;

/** This class represents an atic atac monster. */
class AticAtacMonster : public QGAMES::Artist
{
	public:
	/** Class to determinate the main aspects of the monster's behaviour. */
	class Behaviour 
	{
		public:
		// Constructors & Destructors
		Behaviour ()
			: _monster (NULL),
			  _parameters ()
							{ }
		virtual ~Behaviour () 
							{ }

		/** To set the monster this behaviour is for. */
		void setMonster (AticAtacMonster* m)
							{ _monster = m; }
		/** To set the variables of the behaviour.
			The type of variables will depend on the type of monster. 
			The behaviour is not the owner of the parameters received, so it
			has not to destroy them after finish. */
		void setParameters (const std::vector <void*>& prs)
							{ _parameters = prs; }
		/** To correct the speed of the movement. 
			By default the result isthe same. */
		virtual int correctSpeed (int s, int bs)
							{ return (s); }
		/** Returns true when the monster can died due to a player weapon.
			By default the answer will be negative. */
		virtual bool canDie () const
							{ return (false); }
		/** Given a monster, and having the possibility of knowing its position.
			orientation,... this method should determinate the next orientation. */
		virtual QGAMES::Vector potentialNewOrientation (AticAtacMonster* e) // By default do not move...
							{ return (QGAMES::Vector::_cero); }
		/** Given a monster, and having the possibility of knowing its position.
			orientation,... this method should determinate the next aspect. */
		virtual General::ThingDefinition& aspectToOrientation (AticAtacMonster* e) = 0;

		protected:
		/** This protected method consideres the limits of the room to change the direction. 
			This limit is for any king of monter moving into the limits. */
		void changeDirectionConsideringLimits (QGAMES::bdata& x, QGAMES::bdata& y, 
			AticAtacMonster* e);

		protected:
		/** A reference to the monster this behaviour belongs to. */
		AticAtacMonster* _monster;
		/** The list of parameters this behaviour need to work. */
		std::vector <void*> _parameters;
	};

	/** To simplify a vector of behaviours. */
	typedef std::vector <Behaviour*> Behaviours;

	/** The bahaviour is always a fridn of the monster.
		It has to access to internal monster information in some cases. */
	friend Behaviour;

	/** The different status a monster can have. */
	enum Status { __NONE = 0, __APPEARING = 1, __MOVING = 2, __DIEING = 3 };

	// Constructors
	/** Creates a monster. */
	AticAtacMonster (int id, const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

	virtual Entity* clone ()
							{ return (new AticAtacMonster (_id, _forms, _data)); }

	/** To know whether the monster can die or not duw to the weapon. */
	bool canDie () const;
	/** To set up the final aspect of the monster. */
	void setAspectTo (const General::ThingDefinition& df, Status st = Status::__APPEARING);
	/** To change the aspect but maintaining the selected original form. 
		Take care whether a previous aspect exists. Otherwise, */
	void changeAspectTo (const General::ThingDefinition& df);
	/** To know what it is. */
	General::WhatIs whatIs () const
							{ return (_mtDefinition._whatIs); }
	/** To change the behaviour. Those will be empty by default. */
	void setBehaviour (Behaviour* b, const std::vector <void*>& prms =
		std::vector <void*> ());
	/** To know the behaviour */
	Behaviour* behaviour () 
							{ return (_behaviour); }

	/** To know whether the monster is or not visible now. */
	bool isVisible () const
							{ return (_isVisible); }
	/** To activate or desactivate if the monster object is visible. 
		In a room (scene) there would be different monsters (up to a maximum of 5).
		The monsters in the scene always will be the "same" entities.
		Their aspects and positions will change accorging to the room the player is in. */
	void setVisible (bool v)
							{ _isVisible = v; }

	/** Set the limits to move the monster....when it moves
		Two versions of the method... */
	void setMovingLimits (const QGAMES::Position& min, const QGAMES::Position& max);
	void setMovingLimits (const QGAMES::Rectangle& l)
							{ setMovingLimits (l.pos1 (), l.pos2 ()); }
	
	/** The collision zone is the real core of the monster.
		It is a little bit smaller than the standard collision zone. */
	virtual QGAMES::Rectangle collisionZone () const;

	/** The initialization is necessary.
		All the monster entities are not visible at the beggining. */
	virtual void initialize ();
	/** The position is updated, moving between the initial and the final frame. */
	virtual void updatePositions ();
	/** The monster is only drawn when it is visible. */
	virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

	private:
	/** To actualize the direction and the aspect (if any) of the monster
		when moving. The concrect direction and aspect will depend on the specific monster. */
	virtual void actualizePositionWhenMove ();

	private:
	/** To determinate whether the monster is or not visible. */
	bool _isVisible;
	/** The status of the monster. */
	Status _status;
	/** The behaviour of the monster.
		It is something that depends on the monster itself. */
	Behaviour* _behaviour;

	// Implementation...
	/** The aspect of the monster...getting from the General class. */
	General::ThingDefinition _mtDefinition;
	/** The form representing the monster. 
		After it has been selected randomically from teh definition (if there are many) */
	General::FormDefinition _formDefined;
	/** The number of form defined. */
	int _numberFormDefined;
	/** Counter to count steps to change the aspect. */
	int _counter;
	/** Counter To count the number of changes of aspect. */
	int _counterChanges;
};

/** Class to represent the behaviour related with a monster appearing in the
	room after a couple of seconds. */
class MonsterAppearingBehaviour : public AticAtacMonster::Behaviour
{
	public:
	MonsterAppearingBehaviour ()
		: AticAtacMonster::Behaviour ()
							{ }

	/** @see parent. */
	/** This kind of monster can die due to a weapon. */
	virtual bool canDie () const
							{ return (true); }

	/** @see parent. */
	virtual QGAMES::Vector potentialNewOrientation (AticAtacMonster* e);
	/** @see parent. */
	virtual General::ThingDefinition& aspectToOrientation (AticAtacMonster* e);
};

/** Class representing the behaviour of the mummy.
	The mummy takes into account if the leaf is in the room.
	Otherwhise the mummy will follow the main character. */
class MonsterMummyBehaviour : public AticAtacMonster::Behaviour
{
	public:
	MonsterMummyBehaviour ()
		: AticAtacMonster::Behaviour ()
							{ }


	/** @see parent. */
	virtual int correctSpeed (int s, int bs)
							{ return (bs); }
	/** @see parent. */
	virtual QGAMES::Vector potentialNewOrientation (AticAtacMonster* e);
	/** @see parent. */
	virtual General::ThingDefinition& aspectToOrientation (AticAtacMonster* e)
							{ return (General::_e._mummy); }
};

/** Class representing the behaviour of the devil. */
class MonsterDevilBehaviour : public AticAtacMonster::Behaviour
{
	public:
	MonsterDevilBehaviour ()
		: AticAtacMonster::Behaviour ()
							{ }

	/** @see parent. */
	virtual QGAMES::Vector potentialNewOrientation (AticAtacMonster* e);
	/** @see parent. */
	virtual General::ThingDefinition& aspectToOrientation (AticAtacMonster* e)
							{ return (General::_e._devil); }
};

/** Class representing the behaviour of Dracula. */
class MonsterDraculaBehaviour : public AticAtacMonster::Behaviour
{
	public:
	MonsterDraculaBehaviour ()
		: AticAtacMonster::Behaviour ()
							{ }

	/** @see parent. */
	virtual QGAMES::Vector potentialNewOrientation (AticAtacMonster* e);
	/** @see parent. */
	virtual General::ThingDefinition& aspectToOrientation (AticAtacMonster* e)
							{ return (General::_e._dracula); }
};

/** Class representing the behaviour of the Hunchback. */
class MonsterHunchbackBehaviour : public AticAtacMonster::Behaviour
{
	public:
	MonsterHunchbackBehaviour ()
		: AticAtacMonster::Behaviour ()
							{ }

	/** @see parent. */
	virtual QGAMES::Vector potentialNewOrientation (AticAtacMonster* e);
	/** @see parent. */
	virtual General::ThingDefinition& aspectToOrientation (AticAtacMonster* e)
							{ return (General::_e._hunchback); }
};

/** Class representing the behaviour of the Frankestein. */
class MonsterFrankesteinBehaviour : public AticAtacMonster::Behaviour
{
	public:
	MonsterFrankesteinBehaviour ()
		: AticAtacMonster::Behaviour ()
							{ }

	/** @see parent. */
	virtual QGAMES::Vector potentialNewOrientation (AticAtacMonster* e);
	/** @see parent. */
	virtual General::ThingDefinition& aspectToOrientation (AticAtacMonster* e)
							{ return (General::_e._frankestein); }
};

/** Class representing the behaviour of the mushrooms. 
	The mushroom doesn move, and they have always the same aspect. */
class MonsterMushroomBehaviour : public AticAtacMonster::Behaviour
{
	public:
	MonsterMushroomBehaviour ()
		: AticAtacMonster::Behaviour ()
							{ }

	/** @see parent. */
	virtual General::ThingDefinition& aspectToOrientation (AticAtacMonster* e)
							{ return (General::_e._mushrooms); }
};

#endif