#include "Core/Engine.h"

namespace KKAM::Core {
	void Engine::Initialize() {
		AppWindowSettings settings{};
		settings.Width = 1280;
		settings.Height = 720;
		settings.Title = "KKAM Engine";

		AppWindow_ = std::make_shared<AppWindow>();
		AppWindow_->Initialize(settings);
	}
	void Engine::Run() {
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
		AppWindow_->Process();
	}
	void Engine::Render() {
		// Rendering logic goes here
	}
}