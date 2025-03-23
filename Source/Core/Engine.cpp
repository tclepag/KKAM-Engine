#include "Core/Engine.h"

namespace KKAM::Core {
	void Engine::Initialize() {
		AppWindowSettings settings{};
		settings.Width = 1280;
		settings.Height = 720;
		settings.Title = L"KKAM Engine";

		AppWindow_ = std::make_shared<AppWindow>();
		AppWindow_->Initialize(settings);

		// Setup window hooks

		AppWindow_->RegisterProc(WM_CLOSE, "WindowClose");
		AppWindow_->HookProc("WindowClose", "BaseWindowClose", [this](UINT Msg, WPARAM wParam, LPARAM lParam) {
			IsRunning_ = false;
			}
		);
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
		// Rendering logic goes here
	}
}