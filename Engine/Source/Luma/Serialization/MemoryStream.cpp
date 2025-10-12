#include "lmpch.hpp"
#include "MemoryStream.hpp"

namespace Luma
{
	//==============================================================================
	/// MemoryStreamWriter
	MemoryStreamWriter::MemoryStreamWriter(Buffer& buffer, size_t size)
		: m_Buffer(buffer)
	{
		if (size > buffer.Size)
			buffer.Allocate((uint32_t)size);
	}

	MemoryStreamWriter::~MemoryStreamWriter()
	{
	}

	bool MemoryStreamWriter::WriteData(const char* data, size_t size)
	{
		if (m_WritePos + size > m_Buffer.Size)
			return false;

		m_Buffer.Write(data, (uint32_t)size, (uint32_t)m_WritePos);
		m_WritePos += size;
		return true;
	}

	//==============================================================================
	/// MemoryStreamReader
	MemoryStreamReader::MemoryStreamReader(const Buffer& buffer)
		: m_Buffer(buffer)
	{
	}

	MemoryStreamReader::~MemoryStreamReader()
	{
	}

	bool MemoryStreamReader::ReadData(char* destination, size_t size)
	{
		if (m_ReadPos + size > m_Buffer.Size)
			return false;

		memcpy(destination, (char*)m_Buffer.Data + m_ReadPos, size);
		m_ReadPos += size;
		return true;
	}

	size_t MemoryStreamReader::GetStreamLength()
	{
		return m_Buffer.Size;
	}

} // namespace Luma