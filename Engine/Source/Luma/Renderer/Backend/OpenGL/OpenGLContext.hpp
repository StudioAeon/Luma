#pragma once

#include "Luma/Renderer/RendererContext.hpp"

struct SDL_Window;
struct SDL_GLContextState;
typedef struct SDL_GLContextState* SDL_GLContext;

namespace Luma {

	class OpenGLContext : public RendererContext
	{
	public:
		OpenGLContext() = default;
		virtual ~OpenGLContext();

		virtual void Init() override;

		void SetWindow(SDL_Window* window);
		SDL_GLContext GetNativeContext() const { return m_GLContext; }
	private:
		SDL_Window* m_Window = nullptr;
		SDL_GLContext m_GLContext = nullptr;
	};
}