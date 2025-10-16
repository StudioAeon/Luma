#pragma once

#include "Luma/Renderer/Texture.hpp"
#include "Luma/Asset/AssetManager.hpp"

namespace Luma {

	class ObjectsPanel
	{
	public:
		ObjectsPanel();

		void OnImGuiRender();

	private:
		void DrawObject(const char* label, AssetHandle handle);

	private:
		Ref<Texture2D> m_CubeImage;
	};

}