#include "lmpch.hpp"
#include "FileStream.hpp"

namespace Luma
{
	//==============================================================================
	/// FileStreamWriter
	FileStreamWriter::FileStreamWriter(const std::filesystem::path& path)
		: m_Path(path)
	{
		m_Stream = std::ofstream(path, std::ofstream::out | std::ofstream::binary);
	}

	FileStreamWriter::~FileStreamWriter()
	{
		m_Stream.close();
	}

	bool FileStreamWriter::WriteData(const char* data, size_t size)
	{
		m_Stream.write(data, size);
		return true;
	}

	//==============================================================================
	/// FileStreamReader
	FileStreamReader::FileStreamReader(const std::filesystem::path& path)
		: m_Path(path)
	{
		m_Stream = std::ifstream(path, std::ifstream::in | std::ifstream::binary);
	}

	FileStreamReader::~FileStreamReader()
	{
		m_Stream.close();
	}

	bool FileStreamReader::ReadData(char* destination, size_t size)
	{
		m_Stream.read(destination, size);
		return true;
	}

	size_t FileStreamReader::GetStreamLength()
	{
		const std::streampos currentPosition = m_Stream.tellg();
		m_Stream.seekg(0, std::ios::end);

		const std::streampos length = m_Stream.tellg() - currentPosition;
		m_Stream.seekg(currentPosition);

		return static_cast<size_t>(length);
	}

	size_t FileStreamReader::ReadAsString(std::string& outString)
	{
		const size_t size = GetStreamLength();

		if (!size)
			return 0;

		outString.resize(size);

		try
		{
			if (m_Stream.read(static_cast<std::ifstream::char_type*> (outString.data()), static_cast<std::streamsize>(size)))
			{
				return size;
			}
			else
			{
				std::string filename = m_Path.filename().string().c_str();
				throw std::runtime_error(std::string("Failed to read from file: " + filename).c_str());
			}

		}
		catch (const std::exception& e)
		{
			std::string filename = m_Path.filename().string().c_str();
			throw std::runtime_error(std::string("Failed to read from file: " + filename + ": " + e.what()).c_str());
		}

		return 0;
	}

} // namespace Luma