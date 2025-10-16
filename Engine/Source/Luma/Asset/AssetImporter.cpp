#include "lmpch.hpp"
#include "AssetImporter.hpp"

namespace Luma {

	void AssetImporter::Init()
	{
		s_Serializers[AssetType::Texture] = CreateScope<TextureSerializer>();
		s_Serializers[AssetType::Mesh] = CreateScope<MeshSerializer>();
		s_Serializers[AssetType::EnvMap] = CreateScope<EnvironmentSerializer>();
	}

	void AssetImporter::Serialize(const Ref<Asset>& asset)
	{
		if (s_Serializers.find(asset->Type) == s_Serializers.end())
		{
			LM_CORE_WARN("There's currently no importer for assets of type {0}", asset->Extension);
			return;
		}

		s_Serializers[asset->Type]->Serialize(asset);
	}

	bool AssetImporter::TryLoadData(Ref<Asset>& asset)
	{
		if (asset->Type == AssetType::Directory)
			return false;

		if (s_Serializers.find(asset->Type) == s_Serializers.end())
		{
			LM_CORE_WARN("There's currently no importer for assets of type {0}", asset->Extension);
			return false;
		}

		return s_Serializers[asset->Type]->TryLoadData(asset);
	}

	std::unordered_map<AssetType, Scope<AssetSerializer>> AssetImporter::s_Serializers;

}