#pragma once

#include "Graphics/IShader.h"

namespace KKAM::Graphics {
	class DX11Shader : public IShader<ID3D11DeviceContext*> {
	public:
		DX11Shader(ID3D11Device* device);
		~DX11Shader();
		void Create() override;
		void Bind(ID3D11DeviceContext*) override;
		void Unbind(ID3D11DeviceContext*) override;
		void SetVertexPath(const std::wstring& path) override;
		void SetFragmentPath(const std::wstring& path) override;
		void Release() override;
	private:
		ID3D11Device* Device_;
		ComPtr<ID3D11DeviceContext> DeviceContext_;
		ComPtr<ID3D11VertexShader> VertexShader_;
		ComPtr<ID3D11PixelShader> PixelShader_;
		ComPtr<ID3D11InputLayout> InputLayout_;
	};
}