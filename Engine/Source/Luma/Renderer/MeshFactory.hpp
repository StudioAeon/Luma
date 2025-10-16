#pragma once

#include "Mesh.hpp"

namespace Luma {

	class MeshFactory
	{
	public:
		static Ref<Mesh> CreateBox(const glm::vec3& size);
		static Ref<Mesh> CreateSphere(float radius);
		static Ref<Mesh> CreateCapsule(float radius, float height);
	};

}