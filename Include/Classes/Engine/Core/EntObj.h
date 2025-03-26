#pragma once

#include <vector>
#include <cstdint>
#include "Ent.h"

namespace Core {
	class EntManager;
}

namespace Classes::Engine::Core {
	class Mod;

	using SID = uint16_t;
	using Children = std::vector<EID>;
	using Mods = std::vector<Mod>;

	/// <summary>
	/// Base class for all scene entities
	/// </summary>
	class EntObj : public Ent {
        friend class ::Core::EntManager;
	public:
		// The entity's scene index
		const SID& SId;

		EntObj(const std::string& Name): Ent(Name), SId(SId_) {}

		// Returns the scene that owns this entity
		EntObj& GetScene();
		// Returns an entity parented to this object by entity id
		EntObj& GetEnt(const EID& EntId);
		// Returns an entity parented to this object by name
		EntObj& GetEnt(const EName& EntName);
		// Returns all entities parented to this object
		std::vector<EntObj&> GetAllEnts();

		// Returns the mod for this entity
		template <typename T>
		T* GetMod();
		// Returns a mod by name
		template <typename T>
		T* GetMod(const std::string& Name);
		// Returns all mods for this entity
		Mods GetMods() { return Mods_; }
		// Adds a modifier to this class
		template <typename T>
		T* AttachMod(T* Modifier);
		// Creates a modifier and adds it to this class
		template <typename T>
		T* AttachMod();
		// Removes a modifier from this class
		template <typename T>
		void DetachMod(T* Modifier);
		// Removes a modifier by its type
		template <typename T>
		void DetachMod();
	private:
		SID SId_ = INVALID_ID;
		Children Children_ = {};
		Mods Mods_ = {};

		void SetSID(const SID& SId) {
			SId_ = SId;
		}
	};
}