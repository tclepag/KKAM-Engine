#pragma once

#include <cstdint>
#include <string>

namespace Core {
	class KEngine;
	class KEntityRegistry;
};
using namespace Core;

namespace Classes::Engine::Core {
	constexpr size_t INVALID_ID = 0xFFFF;
	using Name = std::string;
	constexpr const char* DEFAULT_NAME = "Unnamed";

	/// <summary>
	/// Base class for all entities in the KEngine
	/// </summary>
	class KEntity {
		friend class KEntityRegistry;
	public:
		// The entities id
		const size_t& EID = INVALID_ID;
		// The entities parent id
		const size_t& PID = INVALID_ID;
		// The top most entity of the hierarchy from this object
		const size_t& RID = INVALID_ID;

		// Human-readable entity name
		Name Name = DEFAULT_NAME;

		KEntity(const std::string& Name) : Name(Name), EID(EID_), PID(PID_), RID(RID_) {}

		bool IsPendingDeath() const { return PendingDeath_; }
		void Kill() {
			PendingDeath_ = true;
		}
	protected:
		KEngine* Engine_;
		// The entities id
		size_t EID_ = INVALID_ID;
		// The entities parent id
		size_t PID_ = INVALID_ID;
		// The top most entity of the hierarchy from this object
		size_t RID_ = INVALID_ID;

		virtual void OnInit() {}
		virtual void OnStart() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		
		virtual void OnSerialize() {}
		virtual void OnDeserialize() {}
	private:
		bool PendingDeath_ = false;

		void SetEID(const size_t& EID) { EID_ = EID; }
		void SetPID(const size_t& PID) { PID_ = PID; }
		void SetRID(const size_t& RID) { RID_ = RID; }
	};
}