set(OPENGL_SOURCES
		OpenGLRendererAPI.cpp
)

target_sources(Luma
		PRIVATE
		${OPENGL_SOURCES}
)