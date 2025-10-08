set(MATH_SOURCES
		Mat4.cpp
)

set(MATH_HEADERS
		AABB.hpp
		Mat4.hpp
		Ray.hpp
)

target_sources(Luma
		PRIVATE
		${MATH_SOURCES}
		${MATH_HEADERS}

)