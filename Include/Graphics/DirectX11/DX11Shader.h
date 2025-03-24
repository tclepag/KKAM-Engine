#pragma once

#include "Graphics/IShader.h"

namespace KKAM::Graphics {

	struct TransformBufferType {
		glm::mat4 World;
		glm::mat4 View;
		glm::mat4 Projection;
	};

	class DX11Shader : public IShader<ID3D11DeviceContext*> {
	public:
		DX11Shader(Engine* Engine, ID3D11Device* device);
		~DX11Shader();
		void Create() override;
		void SetTransformationMatrices(const glm::mat4& world,
			const glm::mat4& view,
			const glm::mat4& projection) override;
		void Bind(ID3D11DeviceContext*) override;
		void Unbind(ID3D11DeviceContext*) override;
		void SetVertexPath(const std::string& path) override;
		void SetFragmentPath(const std::string& path) override;
		void Release() override;
	private:
		ID3D11Device* Device_;
		ComPtr<ID3D11DeviceContext> DeviceContext_;
		ComPtr<ID3D11VertexShader> VertexShader_;
		ComPtr<ID3D11PixelShader> PixelShader_;
		ComPtr<ID3D11InputLayout> InputLayout_;
		ComPtr<ID3D11Buffer> TransformBuffer_;
	};
}