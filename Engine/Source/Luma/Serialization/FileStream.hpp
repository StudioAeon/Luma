#pragma once

#include "StreamWriter.hpp"
#include "StreamReader.hpp"
#include "Luma/Core/Buffer.hpp"

#include <filesystem>
#include <fstream>

namespace Luma
{
	//==============================================================================
	/// FileStreamWriter
	class FileStreamWriter : public StreamWriter
	{
	public:
		FileStreamWriter(const std::filesystem::path& path);
		FileStreamWriter(const FileStreamWriter&) = delete;
		virtual ~FileStreamWriter();

		bool IsStreamGood() const final { return m_Stream.good(); }
		uint64_t GetStreamPosition() final { return m_Stream.tellp(); }
		void SetStreamPosition(uint64_t position) final { m_Stream.seekp(position); }
		bool WriteData(const char* data, size_t size) final;

	private:
		std::filesystem::path m_Path;
		std::ofstream m_Stream;
	};

	//==============================================================================
	/// FileStreamReader
	class FileStreamReader : public StreamReader
	{
	public:
		FileStreamReader(const std::filesystem::path& path);
		FileStreamReader(const FileStreamReader&) = delete;
		~FileStreamReader();

		const std::filesystem::path& GetFilePath() const { return m_Path; }

		bool IsStreamGood() const final { return m_Stream.good(); }
		uint64_t GetStreamPosition() override { return m_Stream.tellg(); }
		void SetStreamPosition(uint64_t position) override { m_Stream.seekg(position); }
		bool ReadData(char* destination, size_t size) override;
		size_t GetStreamLength() override;

		// Trye to read the content of the file as string, starting from the current read position
		// @returns the size string after reading the contents of the file into it
		size_t ReadAsString(std::string& outString);

	private:
		std::filesystem::path m_Path;
		std::ifstream m_Stream;
	};

} // namespace Luma