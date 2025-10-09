#include "lmpch.hpp"
#include "Camera.hpp"

namespace Luma {

	Camera::Camera(const glm::mat4& projectionMatrix)
		: m_ProjectionMatrix(projectionMatrix)
	{}

}