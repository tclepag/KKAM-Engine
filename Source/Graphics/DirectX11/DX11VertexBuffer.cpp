#include "Graphics/DirectX11/DX11VertexBuffer.h"

namespace Graphics {
	DX11VertexBuffer::DX11VertexBuffer(ID3D11Device* Device)
		: Device_(Device) {
		Device_->GetImmediateContext(DeviceContext_.GetAddressOf());
	}
	DX11VertexBuffer::~DX11VertexBuffer() {
		Release();
	}
	void DX11VertexBuffer::Create() {
		if (Vertices_.empty()) return;
		if (VertexBuffer_) {
			VertexBuffer_.Reset();
		}
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(Vertex) * Vertices_.size();
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = Vertices_.data();
		HRESULT hr = Device_->CreateBuffer(&bufferDesc, &initData, VertexBuffer_.GetAddressOf());
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create vertex buffer");
		}
	}
	void DX11VertexBuffer::Bind(ID3D11DeviceContext* Context) {
		ID3D11DeviceContext* contextToUse = Context ? Context : DeviceContext_.Get();
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		contextToUse->IASetVertexBuffers(0, 1, VertexBuffer_.GetAddressOf(), &stride, &offset);
	}
	void DX11VertexBuffer::Unbind(ID3D11DeviceContext* Context) {
		ID3D11DeviceContext* contextToUse = Context ? Context : DeviceContext_.Get();
		ID3D11Buffer* nullBuffer = nullptr;
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		contextToUse->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
	}
	void DX11VertexBuffer::SetData(VertexArray Vertices) {
		Vertices_ = std::move(Vertices);
	}
	void DX11VertexBuffer::Release() {
		VertexBuffer_.Reset();
		DeviceContext_.Reset();
	}
}