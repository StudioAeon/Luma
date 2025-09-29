#pragma once

#include "RenderCommandQueue.hpp"
#include "RendererAPI.hpp"

namespace Luma {

	class Renderer
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		// Commands
		static void Clear();
		static void Clear(float r, float g, float b, float a = 1.0f);
		static void SetClearColor(float r, float g, float b, float a);

		static void ClearMagenta();

		void Init();

		static void* Submit(RenderCommandFn fn, unsigned int size)
		{
			return s_Instance->m_CommandQueue.Allocate(fn, size);
		}

		void WaitAndRender();

		inline static Renderer& Get() { return *s_Instance; }
	private:
		static Renderer* s_Instance;

		RenderCommandQueue m_CommandQueue;
	};

}

#define LM_RENDER_PASTE2(a, b) a ## b
#define LM_RENDER_PASTE(a, b) LM_RENDER_PASTE2(a, b)
#define LM_RENDER_UNIQUE(x) LM_RENDER_PASTE(x, __LINE__)

#define LM_RENDER(code) \
	struct LM_RENDER_UNIQUE(LMRenderCommand) \
	{\
		static void Execute(void*)\
		{\
			code\
		}\
	};\
	{\
		auto mem = RenderCommandQueue::Submit(sizeof(LM_RENDER_UNIQUE(LMRenderCommand)), LM_RENDER_UNIQUE(LMRenderCommand)::Execute);\
		new (mem) LM_RENDER_UNIQUE(LMRenderCommand)();\
	}\

#define LM_RENDER_I(arg0, code) \
	struct LM_RENDER_UNIQUE(LMRenderCommand) \
	{\
		LM_RENDER_UNIQUE(LMRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(param)>::type>::type arg0) \
		: arg0(arg0) {}\
		\
		static void Execute(void* self)\
		{\
			auto& arg0 = ((LM_RENDER_UNIQUE(LMRenderCommand)*)self)->arg0;\
			code\
		}\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
	};\
	{\
		auto mem = ::Luma::Renderer::Submit(LM_RENDER_UNIQUE(LMRenderCommand)::Execute, sizeof(LM_RENDER_UNIQUE(LMRenderCommand)));\
		new (mem) LM_RENDER_UNIQUE(LMRenderCommand)(arg0);\
	}\

#define LM_RENDER_II(arg0, arg1, code) \
	struct LM_RENDER_UNIQUE(LMRenderCommand) \
	{\
		LM_RENDER_UNIQUE(LMRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1) \
		: arg0(arg0), arg1(arg1) {}\
		\
		static void Execute(void* self)\
		{\
			auto& arg0 = ((LM_RENDER_UNIQUE(LMRenderCommand)*)self)->arg0;\
			auto& arg1 = ((LM_RENDER_UNIQUE(LMRenderCommand)*)self)->arg1;\
			code\
		}\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
	};\
	{\
		auto mem = ::Luma::Renderer::Submit(LM_RENDER_UNIQUE(LMRenderCommand)::Execute, sizeof(LM_RENDER_UNIQUE(LMRenderCommand)));\
		new (mem) LM_RENDER_UNIQUE(LMRenderCommand)(arg0, arg1);\
	}\

#define LM_RENDER_III(arg0, arg1, arg2, code) \
	struct LM_RENDER_UNIQUE(LMRenderCommand) \
	{\
		LM_RENDER_UNIQUE(LMRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2) \
		: arg0(arg0), arg1(arg1), arg2(arg2) {}\
		\
		static void Execute(void* self)\
		{\
			auto& arg0 = ((LM_RENDER_UNIQUE(LMRenderCommand)*)self)->arg0;\
			auto& arg1 = ((LM_RENDER_UNIQUE(LMRenderCommand)*)self)->arg1;\
			auto& arg2 = ((LM_RENDER_UNIQUE(LMRenderCommand)*)self)->arg2;\
			code\
		}\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
	};\
	{\
		auto mem = ::Luma::Renderer::Submit(LM_RENDER_UNIQUE(LMRenderCommand)::Execute, sizeof(LM_RENDER_UNIQUE(LMRenderCommand)));\
		new (mem) LM_RENDER_UNIQUE(LMRenderCommand)(arg0, arg1, arg2);\
	}\

#define LM_RENDER_IV(arg0, arg1, arg2, arg3, code) \
	struct LM_RENDER_UNIQUE(LMRenderCommand) \
	{\
		LM_RENDER_UNIQUE(LMRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
		: arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
		\
		static void Execute(void* self)\
		{\
			auto& arg0 = ((LM_RENDER_UNIQUE(LMRenderCommand)*)self)->arg0;\
			auto& arg1 = ((LM_RENDER_UNIQUE(LMRenderCommand)*)self)->arg1;\
			auto& arg2 = ((LM_RENDER_UNIQUE(LMRenderCommand)*)self)->arg2;\
			auto& arg3 = ((LM_RENDER_UNIQUE(LMRenderCommand)*)self)->arg3;\
			code\
		}\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
	};\
	{\
		auto mem = Renderer::Submit(LM_RENDER_UNIQUE(LMRenderCommand)::Execute, sizeof(LM_RENDER_UNIQUE(LMRenderCommand)));\
		new (mem) LM_RENDER_UNIQUE(LMRenderCommand)(arg0, arg1, arg2, arg3);\
	}