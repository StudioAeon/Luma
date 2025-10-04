#pragma once

#include "RenderCommandQueue.hpp"
#include "RendererAPI.hpp"

namespace Luma {

	class ShaderLibrary;

	// TODO: Maybe this should be renamed to RendererAPI? Because we want an actual renderer vs API calls...
	class Renderer
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		// Commands
		static void Clear();
		static void Clear(float r, float g, float b, float a = 1.0f);
		static void SetClearColor(float r, float g, float b, float a);

		static void DrawIndexed(uint32_t count, bool depthTest = true);

		static void ClearMagenta();

		static void Init();

		static const Scope<ShaderLibrary>& GetShaderLibrary() { return Get().m_ShaderLibrary; }

		static void* Submit(RenderCommandFn fn, uint32_t size)
		{
			return s_Instance->m_CommandQueue.Allocate(fn, size);
		}

		void WaitAndRender();

		inline static Renderer& Get() { return *s_Instance; }
	private:
		static Renderer* s_Instance;

		RenderCommandQueue m_CommandQueue;
		Scope<ShaderLibrary> m_ShaderLibrary;
	};

}

#define LM_RENDER_PASTE2(a, b) a ## b
#define LM_RENDER_PASTE(a, b) LM_RENDER_PASTE2(a, b)
#define LM_RENDER_UNIQUE(x) LM_RENDER_PASTE(x, __LINE__)

#define LM_RENDER(code) \
	struct LM_RENDER_UNIQUE(ZNRenderCommand) \
	{\
		static void Execute(void*)\
		{\
			code\
		}\
	};\
	{\
		auto mem = ::Luma::Renderer::Submit(LM_RENDER_UNIQUE(ZNRenderCommand)::Execute, sizeof(LM_RENDER_UNIQUE(ZNRenderCommand)));\
		new (mem) LM_RENDER_UNIQUE(ZNRenderCommand)();\
	}\

#define LM_RENDER_1(arg0, code) \
	do {\
	struct LM_RENDER_UNIQUE(ZNRenderCommand) \
	{\
		LM_RENDER_UNIQUE(ZNRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0) \
		: arg0(arg0) {}\
		\
		static void Execute(void* argBuffer)\
		{\
			auto& arg0 = ((LM_RENDER_UNIQUE(ZNRenderCommand)*)argBuffer)->arg0;\
			code\
		}\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
	};\
	{\
		auto mem = ::Luma::Renderer::Submit(LM_RENDER_UNIQUE(ZNRenderCommand)::Execute, sizeof(LM_RENDER_UNIQUE(ZNRenderCommand)));\
		new (mem) LM_RENDER_UNIQUE(ZNRenderCommand)(arg0);\
	} } while(0)

#define LM_RENDER_2(arg0, arg1, code) \
	struct LM_RENDER_UNIQUE(ZNRenderCommand) \
	{\
		LM_RENDER_UNIQUE(ZNRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1) \
		: arg0(arg0), arg1(arg1) {}\
		\
		static void Execute(void* argBuffer)\
		{\
			auto& arg0 = ((LM_RENDER_UNIQUE(ZNRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((LM_RENDER_UNIQUE(ZNRenderCommand)*)argBuffer)->arg1;\
			code\
		}\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
	};\
	{\
		auto mem = ::Luma::Renderer::Submit(LM_RENDER_UNIQUE(ZNRenderCommand)::Execute, sizeof(LM_RENDER_UNIQUE(ZNRenderCommand)));\
		new (mem) LM_RENDER_UNIQUE(ZNRenderCommand)(arg0, arg1);\
	}\

#define LM_RENDER_3(arg0, arg1, arg2, code) \
	struct LM_RENDER_UNIQUE(ZNRenderCommand) \
	{\
		LM_RENDER_UNIQUE(ZNRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2) \
		: arg0(arg0), arg1(arg1), arg2(arg2) {}\
		\
		static void Execute(void* argBuffer)\
		{\
			auto& arg0 = ((LM_RENDER_UNIQUE(ZNRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((LM_RENDER_UNIQUE(ZNRenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((LM_RENDER_UNIQUE(ZNRenderCommand)*)argBuffer)->arg2;\
			code\
		}\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
	};\
	{\
		auto mem = ::Luma::Renderer::Submit(LM_RENDER_UNIQUE(ZNRenderCommand)::Execute, sizeof(LM_RENDER_UNIQUE(ZNRenderCommand)));\
		new (mem) LM_RENDER_UNIQUE(ZNRenderCommand)(arg0, arg1, arg2);\
	}\

#define LM_RENDER_4(arg0, arg1, arg2, arg3, code) \
	struct LM_RENDER_UNIQUE(ZNRenderCommand) \
	{\
		LM_RENDER_UNIQUE(ZNRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
		: arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
		\
		static void Execute(void* argBuffer)\
		{\
			auto& arg0 = ((LM_RENDER_UNIQUE(ZNRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((LM_RENDER_UNIQUE(ZNRenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((LM_RENDER_UNIQUE(ZNRenderCommand)*)argBuffer)->arg2;\
			auto& arg3 = ((LM_RENDER_UNIQUE(ZNRenderCommand)*)argBuffer)->arg3;\
			code\
		}\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
	};\
	{\
		auto mem = Renderer::Submit(LM_RENDER_UNIQUE(ZNRenderCommand)::Execute, sizeof(LM_RENDER_UNIQUE(ZNRenderCommand)));\
		new (mem) LM_RENDER_UNIQUE(ZNRenderCommand)(arg0, arg1, arg2, arg3);\
	}

#define LM_RENDER_S(code) auto self = this;\
LM_RENDER_1(self, code)

#define LM_RENDER_S1(arg0, code) auto self = this;\
LM_RENDER_2(self, arg0, code)

#define LM_RENDER_S2(arg0, arg1, code) auto self = this;\
LM_RENDER_3(self, arg0, arg1, code)

#define LM_RENDER_S3(arg0, arg1, arg2, code) auto self = this;\
LM_RENDER_4(self, arg0, arg1, arg2, code)