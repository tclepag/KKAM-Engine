#pragma once

#include "Classes/Engine/Core/KObject.h"
#include <memory>

namespace Core {
	class KEntityManager;
}

namespace Classes::Engine {
	using namespace Engine::Core;
	class KScene : public KEntity {
	public:

		// Parents an entity to this KScene
		void AddEnt(KEntity& Entity);
		// Removes an entity from this KScene
		void RemoveEnt(KEntity& Entity);
		// Returns an entity parented to this object by entity id
		KObject& GetEnt(const EID& EntId);
		// Returns an entity parented to this object by KScene id
		KObject& GetEnt(const SID& SceneId);
		// Returns an entity parented to this object by name
		KObject& GetEnt(const EName& EntName);
		// Returns all entities parented to this object
		std::vector<KObject&> GetAllEnts();

		// TODO: Camera class needed
	protected:
		void OnUpdate() override;
		void OnRender() override;
	private:
		std::unique_ptr<::Core::KEntityManager> Ents_;
	};
}