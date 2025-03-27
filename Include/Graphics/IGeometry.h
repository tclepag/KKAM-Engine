#pragma once

#include "Common/Common.h"

namespace Core {
	class KEngine;
}

namespace Graphics {
	using Core::KEngine;
	class IGeometry {
	public:
		IGeometry(KEngine* KEngine) : Engine_(KEngine) {}
		virtual ~IGeometry() = default;
		virtual void Create() = 0;
		virtual void Rebake() = 0;
		virtual void Draw() = 0;
		virtual void Release() = 0;
		virtual void SetData(VertexArray Vertices, Indices Indices) = 0;
		virtual void SetVertexPath(const std::string& path) = 0;
		virtual void SetFragmentPath(const std::string& path) = 0;
		virtual void SetTransformMatrices(const glm::mat4& world,
			const glm::mat4& view,
			const glm::mat4& projection) = 0;
		virtual void SetWorldMatrix(const glm::mat4& world) = 0;
		virtual void SetViewMatrix(const glm::mat4& view) = 0;
		virtual void SetProjectionMatrix(const glm::mat4& projection) = 0;

		// Getters

		glm::mat4 GetWorldMatrix() const { return WorldMatrix_; }
		glm::mat4 GetViewMatrix() const { return ViewMatrix_; }
		glm::mat4 GetProjectionMatrix() const { return ProjectionMatrix_; }
	protected:
		KEngine* Engine_;
		std::string VertexPath_;
		std::string FragmentPath_;
		glm::mat4 WorldMatrix_;
		glm::mat4 ViewMatrix_;
		glm::mat4 ProjectionMatrix_;
	};
} // namespace Graphics