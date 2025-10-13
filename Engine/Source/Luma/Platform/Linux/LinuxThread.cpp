#include "lmpch.hpp"
#include "Luma/Core/Thread.hpp"

#include <pthread.h>

namespace Luma {

	Thread::Thread(const std::string& name)
		: m_Name(name)
	{
	}

	void Thread::SetName(const std::string& name)
	{
		pthread_setname_np(m_Thread.native_handle(), name.c_str());
	}

	void Thread::Join()
	{
		m_Thread.join();
	}

	// TODO: `ThreadSignal`

	std::thread::id Thread::GetID() const
	{
		return m_Thread.get_id();
	}
}