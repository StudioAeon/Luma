set(ASSET_SOURCES
		AssetImporter.cpp
		AssetManager.cpp
		AssetSerializer.cpp
)

set(ASSET_HEADERS
		Asset.hpp
		AssetImporter.hpp
		AssetManager.hpp
		AssetSerializer.hpp
)

target_sources(Luma
		PRIVATE
		${ASSET_SOURCES}
		${ASSET_HEADERS}
)