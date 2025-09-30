set(RENDERER_SOURCES
		Buffer.cpp
		RenderCommandQueue.cpp
		Renderer.cpp
		Shader.cpp
)

set(RENDERER_HEADERS
		Buffer.hpp
		RenderCommandQueue.hpp
		Renderer.hpp
		RendererAPI.hpp
		Shader.hpp
)

add_subdirectory(Backend/OpenGL)

target_sources(Luma
		PRIVATE
		${RENDERER_SOURCES}
		${RENDERER_HEADERS}
)