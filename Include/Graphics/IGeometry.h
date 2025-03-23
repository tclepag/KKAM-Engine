#pragma once

#include "Common/Common.h"

namespace KKAM::Graphics {
	class IGeometry {
	public:
		virtual ~IGeometry() = default;
		virtual void Create() = 0;
		virtual void Rebake() = 0;
		virtual void Draw() = 0;
		virtual void Release() = 0;
		virtual void SetData(VertexArray Vertices, Indices Indices) = 0;
		virtual void SetVertexPath(const std::wstring& path) = 0;
		virtual void SetFragmentPath(const std::wstring& path) = 0;
	protected:
		std::wstring VertexPath_;
		std::wstring FragmentPath_;
	};
} // namespace KKAM::Graphics