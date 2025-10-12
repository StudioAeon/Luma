set(UTILITIES_SOURCES
		FileSystem.cpp
		StringUtils.cpp
)

set(UTILITIES_HEADERS
		FileSystem.hpp
		StringUtils.hpp
)

target_sources(Luma
		PRIVATE
		${UTILITIES_SOURCES}
		${UTILITIES_HEADERS}
)