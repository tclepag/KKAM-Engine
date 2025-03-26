#include "Core/EntManager.h"

namespace Core {
	EntRegistry EntManager::GlobalRegistry_ = {};
	uint16_t EntManager::GlobalIndex_ = 0;

	void EntManager::RegisterEnt(Ent& Entity) {
		// Register this into our registry
		Registry_[Index_] = &Entity;

		// If this entity is derived from EntityObject then we assign its scene id,
		// technically this is a local id not a "scene" id but the classes that mostly use this
		// will be a Scene class
		if (auto entObj = dynamic_cast<EntObj*>(&Entity); entObj != nullptr) {
			entObj->SetSID(Index_);
		}

		// Increment for next entity
		Index_ += 1;

		// Register this entity to the global registry if they weren't already
		if (!WasEntityRegisteredGlobally(Entity)) {
			GlobalRegistry_[GlobalIndex_] = &Entity;
			Entity.SetEID(GlobalIndex_);
			GlobalIndex_ += 1;
		}
	}
	void EntManager::UnregisterEnt(Ent& Entity) {
		Registry_[GetEntIndex(Entity)] = nullptr;
	}

	Ent* EntManager::FindEnt(uint16_t Index) {
		for (auto& [IND, ENT] : Registry_) {
			if (IND == Index) {
				return ENT;
			}
		}
	}

	uint16_t EntManager::GetEntIndex(Ent& Entity) {
		for (auto& [IND, ENT] : Registry_) {
			if (ENT == &Entity) {
				return IND;
			}
		}
		return INVALID_ID;
	}

	bool EntManager::WasEntityRegisteredGlobally(Ent& Entity) {
		return GetEntIndexGlobal(Entity) != INVALID_ID;
	}

	Ent* EntManager::FindEntGlobal(uint16_t GlobalIndex) {
		for (auto& [IND, ENT] : GlobalRegistry_) {
			if (IND == GlobalIndex) {
				return ENT;
			}
		}
	}

	uint16_t EntManager::GetEntIndexGlobal(Ent& Entity) {
		for (auto& [IND, ENT] : GlobalRegistry_) {
			if (ENT == &Entity) {
				return IND;
			}
		}
		return INVALID_ID;
	}
}