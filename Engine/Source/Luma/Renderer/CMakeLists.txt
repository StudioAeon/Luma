set(RENDERER_SOURCES
		Buffer.cpp
		Camera.cpp
		Framebuffer.cpp
		Mesh.cpp
		RenderCommandQueue.cpp
		Renderer.cpp
		Shader.cpp
		Texture.cpp
)

set(RENDERER_HEADERS
		Buffer.hpp
		Camera.hpp
		Framebuffer.hpp
		Mesh.hpp
		RenderCommandQueue.hpp
		Renderer.hpp
		RendererAPI.hpp
		Shader.hpp
		Texture.hpp
)

add_subdirectory(Backend/OpenGL)

target_sources(Luma
		PRIVATE
		${RENDERER_SOURCES}
		${RENDERER_HEADERS}
)