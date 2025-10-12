set(UTILITIES_SOURCES
		CommandLineParser.cpp
		FileSystem.cpp
		StringUtils.cpp
)

set(UTILITIES_HEADERS
		CommandLineParser.hpp
		ContainerUtils.hpp
		FileSystem.hpp
		ProcessHelper.hpp
		StringUtils.hpp
)

target_sources(Luma
		PRIVATE
		${UTILITIES_SOURCES}
		${UTILITIES_HEADERS}
)