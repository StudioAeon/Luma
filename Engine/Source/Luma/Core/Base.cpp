#include "lmpch.hpp"
#include "Base.hpp"

#include "Log.hpp"
#include "Memory.hpp"

namespace Luma {

	void InitializeCore()
	{
		Allocator::Init();
		Log::Init();

		LM_CORE_TRACE_TAG("Core", "Luma Engine {}", LM_VERSION);
		LM_CORE_TRACE_TAG("Core", "Initializing...");
	}

	void ShutdownCore()
	{
		LM_CORE_TRACE_TAG("Core", "Shutting down...");
		Log::Shutdown();
	}

}