#pragma once

#include "Graphics/IIndexBuffer.h"

namespace Graphics {
	class DX11IndexBuffer : public IIndexBuffer<ID3D11DeviceContext*> {
	public:
		DX11IndexBuffer(ID3D11Device* Device);
		~DX11IndexBuffer();
		void Create() override;
		void Bind(ID3D11DeviceContext* Context) override;
		void Unbind(ID3D11DeviceContext* Context) override;
		void SetData(Indices Indices) override;
		void Release() override;

		// Getters

		unsigned int GetIndexCount() const { return static_cast<unsigned int>(Indices_.size()); }
		ComPtr<ID3D11Buffer> GetBuffer() const { return IndexBuffer_; }
		Indices GetIndices() const { return Indices_; }
	private:
		ID3D11Device* Device_;
		ComPtr<ID3D11DeviceContext> DeviceContext_;
		ComPtr<ID3D11Buffer> IndexBuffer_;
		Indices Indices_;
	};
}