#include "lmpch.hpp"
#include "OpenGLContext.hpp"

#include "Luma/Core/Log.hpp"

#include <glad/glad.h>

namespace Luma {

	OpenGLContext::OpenGLContext(SDL_Window* windowHandle)
		: m_WindowHandle(windowHandle)
	{
	}

	OpenGLContext::~OpenGLContext()
	{
	}

	void OpenGLContext::Create()
	{
		LM_CORE_INFO("OpenGLContext::Create");

		m_GLContext = SDL_GL_CreateContext(m_WindowHandle);
		SDL_GL_MakeCurrent(m_WindowHandle, m_GLContext);
		int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));
		LM_CORE_ASSERT(status, "Failed to initialize Glad!");

#ifdef LM_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		LM_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Luma requires at least OpenGL version 4.5!");
#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		SDL_GL_SwapWindow(m_WindowHandle);
	}

}