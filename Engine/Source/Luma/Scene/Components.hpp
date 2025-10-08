#pragma once

#include "Luma/Renderer/Texture.hpp"
#include "Luma/Renderer/Mesh.hpp"
#include "Luma/Renderer/Camera.hpp"

#include <glm/glm.hpp>

namespace Luma {

	struct TagComponent
	{
		std::string Tag;

		operator std::string& () { return Tag; }
		operator const std::string& () const { return Tag; }
	};

	struct TransformComponent
	{
		glm::mat4 Transform;

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct MeshComponent
	{
		Ref<Luma::Mesh> Mesh;

		operator Ref<Luma::Mesh> () { return Mesh; }
	};

	struct CameraComponent
	{
		//OrthographicCamera Camera;
		Luma::Camera Camera;
		bool Primary = true;

		operator Luma::Camera& () { return Camera; }
		operator const Luma::Camera& () const { return Camera; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;
	};

}