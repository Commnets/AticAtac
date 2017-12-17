/** \addtogroup Atic */
/*@{*/

#ifndef __WORLDSBUILDER_HPP__
#define __WORLDSBUILDER_HPP__

#include <Arcade/arcadeinclude.hpp>

class WorldsBuilder : public QGAMES::WorldBuilder
{
	public:
	WorldsBuilder (const std::string& fDef, QGAMES::MapBuilder* mp)
		: QGAMES::WorldBuilder (fDef, mp)
							{ }

	private:
	virtual QGAMES::World* createWorldObject (int no, const QGAMES::Scenes& s, 
		const QGAMES::WorldProperties& p);
	virtual QGAMES::Scene* createSceneObject (int ns, const QGAMES::Maps& m, 
		const QGAMES::Scene::Connections& cn, const QGAMES::SceneProperties& p, 
		const QGAMES::EntitiesPerLayer& ePL);
};

#endif
  
// End of the file
/*@}*/