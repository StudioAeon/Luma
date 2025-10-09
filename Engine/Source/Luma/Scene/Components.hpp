#pragma once

#include "Luma/Core/UUID.hpp"
#include "Luma/Renderer/Texture.hpp"
#include "Luma/Renderer/Mesh.hpp"
#include "Luma/Scene/SceneCamera.hpp"

#include <glm/glm.hpp>

namespace Luma {

	struct IDComponent
	{
		UUID ID = 0;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent& other)
			: Tag(other.Tag) {}
		TagComponent(const std::string& tag)
			: Tag(tag) {}

		operator std::string& () { return Tag; }
		operator const std::string& () const { return Tag; }
	};

	struct TransformComponent
	{
		glm::mat4 Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other)
			: Transform(other.Transform) {}
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct MeshComponent
	{
		Ref<Luma::Mesh> Mesh;

		MeshComponent() = default;
		MeshComponent(const MeshComponent& other)
			: Mesh(other.Mesh) {}
		MeshComponent(const Ref<Luma::Mesh>& mesh)
			: Mesh(mesh) {}

		operator Ref<Luma::Mesh> () { return Mesh; }
	};

	struct ScriptComponent
	{
		//TODO: Find a scripting language
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other)
			: Camera(other.Camera), Primary(other.Primary) {}

		operator SceneCamera& () { return Camera; }
		operator const SceneCamera& () const { return Camera; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other)
			: Color(other.Color), Texture(other.Texture), TilingFactor(other.TilingFactor) {}
	};

}