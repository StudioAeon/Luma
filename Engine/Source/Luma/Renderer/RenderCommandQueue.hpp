#pragma once

#include "lmpch.hpp"

namespace Luma {

	class RenderCommandQueue
	{
	public:
		using RenderCommand = std::function<uint32_t(void*)>;
		typedef uint32_t(*RenderCommandFn)(void*);

		RenderCommandQueue();
		~RenderCommandQueue();

		void Submit(const RenderCommand& command);
		void SubmitCommand(RenderCommandFn fn, void* params, uint32_t size);
		void Execute();
	private:
		uint8_t* m_CommandBuffer;
		uint8_t* m_CommandBufferPtr;
		uint32_t m_RenderCommandCount = 0;
	};

}