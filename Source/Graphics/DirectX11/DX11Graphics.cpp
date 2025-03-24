#include "Graphics/DirectX11/DX11Graphics.h"
#include "Core/Engine.h"

namespace KKAM::Graphics {
	void DX11Graphics::Initialize() {
		CreateDevice();          // First, create the device
		CreateViewport();        // Second, set up viewport dimensions
		CreateSwapChain();       // Now create swap chain with those dimensions
		CreateRenderTargetView(); // Create render target view from swap chain
		CreateDepthStencilView(); // Create depth stencil view with same dimensions
		CreateRasterizerState();  // Set up pipeline states
		CreateBlendState();
		CreateDepthStencilState();
		CreatePresentParams();
		InitializeImGui();       // Finally, initialize ImGui
	}

	void DX11Graphics::InitializeImGui() {
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Engine_->GetAppWindow()->GetHWND());
		ImGui_ImplDX11_Init(Device_.Get(), DeviceContext_.Get());

		ImGuiIO& io = ImGui::GetIO();

		// Disable ImGui's automatic DPI scaling
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		io.ConfigWindowsResizeFromEdges = false;

		// Force pixel-perfect scaling
		io.FontGlobalScale = 1.0f;

	}

	void DX11Graphics::Redraw() {
		Render(nullptr);
	}
	void DX11Graphics::HandleResize() {
		DeviceContext_->OMSetRenderTargets(0, nullptr, nullptr);
		RenderTargetView_.Reset();
		DepthStencilView_.Reset();

		DeviceContext_->Flush();

		// Resize the swap chain
		HRESULT hr = SwapChain_->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		if (FAILED(hr)) {
			// Handle error
			return;
		}

		// Recreate render target view
		CreateRenderTargetView();

		// Update viewport dimensions
		CreateViewport();

		// Recreate depth stencil view
		CreateDepthStencilView();

		DeviceContext_->OMSetRenderTargets(1, RenderTargetView_.GetAddressOf(), DepthStencilView_.Get());
		DeviceContext_->RSSetViewports(1, &Viewport_);

		Render(LastRenderOperation_);
	}
	void DX11Graphics::Render(std::function<void()> RenderOperation) {
		if (!RenderActive_) {
			// Skip rendering if not active
			return;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (RenderOperation == nullptr) {
			RenderOperation = LastRenderOperation_;
		}
		// Clear the render target and depth stencil
		float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f }; // Dark blue
		DeviceContext_->ClearRenderTargetView(RenderTargetView_.Get(), clearColor);
		DeviceContext_->ClearDepthStencilView(DepthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// Set render targets and viewport
		DeviceContext_->OMSetRenderTargets(1, RenderTargetView_.GetAddressOf(), DepthStencilView_.Get());
		DeviceContext_->RSSetViewports(1, &Viewport_);

		// Set states
		DeviceContext_->OMSetDepthStencilState(DepthStencilState_.Get(), 1);
		DeviceContext_->RSSetState(RasterizerState_.Get());
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		DeviceContext_->OMSetBlendState(BlendState_.Get(), blendFactor, 0xffffffff);

		// Call the render operation
		if (RenderOperation) {
			RenderOperation();
		}

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(static_cast<float>(Engine_->GetAppWindow()->GetWidth()), static_cast<float>(Engine_->GetAppWindow()->GetHeight()));

		// Render ImGui
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		SwapChain_->Present(1, 0);
		LastRenderOperation_ = RenderOperation;
	}
	void DX11Graphics::Shutdown() {
		if (SwapChain_) {
			SwapChain_->SetFullscreenState(FALSE, nullptr);
			SwapChain_.Reset();
		}
		if (RenderTargetView_) {
			RenderTargetView_.Reset();
		}
		if (DepthStencilView_) {
			DepthStencilView_.Reset();
		}
		if (DeviceContext_) {
			DeviceContext_.Reset();
		}
		if (Device_) {
			Device_.Reset();
		}
		if (DebugDevice_) {
			DebugDevice_.Reset();
		}
	}
	void DX11Graphics::CreateRasterizerState() {
		D3D11_RASTERIZER_DESC rastDesc = {};
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.CullMode = D3D11_CULL_BACK;
		rastDesc.FrontCounterClockwise = FALSE;
		rastDesc.DepthBias = 0;
		rastDesc.DepthBiasClamp = 0.0f;
		rastDesc.SlopeScaledDepthBias = 0.0f;
		rastDesc.DepthClipEnable = TRUE;
		rastDesc.ScissorEnable = FALSE;
		rastDesc.MultisampleEnable = FALSE;
		rastDesc.AntialiasedLineEnable = FALSE;

		HRESULT hr = Device_->CreateRasterizerState(&rastDesc, RasterizerState_.GetAddressOf());
		if (FAILED(hr)) {
			// Handle error
			return;
		}
	}
	void DX11Graphics::CreateBlendState() {
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = Device_->CreateBlendState(&blendDesc, BlendState_.GetAddressOf());
		if (FAILED(hr)) {
			// Handle error
			return;
		}
	}
	void DX11Graphics::CreateDepthStencilState() {
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = TRUE;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Front-facing pixels
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Back-facing pixels
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = Device_->CreateDepthStencilState(&depthStencilDesc, DepthStencilState_.GetAddressOf());
		if (FAILED(hr)) {
			// Handle error
			return;
		}
	}
	void DX11Graphics::CreateSwapChain() {
		// Get the DXGI factory from the device
		ComPtr<IDXGIDevice> dxgiDevice;
		HRESULT hr = Device_.As(&dxgiDevice);
		if (FAILED(hr)) {
			// Handle error
			return;
		}

		ComPtr<IDXGIAdapter> dxgiAdapter;
		hr = dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
		if (FAILED(hr)) {
			// Handle error
			return;
		}

		ComPtr<IDXGIFactory> dxgiFactory;
		hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), &dxgiFactory);
		if (FAILED(hr)) {
			// Handle error
			return;
		}

		// Fill out swap chain description
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.BufferCount = 2;
		sd.BufferDesc.Width = Viewport_.Width;  // Should be window width
		sd.BufferDesc.Height = Viewport_.Height; // Should be window height
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = Engine_->GetAppWindow()->GetHWND();  // You need to provide this
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(Device_.Get(), &sd, SwapChain_.GetAddressOf());
		if (FAILED(hr)) {
			// Handle error
			return;
		}
	}
	void DX11Graphics::CreateDevice() {
		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		D3D_FEATURE_LEVEL featureLevel;

		HRESULT hr = D3D11CreateDevice(
			nullptr,                    // Default adapter
			D3D_DRIVER_TYPE_HARDWARE,   // Hardware device
			nullptr,                    // Not using software rasterizer
			createDeviceFlags,          // Flags
			featureLevels,              // Feature levels
			numFeatureLevels,           // Number of feature levels
			D3D11_SDK_VERSION,          // SDK version
			Device_.GetAddressOf(),     // Output device
			&featureLevel,              // Output feature level
			DeviceContext_.GetAddressOf() // Output context
		);

		if (FAILED(hr)) {
			// Fall back to WARP device if hardware fails
			hr = D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_WARP,
				nullptr,
				createDeviceFlags,
				featureLevels,
				numFeatureLevels,
				D3D11_SDK_VERSION,
				Device_.GetAddressOf(),
				&featureLevel,
				DeviceContext_.GetAddressOf()
			);

			if (FAILED(hr)) {
				// Handle error
				return;
			}
		}

		// Create debug device if in debug mode
