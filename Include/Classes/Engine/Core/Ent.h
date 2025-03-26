#pragma once

#include <cstdint>
#include <string>

namespace Core {
	class EntManager;
}

namespace Classes::Engine::Core {
	using EID = uint16_t;
	using EName = std::string;
	constexpr uint16_t INVALID_ID = 0xFFFF;
	constexpr const char* DEFAULT_NAME = "Unnamed";

	/// <summary>
	/// Base class for all entities in the engine
	/// </summary>
	class Ent {
		friend class ::Core::EntManager;
	public:
		// The entity's global index
		const EID& EId;
		// Human-readable entity name
		EName Name = DEFAULT_NAME;

		Ent(const std::string& Name) : Name(Name), EId(EId_) {}

		bool IsPendingDeath() const { return PendingDeath_; }
		void Kill() {
			PendingDeath_ = true;
		}
	protected:
		virtual void OnInit() = 0;
		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
	private:
		// Global Entity ID
		EID EId_ = INVALID_ID;
		bool PendingDeath_ = false;

		void SetEID(const EID& EId) {
			EId_ = EId;
		}
	};
}