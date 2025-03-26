#pragma once

#include "Common/Common.h"

namespace Core {
	class Engine;
}

namespace Graphics {
	using Core::Engine;
	class IGraphics {
	public:
		IGraphics(Engine* engine) : Engine_(engine) {}
		virtual ~IGraphics() = default;
		virtual void Initialize() = 0;
		virtual void HandleResize() = 0;
		virtual void Redraw() = 0;
		virtual void Render(std::function<void()> RenderOperation) = 0;
		virtual void Shutdown() = 0;

		void SetRenderActive(bool active) { RenderActive_ = active; }

		static glm::mat4 CreatePerspectiveProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ) {
			return glm::perspective(fovY, aspectRatio, nearZ, farZ);
		}

		static glm::mat4 CreateOrthographicProjectionMatrix(float width, float height, float nearZ, float farZ) {
			float halfWidth = width / 2.0f;
			float halfHeight = height / 2.0f;
			return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearZ, farZ);
		}

		// Getters

		Engine* GetEngine() const { return Engine_; }
		bool IsRenderActive() const { return RenderActive_; }
	protected:
		Engine* Engine_;
		std::function<void()> LastRenderOperation_;
		bool RenderActive_ = true;

		virtual void InitializeImGui() = 0;
	};
} // namespace Graphics