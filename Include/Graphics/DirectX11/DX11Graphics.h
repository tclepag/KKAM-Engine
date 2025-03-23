#pragma once

#include "Graphics/IGraphics.h"

namespace KKAM::Core {
	class Engine;
}

namespace KKAM::Graphics {
	class DX11Graphics : public IGraphics {
	public:
		DX11Graphics(Engine* engine) : IGraphics(engine) {}
		void Initialize() override;
		void HandleResize() override;
		void Redraw() override;
		void Render(std::function<void()> RenderOperation) override;
		void Shutdown() override;

		// Getters

		ComPtr<ID3D11Device> GetDevice() const { return Device_; }
		ComPtr<ID3D11DeviceContext> GetDeviceContext() const { return DeviceContext_; }
		ComPtr<IDXGISwapChain> GetSwapChain() const { return SwapChain_; }
		ComPtr<ID3D11RenderTargetView> GetRenderTargetView() const { return RenderTargetView_; }
		ComPtr<ID3D11DepthStencilView> GetDepthStencilView() const { return DepthStencilView_; }
		ComPtr<ID3D11DepthStencilState> GetDepthStencilState() const { return DepthStencilState_; }
		ComPtr<ID3D11RasterizerState> GetRasterizerState() const { return RasterizerState_; }
		ComPtr<ID3D11BlendState> GetBlendState() const { return BlendState_; }
		DXGI_PRESENT_PARAMETERS GetPresentParams() const { return PresentParams_; }
		D3D11_VIEWPORT GetViewport() const { return Viewport_; }
	private:
		ComPtr<IDXGISwapChain> SwapChain_;
		ComPtr<ID3D11Device> Device_;
		ComPtr<ID3D11DeviceContext> DeviceContext_;
		ComPtr<ID3D11RenderTargetView> RenderTargetView_;
		ComPtr<ID3D11DepthStencilView> DepthStencilView_;
		ComPtr<ID3D11DepthStencilState> DepthStencilState_;
		ComPtr<ID3D11RasterizerState> RasterizerState_;
		ComPtr<ID3D11BlendState> BlendState_;
		ComPtr<ID3D11Debug> DebugDevice_;
		DXGI_PRESENT_PARAMETERS PresentParams_;
		D3D11_VIEWPORT Viewport_;

		void CreateRasterizerState();
		void CreateBlendState();
		void CreateDepthStencilState();
		void CreateSwapChain();
		void CreateDevice();
		void CreateDebugDevice();
		void CreateViewport();
		void CreatePresentParams();
		void CreateRenderTargetView();
		void CreateDepthStencilView();
	};
} // namespace KKAM::Graphics