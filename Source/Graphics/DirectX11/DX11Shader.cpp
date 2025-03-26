#include "Graphics/DirectX11/DX11Shader.h"
#include "Core/Engine.h"

namespace KKAM::Graphics {
	DX11Shader::DX11Shader(Engine* Engine, ID3D11Device* device)
		: Device_(device), IShader(Engine) {
		Device_->GetImmediateContext(DeviceContext_.GetAddressOf());
		TransformBufferDesc_.Usage = D3D11_USAGE_DYNAMIC;
		TransformBufferDesc_.ByteWidth = sizeof(TransformBufferType);
		TransformBufferDesc_.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		TransformBufferDesc_.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		TransformBufferDesc_.MiscFlags = 0;
		TransformBufferDesc_.StructureByteStride = 0;
	}
	DX11Shader::~DX11Shader() {
		Release();
	}
	void DX11Shader::Create() {
		if (VertexPath_.empty() || FragmentPath_.empty()) return;
		// Load and compile vertex shader
		VertexBlob_ = ShaderBlob::LoadShaderBlob(VertexPath_, "main", "vs_5_0");
		HRESULT hr = Device_->CreateVertexShader(
			VertexBlob_->GetBufferPointer(),
			VertexBlob_->GetBufferSize(),
			nullptr,
			VertexShader_.GetAddressOf()
		);
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create vertex shader");
		}

		// Create the input layout
		hr = Device_->CreateInputLayout(
			InputElementDesc_.data(),
			static_cast<UINT>(InputElementDesc_.size()),
			VertexBlob_->GetBufferPointer(),
			VertexBlob_->GetBufferSize(),
			InputLayout_.GetAddressOf()
		);
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create input layout");
		}

		hr = Device_->CreateBuffer(&TransformBufferDesc_, nullptr, TransformBuffer_.GetAddressOf());
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create transform constant buffer");
		}

		// Load and compile pixel shader
		PixelBlob_ = ShaderBlob::LoadShaderBlob(FragmentPath_, "main", "ps_5_0");
		hr = Device_->CreatePixelShader(
			PixelBlob_->GetBufferPointer(),
			PixelBlob_->GetBufferSize(),
			nullptr,
			PixelShader_.GetAddressOf()
		);
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create pixel shader");
		}
	}

	void DX11Shader::SetTransformationMatrices(const glm::mat4& world,
		const glm::mat4& view,
		const glm::mat4& projection) {
		// Map the constant buffer
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = DeviceContext_->Map(TransformBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to map transformation constant buffer");
		}

		// Copy the matrices to the constant buffer
		TransformBufferType* transformData = (TransformBufferType*)mappedResource.pData;
		transformData->World = glm::transpose(world);  // DirectX expects matrices transposed for HLSL
		transformData->View = glm::transpose(view);
		transformData->Projection = glm::transpose(projection);

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

	void DX11Shader::SetVertexPath(const std::string& path) {
		VertexPath_ = path;
	}

	void DX11Shader::SetFragmentPath(const std::string& path) {
		FragmentPath_ = path;
	}

	void DX11Shader::SetInputElement(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout)
	{
		InputElementDesc_ = layout;

	}

	void DX11Shader::SetInputBufferDesc(D3D11_BUFFER_DESC desc)
	{
		TransformBufferDesc_ = desc;
	}

	void DX11Shader::Release() {
		VertexShader_.Reset();
		PixelShader_.Reset();
		InputLayout_.Reset();
	}
}