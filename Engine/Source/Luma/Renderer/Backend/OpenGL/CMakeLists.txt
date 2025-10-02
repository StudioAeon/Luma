set(OPENGL_SOURCES
		OpenGLBuffer.cpp
		OpenGLFramebuffer.cpp
		OpenGLRendererAPI.cpp
		OpenGLShader.cpp
		OpenGLTexture.cpp
)

set(OPENGL_HEADERS
		OpenGLBuffer.hpp
		OpenGLFramebuffer.hpp
		OpenGLShader.hpp
		OpenGLTexture.hpp
)

target_sources(Luma
		PRIVATE
		${OPENGL_SOURCES}
		${OPENGL_HEADERS}
)