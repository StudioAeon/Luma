#include "lmpch.hpp"
#include "OpenGLContext.hpp"

#include <SDL3/SDL.h>
#include <glad/glad.h>

namespace Luma {

	OpenGLContext::~OpenGLContext()
	{
		if (m_GLContext)
		{
			SDL_GL_DestroyContext(m_GLContext);
			m_GLContext = nullptr;
		}
	}

	void OpenGLContext::SetWindow(SDL_Window* window)
	{
		LM_CORE_ASSERT(window, "Window is null!");
		m_Window = window;
	}

	void OpenGLContext::Init()
	{
		LM_CORE_ASSERT(m_Window, "Window must be set before Init()!");

		if (m_GLContext) {
			LM_CORE_WARN_TAG("Renderer", "OpenGL context already initialized!");
			return;
		}

		LM_CORE_INFO_TAG("Renderer", "OpenGLContext::Init");

		m_GLContext = SDL_GL_CreateContext(m_Window);
		if (!m_GLContext) {
			LM_CORE_ERROR_TAG("Renderer", "Failed to create OpenGL context: {}", SDL_GetError());
			return;
		}

		SDL_GL_MakeCurrent(m_Window, m_GLContext);

		int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));
		LM_CORE_ASSERT(status, "Failed to initialize Glad!");

		LM_CORE_INFO_TAG("Renderer", "OpenGL Info:");
		LM_CORE_INFO_TAG("Renderer", "  Vendor:   {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		LM_CORE_INFO_TAG("Renderer", "  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		LM_CORE_INFO_TAG("Renderer", "  Version:  {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}

}