set(OPENGL_SOURCES
		OpenGLBuffer.cpp
		OpenGLRendererAPI.cpp
		OpenGLShader.cpp
)

set(OPENGL_HEADERS
		OpenGLBuffer.hpp
		OpenGLShader.hpp
)

target_sources(Luma
		PRIVATE
		${OPENGL_SOURCES}
		${OPENGL_HEADERS}
)