set(OPENGL_SOURCES
		OpenGLIndexBuffer.cpp
		OpenGLRendererAPI.cpp
		OpenGLVertexBuffer.cpp
)

set(OPENGL_HEADERS
		OpenGLIndexBuffer.hpp
		OpenGLVertexBuffer.hpp
)

target_sources(Luma
		PRIVATE
		${OPENGL_SOURCES}
		${OPENGL_HEADERS}
)