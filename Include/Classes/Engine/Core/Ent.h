#pragma once

#include <cstdint>
#include <string>

namespace Classes::Engine::Core {
	using EID = uint16_t;
	using EName = std::string;
	constexpr uint16_t INVALID_ID = 0xFFFF;
	constexpr const char* DEFAULT_NAME = "Unnamed";

	/// <summary>
	/// Base class for all entities in the engine
	/// </summary>
	class Ent {
	public:
		// The entity's global index
		const EID& GId;
		// Human-readable entity name
		EName Name = DEFAULT_NAME;

		Ent(const std::string& Name) : Name(Name), GId(GId_) {}

		bool IsPendingDeath() const { return PendingDeath_; }
		void Kill() {
			PendingDeath_ = true;
		}
	private:
		// Global Entity ID
		EID GId_ = INVALID_ID;
		bool PendingDeath_;

		void SetGEID(const EID& GId) {
			GId_ = GId;
		}
	};
}