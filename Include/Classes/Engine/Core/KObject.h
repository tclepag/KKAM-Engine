#pragma once

#include "KModule.h"
#include "KEntity.h"
#include <vector>
#include <cstdint>

namespace Core {
	class KEngine;
}

namespace Classes::Engine::Core {
	using SID = uint16_t;
	using Mods = std::vector<KModule>;

	/// <summary>
	/// Base class for all KScene entities
	/// </summary>
	class KObject : public KEntity {
        friend class ::Core::KEngine;
	public:
		KObject(const std::string& Name) : KEntity(Name) {}

		// Returns the KModule for this entity
		template <typename T>
		T* GetMod();
		// Returns a KModule by name
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
		Mods Mods_ = {};
	};
}