set(CORE_SOURCES
		Application.cpp
		Base.cpp
		Layer.cpp
		LayerStack.cpp
		Log.cpp
)

set(CORE_HEADERS
		Application.hpp
		Assert.hpp
		Base.hpp
		Input.hpp
		Layer.hpp
		LayerStack.hpp
		Log.hpp
		Version.hpp
)

target_sources(Luma
		PRIVATE
		${CORE_SOURCES}
		${CORE_HEADERS}
)