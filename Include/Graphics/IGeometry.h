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
		std::wstring VertexPath_;
		std::wstring FragmentPath_;
		glm::mat4 WorldMatrix_;
		glm::mat4 ViewMatrix_;
		glm::mat4 ProjectionMatrix_;
	};
} // namespace KKAM::Graphics