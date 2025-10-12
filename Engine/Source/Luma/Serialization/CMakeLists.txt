set(SERIALIZATION_SOURCES
		FileStream.cpp
		MemoryStream.cpp
		StreamReader.cpp
		StreamWriter.cpp
)

set(SERIALIZATION_HEADERS
		FileStream.hpp
		MemoryStream.hpp
		Serialization.hpp
		StreamReader.hpp
		StreamWriter.hpp
)

target_sources(Luma
		PRIVATE
		${SERIALIZATION_SOURCES}
		${SERIALIZATION_HEADERS}
)