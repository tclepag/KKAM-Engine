#pragma once

#include "Common/Common.h"

namespace Core {
	class Engine;
}

namespace Graphics {
	using Core::Engine;
	template<typename ContextType>
	class IShader {
	public:
		IShader(Engine* engine) : Engine_(engine) {}
		virtual ~IShader() = default;
		virtual void Create() = 0;
		virtual void SetTransformationMatrices(const glm::mat4& world,
			const glm::mat4& view,
			const glm::mat4& projection) = 0;
		virtual void Bind(ContextType Context) = 0;
		virtual void Unbind(ContextType Context) = 0;
		virtual void SetVertexPath(const std::string& path) = 0;
		virtual void SetFragmentPath(const std::string& path) = 0;
		virtual void Release() = 0;
	protected:
		Engine* Engine_;
		std::string VertexPath_;
		std::string FragmentPath_;
		ComPtr<ID3DBlob> VertexBlob_;
		ComPtr<ID3DBlob> PixelBlob_;
	};
}