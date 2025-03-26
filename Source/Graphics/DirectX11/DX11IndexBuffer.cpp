#include "Graphics/DirectX11/DX11IndexBuffer.h"

namespace Graphics {
	DX11IndexBuffer::DX11IndexBuffer(ID3D11Device* Device)
		: Device_(Device) {
		Device_->GetImmediateContext(DeviceContext_.GetAddressOf());
	}
	DX11IndexBuffer::~DX11IndexBuffer() {
		Release();
	}
	void DX11IndexBuffer::Create() {
		if (Indices_.empty()) return;
		if (IndexBuffer_) {
			IndexBuffer_.Reset();
		}
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(unsigned int) * Indices_.size();
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = Indices_.data();
		HRESULT hr = Device_->CreateBuffer(&bufferDesc, &initData, IndexBuffer_.GetAddressOf());
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create index buffer");
		}
	}
	void DX11IndexBuffer::Bind(ID3D11DeviceContext* Context) {
		ID3D11DeviceContext* contextToUse = Context ? Context : DeviceContext_.Get();
		contextToUse->IASetIndexBuffer(IndexBuffer_.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
	void DX11IndexBuffer::Unbind(ID3D11DeviceContext* Context) {
		ID3D11DeviceContext* contextToUse = Context ? Context : DeviceContext_.Get();
		ID3D11Buffer* nullBuffer = nullptr;
		contextToUse->IASetIndexBuffer(nullBuffer, DXGI_FORMAT_UNKNOWN, 0);
	}
	void DX11IndexBuffer::SetData(Indices Indices) {
		Indices_ = std::move(Indices);
	}
	void DX11IndexBuffer::Release() {
		IndexBuffer_.Reset();
		DeviceContext_.Reset();
	}
}