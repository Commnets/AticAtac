/** \addtogroup Atic */
/*@{*/


#ifndef __ETENTITIES_HPP__
#define __ETENTITIES_HPP__

#include "General.hpp"
#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

/** This class represents an atic atac food. */
class AticAtacFood : public QGAMES::Artist
{
	public:
	// Constructors
	AticAtacFood (int id, const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ()) 
			: QGAMES::Artist (id, f, d),
			  _dObjDefinition ()
							{ setVisible (false); }

	virtual Entity* clone ()
							{ return (new AticAtacMonster (_id, _forms, _data)); }

	/** To set up the final aspect of the food. */
	void setAspectTo (const General::ThingDefinition& df); 
	/** To get the id of the final aspect. */
	int aspectId () const
							{ return (_dObjDefinition._id); }
	/** To get what the thing caiought is. */
	General::WhatIs whatIs () const
							{ return (_dObjDefinition._whatIs); }
	
	/** The collision zone is the real core of the food.
		It is a little bit smaller than the standard collision zone. */
	virtual QGAMES::Rectangle collisionZone () const;

	/** The position is updated, moving between the initial and the final frame. */
	virtual void updatePositions ();
	/** The food is only drawn when it is visible. */
	virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

	private:
	/** What it is. */
	General::ThingDefinition _dObjDefinition;
};

#endif
  
// End of the file
/*@}*/