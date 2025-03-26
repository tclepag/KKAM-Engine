#pragma once

#include "Classes/Engine/Core/EntObj.h"
#include <memory>

namespace Core {
	class EntManager;
}

namespace Classes::Engine {
	using namespace Engine::Core;
	class Scene : public Ent {
	public:

		// Parents an entity to this scene
		void AddEnt(Ent& Entity);
		// Removes an entity from this scene
		void RemoveEnt(Ent& Entity);
		// Returns an entity parented to this object by entity id
		EntObj& GetEnt(const EID& EntId);
		// Returns an entity parented to this object by scene id
		EntObj& GetEnt(const SID& SceneId);
		// Returns an entity parented to this object by name
		EntObj& GetEnt(const EName& EntName);
		// Returns all entities parented to this object
		std::vector<EntObj&> GetAllEnts();

		// TODO: Camera class needed
	protected:
		void OnUpdate() override;
		void OnRender() override;
	private:
		std::unique_ptr<::Core::EntManager> Ents_;
	};
}