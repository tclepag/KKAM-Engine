#pragma once

#include "Common/Common.h"

namespace KKAM::Core {
	class Engine;
}

namespace KKAM::Graphics {
	using KKAM::Core::Engine;
	class IGraphics {
	public:
		IGraphics(Engine* engine) : Engine_(engine) {}
		virtual ~IGraphics() = default;
		virtual void Initialize() = 0;
		virtual void HandleResize() = 0;
		virtual void Redraw() = 0;
		virtual void Render(std::function<void()> RenderOperation) = 0;
		virtual void Shutdown() = 0;
	protected:
		Engine* Engine_;
		std::function<void()> LastRenderOperation_;
	};
} // namespace KKAM::Graphics