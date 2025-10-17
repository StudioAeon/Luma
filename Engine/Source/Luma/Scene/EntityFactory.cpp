#include "lmpch.hpp"
#include "EntityFactory.hpp"
#include "Scene.hpp"

namespace Luma {

	void EntityFactory::Fracture(Entity originalEntity)
	{
		auto& meshComponent = originalEntity.GetComponent<MeshComponent>();
		Ref<Mesh> originalMesh = meshComponent.Mesh;
		const auto& submeshes = originalMesh->GetSubmeshes();

		Ref<Scene> scene = Scene::GetScene(originalEntity.GetSceneUUID());

		for (const auto& submesh : submeshes)
		{
			Entity newEntity = scene->CreateEntity(submesh.MeshName);

			Ref<Mesh> newMesh = Ref<Mesh>::Create(originalMesh, submesh);
			auto& newMeshComponent = newEntity.AddComponent<MeshComponent>();
		}
	}

}