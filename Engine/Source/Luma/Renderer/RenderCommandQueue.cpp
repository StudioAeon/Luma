#include "lmpch.hpp"
#include "RenderCommandQueue.hpp"

#define LM_RENDER_TRACE(...) LM_CORE_TRACE(__VA_ARGS__)

namespace Luma {

	RenderCommandQueue::RenderCommandQueue()
	{
		m_CommandBuffer = new uint8_t[10 * 1024 * 1024]; // 10mb buffer
		m_CommandBufferPtr = m_CommandBuffer;
		memset(m_CommandBuffer, 0, 10 * 1024 * 1024);
	}

	RenderCommandQueue::~RenderCommandQueue()
	{
		delete[] m_CommandBuffer;
	}

	void RenderCommandQueue::Submit(const RenderCommand& command)
	{
		auto ptr = m_CommandBuffer;

		memcpy(m_CommandBuffer, &command, sizeof(RenderCommand));
		m_CommandBufferPtr += sizeof(RenderCommand);
		m_RenderCommandCount++;
	}

	void RenderCommandQueue::SubmitCommand(RenderCommandFn fn, void* params, uint32_t size)
	{
		uint8_t*& buffer = m_CommandBufferPtr;
		memcpy(buffer, &fn, sizeof(RenderCommandFn));
		buffer += sizeof(RenderCommandFn);
		memcpy(buffer, params, size);
		buffer += size;

		auto totalSize = sizeof(RenderCommandFn) + size;
		auto padding = totalSize % 16; // 16-byte alignment
		buffer += padding;

		m_RenderCommandCount++;
	}

	void RenderCommandQueue::Execute()
	{
		LM_RENDER_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes", m_RenderCommandCount, (m_CommandBufferPtr - m_CommandBuffer));

		uint8_t* buffer = m_CommandBuffer;

		for (int i = 0; i < m_RenderCommandCount; i++)
		{
			RenderCommandFn fn = *(RenderCommandFn*)buffer;
			buffer += sizeof(RenderCommandFn);
			buffer += (*fn)(buffer);

			auto padding = (uintptr_t)buffer % 16;
			buffer += padding;
		}

		m_CommandBufferPtr = m_CommandBuffer;
		m_RenderCommandCount = 0;
	}

}