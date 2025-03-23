#pragma once

#include <memory>
#include "Core/AppWindow.h"

namespace KKAM::Core {
	class Engine {
	public:
		/// <summary>
		/// Sets up the engine
		/// </summary>
		void Initialize();
		/// <summary>
		/// Begins the engine loop
		/// </summary>
		void Run();
		/// <summary>
		/// Frees all engine resources and closes application
		/// </summary>
		void Shutdown();
		/// <summary>
		/// Handles engine update loop
		/// </summary>
		void Update();
		/// <summary>
		/// Handles engine render loop
		/// </summary>
		void Render();
	private:
		std::shared_ptr<AppWindow> AppWindow_;

		bool IsRunning_;
	};
}