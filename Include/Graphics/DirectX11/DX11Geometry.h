#pragma once

#include "Common/Common.h"

#include "Graphics/IGeometry.h"
#include "Graphics/DirectX11/DX11VertexBuffer.h"
#include "Graphics/DirectX11/DX11IndexBuffer.h"
#include "Graphics/DirectX11/DX11Shader.h"

namespace KKAM::Graphics {
	class DX11Geometry : public IGeometry {
	public:
		DX11Geometry(ID3D11Device* Device);
		~DX11Geometry();
		void Create() override;
		void Rebake() override;
		void Draw() override;
		void Release() override;
		void SetData(VertexArray Vertices, Indices Indices) override;
		void SetVertexPath(const std::wstring& path) override;
		void SetFragmentPath(const std::wstring& path) override;
		void SetTransformMatrices(const DirectX::XMMATRIX& world,
			const DirectX::XMMATRIX& view,
			const DirectX::XMMATRIX& projection) override;

	private:
		ID3D11Device* Device_;
		ComPtr<ID3D11DeviceContext> DeviceContext_;
		std::unique_ptr<DX11VertexBuffer> VertexBuffer_;
		std::unique_ptr<DX11IndexBuffer> IndexBuffer_;
		std::unique_ptr<DX11Shader> Shader_;
	};
}