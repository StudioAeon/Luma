set(EDITOR_SOURCES
		SceneHierarchyPanel.cpp
)

set(EDITOR_HEADERS
		SceneHierarchyPanel.hpp
)

target_sources(Luma
		PRIVATE
		${EDITOR_SOURCES}
		${EDITOR_HEADERS}
)