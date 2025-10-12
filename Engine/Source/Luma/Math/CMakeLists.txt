set(MATH_SOURCES
		Noise.cpp
)

set(MATH_HEADERS
		AABB.hpp
		Noise.hpp
		Ray.hpp
)

target_sources(Luma
		PRIVATE
		${MATH_SOURCES}
		${MATH_HEADERS}
)