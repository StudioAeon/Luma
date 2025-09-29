#pragma once

#include "Luma/Renderer/Renderer.hpp"

namespace Luma {

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void SetData(void* buffer, uint32_t size, uint32_t offset = 0) = 0;
		virtual void Bind() const = 0;

		virtual uint32_t GetSize() const = 0;
		virtual RendererID GetRendererID() const = 0;

		static IndexBuffer* Create(uint32_t size = 0);
	};

}