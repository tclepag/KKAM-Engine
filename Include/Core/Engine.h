#pragma once

#include <memory>
#include <iostream>
#include "Core/AppWindow.h"
#include "Graphics/IGraphics.h"

#include "Graphics/DirectX11/DX11Graphics.h"
#include "Graphics/DirectX11/DX11Geometry.h"

namespace KKAM::Core {
	using DX11 = KKAM::Graphics::DX11Graphics;
	using DX11Geometry = KKAM::Graphics::DX11Geometry;
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

		// Getters

		bool IsRunning() const { return IsRunning_; }
		std::shared_ptr<AppWindow> GetAppWindow() const { return AppWindow_; }
	private:
		std::shared_ptr<AppWindow> AppWindow_;
		std::shared_ptr<DX11> Graphics_;
		bool IsRunning_ = false;

		DX11Geometry* Geometry_;
	};
}