#pragma once

#include <string>

namespace Classes::Engine::Core {
	using MName = std::string;
	using EID = uint16_t;

	class Ent;

	class Mod {
	public:
		MName Name;

		const EID& ParentId;

		Mod(const MName& Name, const Ent& Entity);
		~Mod();

		Ent& GetParent();

		bool IsPendingDeath() const { return PendingDeath_; }
	protected:
		virtual void OnInit();
		virtual void OnStart();
		virtual void OnUpdate();
		virtual void OnRender();
	private:
		EID ParentId_;
		bool PendingDeath_;
	};
}