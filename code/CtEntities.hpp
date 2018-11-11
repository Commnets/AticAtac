/** \addtogroup Atic */
/*@{*/

#ifndef __CTENTITIES_HPP__
#define __CTENTITIES_HPP__

#include "General.hpp"
#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

/** This class represents an atic atac thing that can be caught. */
class AticAtacThingToCatch : public QGAMES::Artist
{
	public:
	// Constructors
	AticAtacThingToCatch (int id, const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ()) 
			: QGAMES::Artist (id, f, d),
			  _dObjDefinition ()
							{ setVisible (false); }

	virtual Entity* clone () const
							{ return (new AticAtacMonster (_id, _forms, _data)); }

	/** To set up different forms of the thing to catch. */
	void setAspectTo (const General::ThingDefinition& df); // To any other thing to be caught...
	/** To get the id of the final aspect. */
	int aspectId () const
							{ return (_dObjDefinition._id); }
	/** To get what the thing caiought is. */
	General::WhatIs whatIs () const
							{ return (_dObjDefinition._whatIs); }

	/** The position is updated, moving between the initial and the final frame. */
	virtual void updatePositions ();
	/** The thing is only drawn when it is visible. */
	virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

	private:
	/** What the thing is. */
	General::ThingDefinition _dObjDefinition;
};

#endif
  
// End of the file
/*@}*/