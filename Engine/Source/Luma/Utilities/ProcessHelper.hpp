#pragma once

#include "Luma/Core/UUID.hpp"

#include <filesystem>

namespace Luma {

	struct ProcessInfo
	{
		std::filesystem::path FilePath;
		std::filesystem::path WorkingDirectory = "";
		std::string CommandLine = "";
		bool Detached = false;
		bool IncludeFilePathInCommands = true;
	};

	class ProcessHelper
	{
	public:
		static UUID CreateProcess(const ProcessInfo& inProcessInfo);
		static void DestroyProcess(UUID inHandle, uint32_t inExitCode = 0);
	};

}