#include "lmpch.hpp"
#include "Luma/Utilities/ProcessHelper.hpp"

namespace Luma {

	static std::unordered_map<UUID, PROCESS_INFORMATION> s_WindowsProcessStorage;

	UUID ProcessHelper::CreateProcess(const ProcessInfo& inProcessInfo)
	{
		std::filesystem::path workingDirectory = inProcessInfo.WorkingDirectory.empty() ? inProcessInfo.FilePath.parent_path() : inProcessInfo.WorkingDirectory;

		std::wstring commandLine = inProcessInfo.IncludeFilePathInCommands ? inProcessInfo.FilePath.wstring() : L"";

		if (!inProcessInfo.CommandLine.empty())
		{
			// Convert UTF-8 to UTF-16
			int wideSize = MultiByteToWideChar(
				CP_UTF8, 0,
				inProcessInfo.CommandLine.c_str(),
				static_cast<int>(inProcessInfo.CommandLine.length()),
				nullptr, 0
			);

			if (wideSize > 0)
			{
				std::wstring wideArgs(wideSize, L'\0');
				MultiByteToWideChar(
					CP_UTF8, 0,
					inProcessInfo.CommandLine.c_str(),
					static_cast<int>(inProcessInfo.CommandLine.length()),
					wideArgs.data(), wideSize
				);

				if (commandLine.size() > 0)
				{
					commandLine += L" " + wideArgs;
				}
				else
				{
					commandLine = wideArgs;
				}
			}
		}

		PROCESS_INFORMATION processInformation;
		ZeroMemory(&processInformation, sizeof(PROCESS_INFORMATION));

		STARTUPINFOW startupInfo;
		ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
		startupInfo.cb = sizeof(STARTUPINFO);

		DWORD creationFlags = NORMAL_PRIORITY_CLASS;

		if (inProcessInfo.Detached)
			creationFlags |= DETACHED_PROCESS;

		BOOL success = ::CreateProcessW(
			inProcessInfo.FilePath.c_str(), commandLine.data(),
			NULL, NULL, FALSE, creationFlags, NULL,
			workingDirectory.c_str(), &startupInfo, &processInformation);

		if (!success)
		{
			CloseHandle(processInformation.hThread);
			CloseHandle(processInformation.hProcess);
			return 0;
		}

		UUID processID = UUID();

		if (inProcessInfo.Detached)
		{
			CloseHandle(processInformation.hThread);
			CloseHandle(processInformation.hProcess);
		}
		else
		{
			s_WindowsProcessStorage[processID] = processInformation;
		}

		return processID;
	}

	void ProcessHelper::DestroyProcess(UUID inHandle, uint32_t inExitCode)
	{
		LM_CORE_VERIFY(s_WindowsProcessStorage.find(inHandle) != s_WindowsProcessStorage.end(), "Trying to destroy untracked process!");
		const auto& processInformation = s_WindowsProcessStorage[inHandle];
		TerminateProcess(processInformation.hProcess, inExitCode);
		CloseHandle(processInformation.hThread);
		CloseHandle(processInformation.hProcess);
		s_WindowsProcessStorage.erase(inHandle);
	}

}