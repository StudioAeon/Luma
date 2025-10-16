set(EDITOR_SOURCES
		AssetEditorPanel.cpp
		ContentBrowserPanel.cpp
		DefaultAssetEditors.cpp
		EditorCamera.cpp
		ObjectsPanel.cpp
		SceneHierarchyPanel.cpp
)

set(EDITOR_HEADERS
		AssetEditorPanel.hpp
		ContentBrowserPanel.hpp
		DefaultAssetEditors.hpp
		EditorCamera.hpp
		ObjectsPanel.hpp
		SceneHierarchyPanel.hpp
)

target_sources(Luma
		PRIVATE
		${EDITOR_SOURCES}
		${EDITOR_HEADERS}
)