#pragma once

namespace KKAM::Core {
	class Engine {
	public:
		void Initialize();
		void Run();
		void Shutdown();
	private:
		bool IsRunning_;

	};
}