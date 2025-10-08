set(MATH_SOURCES
		Mat4.cpp
		Noise.cpp
)

set(MATH_HEADERS
		AABB.hpp
		Mat4.hpp
		Noise.hpp
		Ray.hpp
)

target_sources(Luma
		PRIVATE
		${MATH_SOURCES}
		${MATH_HEADERS}
)