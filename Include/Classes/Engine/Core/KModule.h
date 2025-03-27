#pragma once

#include <string>

namespace Classes::Engine::Core {
	using MName = std::string;
	using EID = uint16_t;

	class KEntity;

	class KModule {
	public:
		MName Name;

		const EID& ParentId;

		KModule(const MName& Name, const KEntity& Entity);
		~KModule();

		KEntity& GetParent();

		bool IsPendingDeath() const { return PendingDeath_; }
	protected:
		virtual void OnInit() {}
		virtual void OnStart() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}
	private:
		EID ParentId_;
		bool PendingDeath_;
	};
}