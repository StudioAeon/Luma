#include "lmpch.hpp"
#include "Renderer.hpp"

namespace Luma {

	Renderer* Renderer::s_Instance = new Renderer();
	RendererAPIType RendererAPI::s_CurrentRendererAPI = RendererAPIType::OpenGL;

	void Renderer::Init()
	{
		LM_RENDER({ RendererAPI::Init(); });
	}

	void Renderer::Clear()
	{
		LM_RENDER({
			RendererAPI::Clear(0.0f, 0.0f, 0.0f, 1.0f);
		});
	}

	void Renderer::DrawIndexed(uint32_t count, bool depthTest)
	{
		LM_RENDER_2(count, depthTest, {
			RendererAPI::DrawIndexed(count, depthTest);
		});
	}

	void Renderer::Clear(float r, float g, float b, float a)
	{
		LM_RENDER_4(r, g, b, a, {
			RendererAPI::Clear(r, g, b, a);
		});
	}

	void Renderer::ClearMagenta()
	{
		Clear(1, 0, 1);
	}

	void Renderer::SetClearColor(float r, float g, float b, float a)
	{
	}

	void Renderer::WaitAndRender()
	{
		s_Instance->m_CommandQueue.Execute();
	}

}