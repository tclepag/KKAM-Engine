#pragma once

#include "Common/Common.h"

namespace KKAM::Graphics {

	template<typename ContextType>
	class IVertexBuffer {
	public:
		virtual ~IVertexBuffer() = default;
		virtual void Create() = 0;
		virtual void Bind(ContextType Context) = 0;
		virtual void Unbind(ContextType Context) = 0;
		virtual void SetData(VertexArray Vertices) = 0;
		virtual void Release() = 0;
	};
} // namespace KKAM::Graphics