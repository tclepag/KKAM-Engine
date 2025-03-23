#pragma once

#include "Common/Common.h"

namespace KKAM::Graphics {
	template<typename ContextType>
	class IShader {
	public:
		virtual ~IShader() = default;
		virtual void Create() = 0;
		virtual void Bind(ContextType Context) = 0;
		virtual void Unbind(ContextType Context) = 0;
		virtual void SetVertexPath(const std::wstring& path) = 0;
		virtual void SetFragmentPath(const std::wstring& path) = 0;
		virtual void Release() = 0;
	protected:
		std::wstring VertexPath_;
		std::wstring FragmentPath_;
	};
}