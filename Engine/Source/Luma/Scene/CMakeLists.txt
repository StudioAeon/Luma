set(SCENE_SOURCES
		Entity.cpp
		Scene.cpp
)

set(SCENE_HEADERS
		Components.hpp
		Entity.hpp
		Scene.hpp
)

target_sources(Luma
		PRIVATE
		${SCENE_SOURCES}
		${SCENE_HEADERS}
)