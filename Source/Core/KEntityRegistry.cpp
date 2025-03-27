#include "Core/KEntityRegistry.h"

namespace Core {
	void KEntityRegistry::RegisterEntity(KEntity* Entity) {
		KEntityData data{};
		data.Entity = Entity;
		data.EntityID = RegistryInt_;
		Entities_[RegistryInt_] = data;
		Entity->SetEID(data.EntityID);
		RegistryInt_ += 1;
	}

	void KEntityRegistry::UnregisterEntity(const size_t& EntityID) {
		// Make sure we have a valid entity
		if (Entities_.count(EntityID) <= 0) {
			return;
		}
		auto& data = Entities_[EntityID];
		// First set parent to invalid id, to remove any possible references to this entity
		SetEntityParent(data.EntityID, INVALID_ID);

		// Now invalidate this entity, usually we invalidate entities before destruction
		data.EntityID = INVALID_ID;
		Entities_.erase(EntityID);
	}

	void KEntityRegistry::SetEntityParent(const size_t& EntityID, const size_t& ParentID) {
		// Make sure we have a valid entity to parent
		if (Entities_.count(EntityID) <= 0) {
			return;
		}
		auto& data = Entities_[EntityID];

		// Remove this entity from an parent
		// if they are parented already
		if (data.ParentID && Entities_.count(data.ParentID) > 0) {
			auto& oldParentData = Entities_[data.ParentID];
			auto newEnd = std::remove(oldParentData.Children.begin(), oldParentData.Children.end(), data.EntityID);
			oldParentData.Children.erase(newEnd, oldParentData.Children.end());
		}

		// Set parent only if we have an parent entity
		if (Entities_.count(ParentID) <= 0) {
			// Get new parent's id
			auto& parentData = Entities_[ParentID];

			// Set children and add to the parentData array
			data.ParentID = parentData.EntityID;
			parentData.Children.emplace_back(data.EntityID);

			// Iterate through parent hierarchy to get
			// root entity id (root objects do not have a parent)
			KEntityData cur = data;
			while (cur.ParentID != INVALID_ID) {
				if (Entities_.count(cur.ParentID) <= 0) {
					break;
				}
				cur = Entities_[cur.ParentID];
			}

			data.RootID = cur.EntityID;
			data.Entity->SetRID(data.RootID);
			data.Entity->SetPID(ParentID);
		}
		else {
			// Set all other entity id references to INVALID_ID
			// This will turn an entity into a root object
			data.ParentID = INVALID_ID;
			data.RootID = INVALID_ID;
			data.Entity->SetRID(INVALID_ID);
			data.Entity->SetPID(INVALID_ID);
		}
	}

	KEntityData* KEntityRegistry::GetEntity(const size_t& EntityID) {
		return &Entities_[EntityID];
	}
}