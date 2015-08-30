#ifndef __ENTITIESBUILDER_HPP__
#define __ENTITIESBUILDER_HPP__

#include <Common/entitybuilder.hpp>

class EntitiesBuilder : public QGAMES::EntityBuilder
{
	public:
	EntitiesBuilder (const std::string& fDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
		: QGAMES::EntityBuilder (fDef, fB, mB)
							{ }
	
	private:
	QGAMES::Entity* createEntity (const QGAMES::EntityBuilder::EntityDefinition& def);
};

#endif

