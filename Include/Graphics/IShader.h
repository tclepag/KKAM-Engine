#pragma once

#include "Common/Common.h"

namespace KKAM::Graphics {
	template<typename ContextType>
	class IShader {
	public:
		virtual ~IShader() = default;
		virtual void Create() = 0;
		virtual void SetTransformationMatrices(const DirectX::XMMATRIX& world,
			const DirectX::XMMATRIX& view,
			const DirectX::XMMATRIX& projection) = 0;
		virtual void Bind(ContextType Context) = 0;
		virtual void Unbind(ContextType Context) = 0;
		virtual void SetVertexPath(const std::wstring& path) = 0;
		virtual void SetFragmentPath(const std::wstring& path) = 0;
		virtual void Release() = 0;

		static DirectX::XMMATRIX CreatePerspectiveProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ) {
			return DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
		}

		static DirectX::XMMATRIX CreateOrthographicProjectionMatrix(float width, float height, float nearZ, float farZ) {
			return DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);
		}
	protected:
		std::wstring VertexPath_;
		std::wstring FragmentPath_;
	};
}