#ifdef _DEBUG
		CreateDebugDevice();
#endif
	}
	void DX11Graphics::CreateDebugDevice() {
#ifdef _DEBUG
		HRESULT hr = Device_.As(&DebugDevice_);
		if (FAILED(hr)) {
			// Handle error - debug interface isn't available
			return;
		}
#endif
	}
	void DX11Graphics::CreateViewport() {
		auto window = Engine_->GetAppWindow();

		UINT width = static_cast<UINT>(window->GetWidth());
		UINT height = static_cast<UINT>(window->GetHeight());

		// Setup viewport - you should get these dimensions from your window
		Viewport_.Width = width;
		Viewport_.Height = height;
		Viewport_.MinDepth = 0.0f;
		Viewport_.MaxDepth = 1.0f;
		Viewport_.TopLeftX = 0;
		Viewport_.TopLeftY = 0;
	}
	void DX11Graphics::CreatePresentParams() {
		PresentParams_ = {};
	}
	void DX11Graphics::CreateRenderTargetView() {
		if (!Device_ || !SwapChain_)
			return;

		// Get the back buffer texture
		ComPtr<ID3D11Texture2D> backBuffer;
		HRESULT hr = SwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
		if (FAILED(hr)) {
			// Handle error
			return;
		}

		// Create render target view
		hr = Device_->CreateRenderTargetView(backBuffer.Get(), nullptr, RenderTargetView_.GetAddressOf());
		if (FAILED(hr)) {
			// Handle error
			return;
		}
	}
	void DX11Graphics::CreateDepthStencilView() {
		if (!Device_)
			return;

		// Get the back buffer texture
		ComPtr<ID3D11Texture2D> depthStencilTexture;

		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = Viewport_.Width;
		descDepth.Height = Viewport_.Height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		HRESULT hr = Device_->CreateTexture2D(&descDepth, nullptr, depthStencilTexture.GetAddressOf());
		if (FAILED(hr)) {
			// Handle error
			return;
		}

		// Create depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		hr = Device_->CreateDepthStencilView(depthStencilTexture.Get(), &descDSV, DepthStencilView_.GetAddressOf());
		if (FAILED(hr)) {
			// Handle error
			return;
		}
	}
}