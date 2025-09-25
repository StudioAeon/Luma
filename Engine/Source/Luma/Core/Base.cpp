#include "lmpch.hpp"
#include "Base.hpp"

#include "Log.hpp"

namespace Luma {

	void InitializeCore()
	{
		Log::Init();

		LM_CORE_TRACE("Luma Engine {}", LM_VERSION);
		LM_CORE_TRACE("Initializing...");
	}

	void ShutdownCore()
	{
		LM_CORE_TRACE("Shutting down...");
		Log::Shutdown();
	}

}