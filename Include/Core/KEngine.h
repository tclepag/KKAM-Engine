#pragma once

#include <memory>
#include <iostream>
#include "Core/KWindow.h"
#include "Graphics/IGraphics.h"

#include "KRegistry.h"

#include "Graphics/DirectX11/DX11Graphics.h"
#include "Graphics/DirectX11/DX11Geometry.h"

namespace Core {
	using DX11 = Graphics::DX11Graphics;
	using DX11Geometry = Graphics::DX11Geometry;
	class KEngine {
	public:
		/// <summary>
		/// Sets up the KEngine
		/// </summary>
		void Initialize();
		/// <summary>
		/// Begins the KEngine loop
		/// </summary>
		void Run();
		/// <summary>
		/// Frees all KEngine resources and closes application
		/// </summary>
		void Shutdown();
		/// <summary>
		/// Handles KEngine update loop
		/// </summary>
		void Update();
		/// <summary>
		/// Handles KEngine render loop
		/// </summary>
		void Render();

		// Getters

		bool IsRunning() const { return IsRunning_; }
		std::shared_ptr<KWindow> GetAppWindow() const { return AppWindow_; }
		std::shared_ptr<DX11> GetGraphics() const { return Graphics_; }
		std::shared_ptr<KRegistry> GetWorld() const { return Registry_; }
		float GetDeltaTime() const { return DeltaTime_; }
	private:
		std::shared_ptr<KRegistry> World_;
		std::shared_ptr<KWindow> AppWindow_;
		std::shared_ptr<DX11> Graphics_;
		bool IsRunning_ = false;
		float RotAngle_ = 0.0f;
		float DeltaTime_ = 0.0f;
		std::chrono::steady_clock::time_point LastTime_ = std::chrono::steady_clock::now();
		DX11Geometry* Geometry_;
		void DrawGeometry();
	};
}