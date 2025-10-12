set(MATH_SOURCES
		Math.cpp
		Noise.cpp
)

set(MATH_HEADERS
		AABB.hpp
		Math.hpp
		Noise.hpp
		Ray.hpp
)

target_sources(Luma
		PRIVATE
		${MATH_SOURCES}
		${MATH_HEADERS}
)