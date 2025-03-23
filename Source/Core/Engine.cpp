#include "Core/Engine.h"

namespace KKAM::Core {
	void Engine::Initialize() {
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

		// Hooks
		AppWindow_->HookProc("WindowClose", "BaseWindowClose", [this](UINT Msg, WPARAM wParam, LPARAM lParam) {
			IsRunning_ = false;
			}
		);

		AppWindow_->HookProc("WindowResize", "RedrawGraphics", [this](UINT Msg, WPARAM wParam, LPARAM lParam) {
			Graphics_->HandleResize();
			}
		);

		// Create test geometry

		VertexArray triangle = {
			{0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f},
			{0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
			{-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f}
		};

		Indices indices = { 0, 1, 2 };

		Geometry_ = new DX11Geometry(Graphics_->GetDevice().Get());
		Geometry_->SetData(triangle, indices);
		Geometry_->SetVertexPath(L"Content/Shaders/Basic.hlsl");
		Geometry_->SetFragmentPath(L"Content/Shaders/BasicPixel.hlsl");
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
		Graphics_->Render([this]() {
			float aspectRatio = static_cast<float>(AppWindow_->GetWidth()) / static_cast<float>(AppWindow_->GetHeight());
			DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(
				DirectX::XM_PIDIV4,  // 45 degrees field of view
				aspectRatio,
				0.1f,                // Near plane
				1000.0f              // Far plane
			);

			// Create hardcoded view matrix (camera positioned at (0, 0, -5) looking at origin)
			DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);
			DirectX::XMVECTOR lookAtPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eyePosition, lookAtPosition, upDirection);

			// Create model/world matrix (identity for now, or add transformations)
			DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
			// If you want to translate/rotate/scale your model:
			// DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
			// DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixRotationY(angle);

			Geometry_->SetTransformMatrices(worldMatrix, viewMatrix, projMatrix);
			Geometry_->Draw();
			});
	}
} // namespace KKAM::Core