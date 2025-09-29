set(RENDERER_SOURCES
		IndexBuffer.cpp
		RenderCommandQueue.cpp
		Renderer.cpp
		Shader.cpp
		VertexBuffer.cpp
)

set(RENDERER_HEADERS
		IndexBuffer.hpp
		RenderCommandQueue.hpp
		Renderer.hpp
		RendererAPI.hpp
		Shader.hpp
		VertexBuffer.hpp
)

add_subdirectory(Backend/OpenGL)

target_sources(Luma
		PRIVATE
		${RENDERER_SOURCES}
		${RENDERER_HEADERS}
)