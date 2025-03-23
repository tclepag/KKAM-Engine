#include "Graphics/DirectX11/DX11Geometry.h"

namespace KKAM::Graphics {
	DX11Geometry::DX11Geometry(ID3D11Device* Device)
		: Device_(Device) {
		VertexBuffer_ = std::make_unique<DX11VertexBuffer>(Device_);
		IndexBuffer_ = std::make_unique<DX11IndexBuffer>(Device_);
		Shader_ = std::make_unique<DX11Shader>(Device_);
		Device_->GetImmediateContext(DeviceContext_.GetAddressOf());
	}
	DX11Geometry::~DX11Geometry() {
		Release();
	}
	void DX11Geometry::Create() {
		Shader_->Create();
		VertexBuffer_->Create();
		IndexBuffer_->Create();
	}
	void DX11Geometry::Rebake() {
		Create();
	}
	void DX11Geometry::Draw() {
		if (Shader_) {
			Shader_->SetTransformationMatrices(WorldMatrix_, ViewMatrix_, ProjectionMatrix_);
			Shader_->Bind(DeviceContext_.Get());
		}
		VertexBuffer_->Bind(DeviceContext_.Get());
		IndexBuffer_->Bind(DeviceContext_.Get());
		DeviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DeviceContext_->DrawIndexed(IndexBuffer_->GetIndexCount(), 0, 0);
		IndexBuffer_->Unbind(DeviceContext_.Get());
		VertexBuffer_->Unbind(DeviceContext_.Get());
	}
	void DX11Geometry::Release() {
		VertexBuffer_->Release();
		IndexBuffer_->Release();
	}
	void DX11Geometry::SetData(VertexArray Vertices, Indices Indices) {
		VertexBuffer_->SetData(Vertices);
		IndexBuffer_->SetData(Indices);
	}

	void DX11Geometry::SetVertexPath(const std::wstring& path) {
		VertexPath_ = path;
		if (Shader_) {
			Shader_->SetVertexPath(path);
		}
	}

	void DX11Geometry::SetFragmentPath(const std::wstring& path) {
		FragmentPath_ = path;
		if (Shader_) {
			Shader_->SetFragmentPath(path);
		}
	}

	void DX11Geometry::SetTransformMatrices(const DirectX::XMMATRIX& world,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection) {
		WorldMatrix_ = world;
		ViewMatrix_ = view;
		ProjectionMatrix_ = projection;
	}
}