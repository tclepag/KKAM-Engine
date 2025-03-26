#pragma once

#include "Classes/Engine/Core/Ent.h"
#include "Classes/Engine/Core/EntObj.h"

#include "Common/Common.h"

namespace Core {
	using namespace Classes::Engine::Core;

	using EntRegistry = std::unordered_map<uint16_t, Ent*>;

	class EntManager {
	public:
		// Adds an entity to this registry
		void RegisterEnt(Ent& Entity);
		// Removes an entity from this registry
		void UnregisterEnt(Ent& Entity);
		// Returns an entity's index in this registry
		uint16_t GetEntIndex(Ent& Entity);
		// Returns the entity thats at this index
		Ent* FindEnt(uint16_t Index);
	private:
		uint16_t Index_;
		static uint16_t GlobalIndex_;
		EntRegistry Registry_;
		static EntRegistry GlobalRegistry_;

		static bool WasEntityRegisteredGlobally(Ent& Entity);
		// Returns an entity's global index in the global registry
		static uint16_t GetEntIndexGlobal(Ent& Entity);
		// Returns the entity thats at the global index
		static Ent* FindEntGlobal(uint16_t GlobalIndex);
	};
}