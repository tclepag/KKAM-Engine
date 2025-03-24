#include "Graphics/DirectX11/DX11Shader.h"
#include "Core/Engine.h"

namespace KKAM::Graphics {
	DX11Shader::DX11Shader(Engine* Engine, ID3D11Device* device)
		: Device_(device), IShader(Engine) {
		Device_->GetImmediateContext(DeviceContext_.GetAddressOf());
	}
	DX11Shader::~DX11Shader() {
		Release();
	}
	void DX11Shader::Create() {
		if (VertexPath_.empty() || FragmentPath_.empty()) return;
		// Load and compile vertex shader
		ComPtr<ID3DBlob> vertexShaderBlob;
		auto vertexHLSLPath = VertexPath_ + ".hlsl";
		auto vertexCSOPath = VertexPath_ + ".cso";
		HRESULT hr;
		if (std::filesystem::exists(vertexCSOPath)) {
			hr = D3DReadFileToBlob(StringToWString(vertexCSOPath).c_str(), vertexShaderBlob.GetAddressOf());
			if (FAILED(hr)) {
				throw std::runtime_error("Failed to read vertex shader from file");
			}
			OutputDebugStringA("Vertex shader loaded successfully\n");
			Engine_->GetConsole().LogInfo("Vertex shader loaded successfully");
		}
		else {
			hr = D3DCompileFromFile(
				StringToWString(VertexPath_ + ".hlsl").c_str(),
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
			OutputDebugStringA("Vertex shader compiled successfully\n");
			Engine_->GetConsole().LogInfo("Vertex shader compiled successfully");
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
		auto pixelHLSLPath = FragmentPath_ + ".hlsl";
		auto pixelCSOPath = FragmentPath_ + ".cso";
		if (std::filesystem::exists(pixelCSOPath)) {
			hr = D3DReadFileToBlob(StringToWString(pixelCSOPath).c_str(), pixelShaderBlob.GetAddressOf());
			if (FAILED(hr)) {
				throw std::runtime_error("Failed to read pixel shader from file");
			}

			OutputDebugStringA("Pixel shader loaded successfully\n");
			Engine_->GetConsole().LogInfo("Pixel shader loaded successfully");
		}
		else {
			hr = D3DCompileFromFile(
				StringToWString(FragmentPath_ + ".hlsl").c_str(),
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
			OutputDebugStringA("Pixel shader compiled successfully\n");
			Engine_->GetConsole().LogInfo("Pixel shader compiled successfully");
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

	void DX11Shader::Release() {
		VertexShader_.Reset();
		PixelShader_.Reset();
		InputLayout_.Reset();
	}
}