#include "Graphics/DirectX11/DX11Shader.h"

namespace KKAM::Graphics {
	DX11Shader::DX11Shader(ID3D11Device* device)
		: Device_(device) {
		Device_->GetImmediateContext(DeviceContext_.GetAddressOf());
	}
	DX11Shader::~DX11Shader() {
		Release();
	}
	void DX11Shader::Create() {
		if (VertexPath_.empty() || FragmentPath_.empty()) return;
		// Load and compile vertex shader
		ComPtr<ID3DBlob> vertexShaderBlob;
		HRESULT hr = D3DCompileFromFile(
			VertexPath_.c_str(),
			nullptr,
			nullptr,
			"main",
			"vs_5_0",
			0,
			0,
			&vertexShaderBlob,
			nullptr
		);
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to compile vertex shader");
		}
		// Create vertex shader
		hr = Device_->CreateVertexShader(
			vertexShaderBlob->GetBufferPointer(),
			vertexShaderBlob->GetBufferSize(),
			nullptr,
			VertexShader_.GetAddressOf()
		);
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create vertex shader");
		}
		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);
		// Create the input layout
		hr = Device_->CreateInputLayout(
			layout,
			numElements,
			vertexShaderBlob->GetBufferPointer(),
			vertexShaderBlob->GetBufferSize(),
			InputLayout_.GetAddressOf()
		);
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create input layout");
		}
		// Load and compile pixel shader
		ComPtr<ID3DBlob> pixelShaderBlob;
		hr = D3DCompileFromFile(
			FragmentPath_.c_str(),
			nullptr,
			nullptr,
			"main",
			"ps_5_0",
			0,
			0,
			&pixelShaderBlob,
			nullptr
		);
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to compile pixel shader");
		}
		// Create pixel shader
		hr = Device_->CreatePixelShader(
			pixelShaderBlob->GetBufferPointer(),
			pixelShaderBlob->GetBufferSize(),
			nullptr,
			PixelShader_.GetAddressOf()
		);
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create pixel shader");
		}
	}

	void DX11Shader::Bind(ID3D11DeviceContext* context) {
		context->IASetInputLayout(InputLayout_.Get());
		context->VSSetShader(VertexShader_.Get(), nullptr, 0);
		context->PSSetShader(PixelShader_.Get(), nullptr, 0);
	}

	void DX11Shader::Unbind(ID3D11DeviceContext* context) {
		context->VSSetShader(nullptr, nullptr, 0);
		context->PSSetShader(nullptr, nullptr, 0);
	}

	void DX11Shader::SetVertexPath(const std::wstring& path) {
		VertexPath_ = path;
	}

	void DX11Shader::SetFragmentPath(const std::wstring& path) {
		FragmentPath_ = path;
	}

	void DX11Shader::Release() {
		VertexShader_.Reset();
		PixelShader_.Reset();
		InputLayout_.Reset();
	}
}