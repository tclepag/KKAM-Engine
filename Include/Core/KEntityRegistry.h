#pragma once

#include <cstdint>
#include <map>

#include "Classes/Engine/Core/KEntity.h"
#include "Classes/Engine/Core/KObject.h"

namespace Core {
	using namespace Classes::Engine::Core;

	struct KEntityData {
		// The Entity's current ID
		size_t EntityID = INVALID_ID;
		// The object that this entity is currently below
		size_t ParentID = INVALID_ID;
		// The highest parent in the hierarchy for this object
		size_t RootID = INVALID_ID;
		std::vector<size_t> Children;
		// The entity of this data
		KEntity* Entity;
	};

	using KEntities = std::map<size_t, KEntityData>;

	class KEntityRegistry {
	public:
		KEntityRegistry(KEngine* Engine): Engine_(Engine) {}
		~KEntityRegistry() {}
		void RegisterEntity(KEntity* Entity);
		void UnregisterEntity(const size_t& Entity);
		void SetEntityParent(const size_t& EntityID, const size_t& ParentID);
		KEntityData* GetEntity(const size_t& EntityID);
	private:
		KEngine* Engine_;
		KEntities Entities_;
		size_t RegistryInt_ = 0;
	};
}