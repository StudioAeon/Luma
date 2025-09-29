set(RENDERER_SOURCES
		RenderCommand.cpp
		RenderCommandQueue.cpp
		Renderer.cpp
)

set(RENDERER_HEADERS
		RenderCommand.hpp
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