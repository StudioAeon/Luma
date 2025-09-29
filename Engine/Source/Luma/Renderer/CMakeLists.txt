set(RENDERER_SOURCES
		RenderCommandQueue.cpp
		Renderer.cpp
)

set(RENDERER_HEADERS
		RenderCommandQueue.hpp
		Renderer.hpp
		RendererAPI.hpp
)

add_subdirectory(Backend/OpenGL)

target_sources(Luma
		PRIVATE
		${RENDERER_SOURCES}
		${RENDERER_HEADERS}
)