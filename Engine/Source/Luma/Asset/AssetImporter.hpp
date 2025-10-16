#pragma once

#include "AssetSerializer.hpp"

namespace Luma {

	class AssetImporter
	{
	public:
		static void Init();
		static void Serialize(const Ref<Asset>& asset);
		static bool TryLoadData(Ref<Asset>& asset);

	private:
		static std::unordered_map<AssetType, Scope<AssetSerializer>> s_Serializers;
	};

}