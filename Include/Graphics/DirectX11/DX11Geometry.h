#pragma once

#include "Common/Common.h"

#include "Graphics/IGeometry.h"
#include "Graphics/DirectX11/DX11VertexBuffer.h"
#include "Graphics/DirectX11/DX11IndexBuffer.h"
#include "Graphics/DirectX11/DX11Shader.h"

namespace Graphics {
	class DX11Geometry : public IGeometry {
	public:
		DX11Geometry(Engine* Engine, ID3D11Device* Device);
		~DX11Geometry();
		void Create() override;
		void Rebake() override;
		void Draw() override;
		void Release() override;
		void SetData(VertexArray Vertices, Indices Indices) override;
		void SetVertexPath(const std::string& path) override;
		void SetFragmentPath(const std::string& path) override;
		void SetTransformMatrices(const glm::mat4& world,
			const glm::mat4& view,
			const glm::mat4& projection) override;
		void SetWorldMatrix(const glm::mat4& world) override;
		void SetViewMatrix(const glm::mat4& view) override;
		void SetProjectionMatrix(const glm::mat4& projection) override;

		// Getters

		ComPtr<ID3D11Buffer> GetVertexBuffer() const { return VertexBuffer_->GetBuffer(); }
		ComPtr<ID3D11Buffer> GetIndexBuffer() const { return IndexBuffer_->GetBuffer(); }
	private:
		ID3D11Device* Device_;
		ComPtr<ID3D11DeviceContext> DeviceContext_;
		std::unique_ptr<DX11VertexBuffer> VertexBuffer_;
		std::unique_ptr<DX11IndexBuffer> IndexBuffer_;
		std::unique_ptr<DX11Shader> Shader_;
	};
}