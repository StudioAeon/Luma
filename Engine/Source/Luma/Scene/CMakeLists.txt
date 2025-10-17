set(SCENE_SOURCES
		Entity.cpp
		EntityFactory.cpp
		Scene.cpp
		SceneCamera.cpp
		SceneSerializer.cpp
)

set(SCENE_HEADERS
		Components.hpp
		Entity.hpp
		EntityFactory.hpp
		Scene.hpp
		SceneCamera.hpp
		SceneSerializer.hpp
)

target_sources(Luma
		PRIVATE
		${SCENE_SOURCES}
		${SCENE_HEADERS}
)