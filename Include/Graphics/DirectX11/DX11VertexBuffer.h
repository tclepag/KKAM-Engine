#pragma once

#include "Graphics/IVertexBuffer.h"

namespace KKAM::Graphics {
	class DX11VertexBuffer : public IVertexBuffer<ID3D11DeviceContext*> {
	public:
		DX11VertexBuffer(ID3D11Device* Device);
		~DX11VertexBuffer();
		void Create() override;
		void Bind(ID3D11DeviceContext* Context) override;
		void Unbind(ID3D11DeviceContext* Context) override;
		void SetData(VertexArray Vertices) override;
		void Release() override;

		// Getters

		unsigned int GetVertexCount() const { return static_cast<unsigned int>(Vertices_.size()); }
		ComPtr<ID3D11Buffer> GetBuffer() const { return VertexBuffer_; }
		VertexArray GetVertices() const { return Vertices_; }
	private:
		ID3D11Device* Device_;
		ComPtr<ID3D11DeviceContext> DeviceContext_;
		ComPtr<ID3D11Buffer> VertexBuffer_;
		VertexArray Vertices_;
	};
} // namespace KKAM::Graphics