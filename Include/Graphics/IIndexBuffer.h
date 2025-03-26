#pragma once

#include "Common/Common.h"

namespace Graphics {
	template<typename ContextType>
	class IIndexBuffer {
	public:
		virtual ~IIndexBuffer() = default;
		virtual void Create() = 0;
		virtual void Bind(ContextType Context) = 0;
		virtual void Unbind(ContextType Context) = 0;
		virtual void SetData(Indices Indices) = 0;
		virtual void Release() = 0;
	};
} // namespace Graphics