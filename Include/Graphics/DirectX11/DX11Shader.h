#pragma once

#include "Graphics/IShader.h"
#include "Graphics/ShaderBlob.h"

namespace Graphics {

	struct TransformBufferType {
		glm::mat4 World;
		glm::mat4 View;
		glm::mat4 Projection;
	};

	class DX11Shader : public IShader<ID3D11DeviceContext*> {
	public:
		DX11Shader(KEngine* KEngine, ID3D11Device* device);
		~DX11Shader();
		void Create() override;
		void SetTransformationMatrices(const glm::mat4& world,
			const glm::mat4& view,
			const glm::mat4& projection) override;
		void Bind(ID3D11DeviceContext*) override;
		void Unbind(ID3D11DeviceContext*) override;
		void SetVertexPath(const std::string& path) override;
		void SetFragmentPath(const std::string& path) override;
		void SetInputElement(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout);
		void SetInputBufferDesc(D3D11_BUFFER_DESC desc);
		void Release() override;
	private:
		ID3D11Device* Device_;
		ComPtr<ID3D11DeviceContext> DeviceContext_;
		ComPtr<ID3D11VertexShader> VertexShader_;
		ComPtr<ID3D11PixelShader> PixelShader_;
		ComPtr<ID3D11InputLayout> InputLayout_;
		ComPtr<ID3D11Buffer> TransformBuffer_;
		std::vector<D3D11_INPUT_ELEMENT_DESC> InputElementDesc_ = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		D3D11_BUFFER_DESC TransformBufferDesc_{};
	};
}