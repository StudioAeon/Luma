#pragma once

#include "Luma.hpp"

namespace Luma {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		Ref<Shader> m_QuadShader;
		Ref<Shader> m_HDRShader;
		Ref<Shader> m_GridShader;
		Ref<Mesh> m_Mesh;
		Ref<Mesh> m_SphereMesh, m_PlaneMesh;
		Ref<Texture2D> m_BRDFLUT;

		Ref<MaterialInstance> m_MeshMaterial;
		Ref<MaterialInstance> m_GridMaterial;
		std::vector<Ref<MaterialInstance>> m_MetalSphereMaterialInstances;
		std::vector<Ref<MaterialInstance>> m_DielectricSphereMaterialInstances;

		float m_GridScale = 16.025f, m_GridSize = 0.025f;
		float m_MeshScale = 1.0f;

		struct AlbedoInput
		{
			glm::vec3 Color = { 0.972f, 0.96f, 0.915f }; // Silver, from https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
			Ref<Texture2D> TextureMap;
			bool SRGB = true;
			bool UseTexture = false;
		};
		AlbedoInput m_AlbedoInput;

		struct NormalInput
		{
			Ref<Texture2D> TextureMap;
			bool UseTexture = false;
		};
		NormalInput m_NormalInput;

		struct MetalnessInput
		{
			float Value = 1.0f;
			Ref<Texture2D> TextureMap;
			bool UseTexture = false;
		};
		MetalnessInput m_MetalnessInput;

		struct RoughnessInput
		{
			float Value = 0.5f;
			Ref<Texture2D> TextureMap;
			bool UseTexture = false;
		};
		RoughnessInput m_RoughnessInput;

		std::unique_ptr<Framebuffer> m_Framebuffer, m_FinalPresentBuffer;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<TextureCube> m_EnvironmentCubeMap, m_EnvironmentIrradiance;

		Camera m_Camera;

		struct Light
		{
			glm::vec3 Direction;
			glm::vec3 Radiance;
		};
		Light m_Light;
		float m_LightMultiplier = 0.3f;

		// PBR params
		float m_Exposure = 1.0f;

		bool m_RadiancePrefilter = false;

		float m_EnvMapRotation = 0.0f;

		enum class Scene : uint32_t
		{
			Spheres = 0, Model = 1
		};
		Scene m_Scene;

		// Editor resources
		Ref<Texture2D> m_CheckerboardTex;
	};

}