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

		D3D11_BUFFER_DESC transformBufferDesc = {};
		transformBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		transformBufferDesc.ByteWidth = sizeof(TransformBufferType); // Define this struct
		transformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		transformBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		transformBufferDesc.MiscFlags = 0;
		transformBufferDesc.StructureByteStride = 0;

		hr = Device_->CreateBuffer(&transformBufferDesc, nullptr, TransformBuffer_.GetAddressOf());
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create transform constant buffer");
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

	void DX11Shader::SetTransformationMatrices(const DirectX::XMMATRIX& world,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection) {
		// Map the constant buffer
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = DeviceContext_->Map(TransformBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to map transformation constant buffer");
		}

		// Copy the matrices to the constant buffer
		TransformBufferType* transformData = (TransformBufferType*)mappedResource.pData;
		transformData->World = DirectX::XMMatrixTranspose(world);  // DirectX expects matrices transposed for HLSL
		transformData->View = DirectX::XMMatrixTranspose(view);
		transformData->Projection = DirectX::XMMatrixTranspose(projection);

		// Unmap the constant buffer
		DeviceContext_->Unmap(TransformBuffer_.Get(), 0);

		// Set the constant buffer to the vertex shader
		DeviceContext_->VSSetConstantBuffers(0, 1, TransformBuffer_.GetAddressOf());
	}

	void DX11Shader::Bind(ID3D11DeviceContext* context) {
		context->IASetInputLayout(InputLayout_.Get());
		context->VSSetShader(VertexShader_.Get(), nullptr, 0);
		context->PSSetShader(PixelShader_.Get(), nullptr, 0);
		context->VSSetConstantBuffers(0, 1, TransformBuffer_.GetAddressOf());
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