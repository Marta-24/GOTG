#pragma once

#include <Wiwa/core/Core.h>

#include <glm/glm.hpp>
#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/math/Frustum.h>
#include <Wiwa/utilities/render/FrameBuffer.h>
namespace Wiwa {
	class WI_API Camera
	{
	public:
		enum class CameraType {
			INVALID,
			ORTHOGRAPHIC,
			PERSPECTIVE
		};
		Math::Frustum frustrum;
		FrameBuffer* frameBuffer;
		bool cull = false;
		bool drawBoundingBoxes = false;
		bool drawFrustrums = true;
	private:
		float m_FOV;
		CameraType m_CameraType;

		glm::vec3 m_CameraPos;
		glm::vec3 m_CameraFront;
		glm::vec3 m_CameraUp;
		glm::vec3 m_CameraRot;
		glm::mat4 m_View;
		glm::mat4 m_Projection;

		float m_NearPlaneDist;
		float m_FarPlaneDist;
		float m_AspectRatio;
		void updateView();

		unsigned int vao, vbo, ebo;

		glm::vec3 m_FrustumPoints[8];
	public:
		Camera();
		~Camera();

		void setPlanes(const float nearPlane, const float farPlane);
		void setAspectRatio(const float aspectRatio);
		float getAspectRatio() { return m_AspectRatio; }

		void setResolution(const int width, const int height);

		void setFOV(const float fov);
		float getFOV() { return m_FOV; }

		void setPosition(const glm::vec3 position);
		void setFront(const glm::vec3 front);


		inline glm::vec3 getPosition() { return m_CameraPos; }
		inline glm::vec3 getFront() { return m_CameraFront; }
		inline glm::vec3 getUp() { return m_CameraUp; }
		inline glm::vec3 getRotation() { return m_CameraRot; }
		inline float getFar() { return m_FarPlaneDist; }
		inline float getNear() { return m_NearPlaneDist; }

		inline void setRotation(const glm::vec3 rot);
		void lookat(const glm::vec3 position);
		void SetPerspective(const float fov, const float aspectRatio, const float nearPlaneDistance=0.1f, const float farPlaneDistance=100.0f);
		void UpdateFrustrum();
		void SetOrthographic(const int width, const int height, const float nearPlaneDistance=0.1f, const float farPlaneDistance=100.0f);

		inline CameraType GetCameraType() { return m_CameraType; }

		inline glm::mat4 getView() { return m_View; }
		inline glm::mat4 getProjection() { return m_Projection; }
		
		void DrawFrustrum();

		inline void GetCornerPoints(glm::vec3* outPointArray)
		{
			float tanhfov = glm::tan(m_FOV * 0.5f);
			float tanvfov = glm::tan(m_FOV * 0.5f);
			float frontPlaneHalfWidth = tanhfov * m_NearPlaneDist;
			float frontPlaneHalfHeight = tanvfov * m_NearPlaneDist;
			float farPlaneHalfWidth = tanhfov * m_FarPlaneDist;
			float farPlaneHalfHeight = tanvfov * m_FarPlaneDist;

			glm::vec3 right = glm::cross(m_CameraFront, m_CameraUp);

			glm::vec3 nearCenter = m_CameraPos + m_CameraFront * m_NearPlaneDist;
			glm::vec3 nearHalfWidth = frontPlaneHalfWidth * right;
			glm::vec3 nearHalfHeight = frontPlaneHalfHeight * m_CameraUp;
			outPointArray[0] = nearCenter - nearHalfWidth - nearHalfHeight;
			outPointArray[1] = nearCenter + nearHalfWidth - nearHalfHeight;
			outPointArray[2] = nearCenter - nearHalfWidth + nearHalfHeight;
			outPointArray[3] = nearCenter + nearHalfWidth + nearHalfHeight;

			glm::vec3 farCenter = m_CameraPos + m_CameraFront * m_FarPlaneDist;
			glm::vec3 farHalfWidth = farPlaneHalfWidth * right;
			glm::vec3 farHalfHeight = farPlaneHalfHeight * m_CameraUp;
			outPointArray[4] = farCenter - farHalfWidth - farHalfHeight;
			outPointArray[5] = farCenter + farHalfWidth - farHalfHeight;
			outPointArray[6] = farCenter - farHalfWidth + farHalfHeight;
			outPointArray[7] = farCenter + farHalfWidth + farHalfHeight;
		}

		inline glm::vec3 FarPlanePos(float x, float y) const
		{
			float farPlaneHalfWidth = glm::tan(m_FOV * 0.5f)* m_FarPlaneDist;
			float farPlaneHalfHeight = glm::tan(m_FOV * 0.5f) * m_FarPlaneDist;
			x = x * farPlaneHalfWidth;
			y = y * farPlaneHalfHeight;
			glm::vec3 right = glm::cross(m_CameraFront, m_CameraUp);
			return m_CameraPos + m_CameraFront * m_FarPlaneDist + x * right + y * m_CameraUp;
		}
		inline glm::vec3 NearPlanePos(float x, float y) const 
		{
			float frontPlaneHalfWidth = glm::tan(m_FOV * 0.5f) * m_NearPlaneDist;
			float frontPlaneHalfHeight = glm::tan(m_FOV * 0.5f) * m_NearPlaneDist;
			x = x * frontPlaneHalfWidth; // Map [-1,1] to [-width/2, width/2].
			y = y * frontPlaneHalfHeight;  // Map [-1,1] to [-height/2, height/2].
			glm::vec3 right = glm::cross(m_CameraFront, m_CameraUp);
			return m_CameraPos + m_CameraFront * m_NearPlaneDist + x * right + y * m_CameraUp;
		}
	};
}