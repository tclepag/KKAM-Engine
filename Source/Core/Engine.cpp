#include "Core/Engine.h"

#include <ostream>
#include <iomanip>
#include <stdio.h>

namespace KKAM::Core {
	void Engine::Initialize() {
		// Initialize ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = "Config/imgui.ini";
		io.FontGlobalScale = 1.0f;

		// Initialize the application window
		AppWindowSettings settings{};
		settings.Width = 1280;
		settings.Height = 720;
		settings.Title = L"KKAM Engine";

		AppWindow_ = std::make_shared<AppWindow>();
		AppWindow_->Initialize(settings);

		while (!AppWindow_->IsInitialized())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Wait for the window to be initialized
		}

		// Initialize graphics
		Graphics_ = std::make_shared<DX11>(this);
		Graphics_->Initialize();

		// Setup window hooks

		// Registers
		AppWindow_->RegisterProc(WM_CLOSE, "WindowClose");
		AppWindow_->RegisterProc(WM_ENTERSIZEMOVE, "RenderPause");
		AppWindow_->RegisterProc(WM_EXITSIZEMOVE, "RenderResume");
		AppWindow_->RegisterProc(WM_TIMER, "GraphicsResizeTime");
		AppWindow_->RegisterProc(WM_MOVING, "GraphicsResize");
		AppWindow_->RegisterProc(WM_SIZING, "GraphicsResize");
		AppWindow_->RegisterProc(WM_SIZE, "GraphicsResize");


		// Hooks
		AppWindow_->HookProc("WindowClose", "BaseWindowClose", [this](UINT Msg, WPARAM wParam, LPARAM lParam) {
			IsRunning_ = false;
			}
		);

		AppWindow_->HookProc("RenderPause", "BaseRenderPause", [this](UINT Msg, WPARAM wParam, LPARAM lParam) {
			SetTimer(AppWindow_->GetHWND(), 2, 45, NULL);
			}
		);

		AppWindow_->HookProc("RenderResume", "BaseRenderResume", [this](UINT Msg, WPARAM wParam, LPARAM lParam) {
			KillTimer(AppWindow_->GetHWND(), 2);
			}
		);

		AppWindow_->HookProc("GraphicsResizeTime", "RenderTimer", [this](UINT Msg, WPARAM wParam, LPARAM lParam) {
			if (wParam == 2) {
				Graphics_->HandleResize();
				Update();
				Render();
			}
			}
		);

		AppWindow_->HookProc("GraphicsResize", "WindowSizeMove", [this](UINT Msg, WPARAM wParam, LPARAM lParam) {
			Graphics_->HandleResize();
			}
		);

		// Create test geometry

		VertexArray triangle = {
			// Apex/top point (red)
			{0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f},

			// Base vertices (clockwise order from front)
			{0.0f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f},  // Front (green)
			{0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f}, // Right (blue)
			{-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f} // Left (yellow)
		};

		Indices indices = {
			// Front face (apex-front-right)
			0, 1, 2,

			// Right face (apex-right-left)
			0, 2, 3,

			// Left face (apex-left-front)
			0, 3, 1,

			// Base (front-left-right) - counter-clockwise for back face
			1, 3, 2
		};

		Geometry_ = new DX11Geometry(this, Graphics_->GetDevice().Get());
		Geometry_->SetData(triangle, indices);
		Geometry_->SetVertexPath("Content/Shaders/Default/Basic");
		Geometry_->SetFragmentPath("Content/Shaders/Default/BasicPixel");
		Geometry_->SetWorldMatrix(glm::mat4(1.0f));
		Geometry_->Create();
	}
	void Engine::Run() {
		IsRunning_ = true;
		while (IsRunning_) {
			Update();
			Render();
		}
	}
	void Engine::Shutdown() {
		if (AppWindow_) {
			AppWindow_->Shutdown();
			AppWindow_.reset();
		}

		if (Graphics_) {
			Graphics_->Shutdown();
			Graphics_.reset();
		}

		if (Geometry_) {
			Geometry_->Release();
			delete Geometry_;
			Geometry_ = nullptr;
		}
	}
	void Engine::Update() {
		WinEvProcResult result = AppWindow_->Process();

		if (result == WinEvProcResult::ERR) {
			std::cerr << "Error processing window events" << std::endl;
			return;
		}

		if (result == WinEvProcResult::SHUTDOWN) {
			IsRunning_ = false;
		}
	}
	void Engine::Render() {
		if (!Graphics_->IsRenderActive()) {
			return;
		}
		// Calculate delta time
		auto currentTime = std::chrono::steady_clock::now();
		DeltaTime_ = std::chrono::duration<float>(currentTime - LastTime_).count();
		LastTime_ = currentTime;

		Graphics_->Render([this]() {
			DrawGeometry();
			});
	}

	void Engine::DrawGeometry() {
		float aspectRatio = static_cast<float>(AppWindow_->GetWidth()) / static_cast<float>(AppWindow_->GetHeight());
		glm::mat4 projMatrix = glm::perspectiveLH(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

		// Create hardcoded view matrix (camera positioned at (0, 0, -5) looking at origin)
		glm::vec3 eyePosition = glm::vec3(0.0f, 0.0f, -2.0f);
		glm::vec3 lookAtPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 viewMatrix = glm::lookAtLH(eyePosition, lookAtPosition, upDirection);

		RotAngle_ += DeltaTime_ * 1.0f; // Rotate at 1 radian per second

		Geometry_->SetWorldMatrix(glm::rotate(glm::mat4(1.0f), RotAngle_, glm::normalize(glm::vec3(0.0f, 1.0f, 0.5f))));
		Geometry_->SetProjectionMatrix(projMatrix);
		Geometry_->SetViewMatrix(viewMatrix);

		Geometry_->Draw();

		// Position the overlay
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
		ImGui::SetNextWindowBgAlpha(0.2f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		// Create window with minimal flags
		ImGui::Begin("FPS Overlay", nullptr,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_AlwaysAutoResize);

		// Display FPS
		ImGui::Text("FPS: %.1f", 1.0f / DeltaTime_);
		ImGui::End();
		ImGui::PopStyleVar();
	}
} // namespace KKAM::Core