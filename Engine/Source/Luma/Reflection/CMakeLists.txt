set(REFLECTION_HEADERS
		MetaHelpers.hpp
		TypeDescriptor.hpp
		TypeName.hpp
		TypeRegistry.hpp
		TypeStructures.hpp
		TypeUtils.hpp
)

target_sources(Luma
		PRIVATE
		${REFLECTION_HEADERS}

)