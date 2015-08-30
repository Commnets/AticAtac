#ifndef __ETENTITIES_HPP__
#define __ETENTITIES_HPP__

#include <Common/entity.hpp>
#include <Common/rectangle.hpp>
#include <Arcade/artist.hpp>
#include "General.hpp"
#include "Defs.hpp"


/** This class represents an atic atac food. */
class AticAtacFood : public QGAMES::Artist
{
	public:
	// Constructors
	AticAtacFood (int id, const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ()) 
			: QGAMES::Artist (id, f, d),
			  _dObjDefinition (),
			  _isVisible (false)
							{ /** Nothing else to do . */ }

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

	/** To know whether the food object is or not visible now. */
	bool isVisible () const
							{ return (_isVisible); }
	/** To activate or desactivate if the food object is visible. 
		In a room (scene) there would be different food (up to a maximum of 2).
		The meal in the scene always will be the "same" entities.
		Their aspects and positions will change accorging to the room the player is in. */
	void setVisible (bool v)
							{ _isVisible = v; }

	/** The position is updated, moving between the initial and the final frame. */
	virtual void updatePositions ();
	/** The food is only drawn when it is visible. */
	virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

	private:
	/** What it is. */
	General::ThingDefinition _dObjDefinition;
	/** To determinate whether the food is or not visible. */
	bool _isVisible;
};

#endif