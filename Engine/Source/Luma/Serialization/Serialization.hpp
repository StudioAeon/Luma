#pragma once

#include "Luma/Core/Identifier.hpp"
#include "Luma/Core/UUID.hpp"
#include "Luma/Reflection/MetaHelpers.hpp"
#include "Luma/Reflection/TypeDescriptor.hpp"
#include "Luma/Serialization/StreamWriter.hpp"
#include "Luma/Serialization/StreamReader.hpp"

#include <choc/containers/choc_Value.h>
#include <yaml-cpp/yaml.h>

//=============================================================================
/**  Non-intrusive Binary / Memory Serialization Interface.

	Add SERIALIZABLE macro in a header for class you need to be serialized
	member by member. For example:

	SERIALIZABLE(MyClass,
				&MyClass::MemberPrimitive,
				&MyClass::MemberVector,
				&MyClass::MemberAnotherSerializable);

	If there's no specialization for the ..Impl (de)serialize function for any
	of the listed in the macro members, it's going to call:
	
		T::Serialize(StreamWriter* writer, T& obj)
	..or
		T::Deserialize(StreamReader* reader, T& obj)

	..where T is the type of the object that need to have Serialize and Deserialize
	functions with this signature.

	Instead of adding intrusive interface as described above you can add to your
	header specialization for the free template functions as follows:

	namespace Luma::Serialization::Impl
	{
		template<>
		static inline bool SerializeImpl(StreamWriter* writer, const MyClass& v)
		{
			... some custom serialization routine ...
			writer->WriteRaw(42069);
			return true;
		}

		template<>
		static inline bool DeserializeImpl(StreamReader* reader, MyClass& v)
		{
			... some custom deserialization routine ...
			reader->ReadRaw(v.MyVariable);
			return true;
		}
	} // namespace Luma::Spatialization::Impl
*/
#define SERIALIZABLE(Class, ...) template<>\
struct Luma::Type::Description<Class> : Luma::Type::MemberList<__VA_ARGS__> {};\

namespace Luma::Serialization
{

	// Helper struct for choc::value serialization
	struct ValueSerializer
	{
		std::vector<uint8_t> Data;
		void write(const void* data, size_t size)
		{
			Data.insert(Data.end(), (const uint8_t*)data, (const uint8_t*)data + size);
		}
	};


	//=============================================================================
	/// Pre-computing size of serialized data (e.g. so if serializing to a memory buffer, we can pre-allocate the buffer)
	template<typename... Ts>
	static size_t SerializedSize(const Ts&...vs);

	namespace Impl {
		template<typename T>
		static size_t SerializedSizeImpl(const T& v)
		{
			if constexpr (std::is_trivial<T>::value)
				return sizeof(T);
			else
				return T::SerializedSize(v);
		}

		//=============================================================================
		/** SerializedSize specialization
			More default specializations can be added...
		*/

		template<>
		LM_EXPLICIT_STATIC inline size_t SerializedSizeImpl(const UUID& v)
		{
			return sizeof(uint64_t);
		}

		template<>
		LM_EXPLICIT_STATIC inline size_t SerializedSizeImpl(const Identifier& v)
		{
			return sizeof(uint32_t);
		}

		template<>
		LM_EXPLICIT_STATIC inline size_t SerializedSizeImpl(const std::string& v)
		{
			return sizeof(size_t) + v.size();
		}

		template<>
		LM_EXPLICIT_STATIC inline size_t SerializedSizeImpl(const choc::value::Value& v)
		{
			struct ValueSerializer wrapper;
			v.serialise(wrapper);
			return sizeof(uint32_t) + wrapper.Data.size();
		}

		//=============================================================================
		template<typename T>
		LM_EXPLICIT_STATIC size_t SerializedSizeVec(const std::vector<T>& vec)
		{
			size_t size = sizeof(uint32_t);

			for (const auto& element : vec)
				size += SerializedSize(element);

			return size;
		}

		//=============================================================================
		template<typename T>
		LM_EXPLICIT_STATIC size_t SerializedSizeByType(const T& v)
		{
			if constexpr (Type::Described<T>::value)
			{
				return Type::Description<T>::Apply(
						[](const auto&... members)
				{
					return (SerializedSize(members) + ...);
				}
				, v);
			}
			else if constexpr (Type::is_array_v<T>)
			{
				return SerializedSizeVec(v);
			}
			else
			{
				return SerializedSizeImpl(v);
			}
		}
	} // namespace Impl

	template<typename... Ts>
	static size_t SerializedSize(const Ts&...vs)
	{
		return (Impl::SerializedSizeByType(vs) + ...);
	}


	//=============================================================================
	/// Serialization
	template<typename... Ts>
	static bool Serialize(StreamWriter* writer, const Ts&...vs);

	namespace Impl
	{
		template<typename T>
		static bool SerializeImpl(StreamWriter* writer, const T& v)
		{
			if constexpr (std::is_trivial<T>::value)
				writer->WriteRaw(v);
			else
				T::Serialize(writer, v);

			return true;
		}
		
		//=============================================================================
		/** Serialization specialization
			More default specializations can be added...
		*/

		template<>
		LM_EXPLICIT_STATIC inline bool SerializeImpl(StreamWriter* writer, const UUID& v)
		{
			writer->WriteRaw((uint64_t)v);
			return true;
		}

		template<>
		LM_EXPLICIT_STATIC inline bool SerializeImpl(StreamWriter* writer, const Identifier& v)
		{
			writer->WriteRaw((uint32_t)v);
			return true;
		}

		template<>
		LM_EXPLICIT_STATIC inline bool SerializeImpl(StreamWriter* writer, const std::string& v)
		{
			writer->WriteString(v);
			return true;
		}

		template<>
		LM_EXPLICIT_STATIC inline bool SerializeImpl(Luma::StreamWriter* writer, const choc::value::Value& v)
		{
			struct ValueSerializer wrapper;
			v.serialise(wrapper);

			writer->WriteArray(wrapper.Data);
			return true;
		}

		//=============================================================================
		template<typename T>
		LM_EXPLICIT_STATIC bool SerializeVec(StreamWriter* writer, const std::vector<T>& vec)
		{
			//if (writeSize)
			writer->WriteRaw<uint32_t>((uint32_t)vec.size());

			for (const auto& element : vec)
				Serialize(writer, element);

			return true;
		}

		//=============================================================================
		template<typename T>
		LM_EXPLICIT_STATIC bool SerializeByType(StreamWriter* writer, const T& v)
		{
			if constexpr (Type::Described<T>::value)
			{
				return Type::Description<T>::Apply(
						[&writer](const auto&... members)
				{
					return Serialize(writer, members...);
				}
				, v);
			}
			else if constexpr (Type::is_array_v<T>)
			{
				return SerializeVec(writer, v);
			}
			else
			{
				return SerializeImpl(writer, v);
			}
		}
	} // namespace Impl

	template<typename... Ts>
	static bool Serialize(StreamWriter* writer, const Ts&...vs)
	{
		return (Impl::SerializeByType(writer, vs) && ...);
	}


	//=============================================================================
	/// Deserialization
	template<typename... Ts>
	static bool Deserialize(StreamReader* reader, Ts&...vs);

	namespace Impl
	{
		template<typename T>
		static bool DeserializeImpl(StreamReader* reader, T& v)
		{
			if constexpr (std::is_trivial<T>::value)
				reader->ReadRaw(v);
			else
				T::Deserialize(reader, v);

			return true;
		}

		//=============================================================================
		/** Deserialization specialization
			More default specializations can be added...
		*/

		template<>
		LM_EXPLICIT_STATIC inline bool DeserializeImpl(StreamReader* reader, UUID& v)
		{
			reader->ReadRaw(v);
			return true;
		}

		template<>
		LM_EXPLICIT_STATIC inline bool DeserializeImpl(StreamReader* reader, Identifier& v)
		{
			uint32_t data;
			reader->ReadRaw(data);
			v = data;
			return true;
		}

		template<>
		LM_EXPLICIT_STATIC inline bool DeserializeImpl(StreamReader* reader, std::string& v)
		{
			reader->ReadString(v);
			return true;
		}

		template<>
		LM_EXPLICIT_STATIC inline bool DeserializeImpl(Luma::StreamReader* reader, choc::value::Value& v)
		{
			std::vector<uint8_t> data;
			reader->ReadArray(data);
			choc::value::InputData inputData{ data.data(), data.data() + data.size() };
			v = choc::value::Value::deserialise(inputData);
			return true;
		}

		//=============================================================================
		template<typename T>
		LM_EXPLICIT_STATIC bool DeserializeVec(StreamReader* reader, std::vector<T>&vec)
		{
			uint32_t size = 0;
			//if (size == 0)
			reader->ReadRaw<uint32_t>(size);

			vec.resize(size);

			for (uint32_t i = 0; i < size; i++)
				Deserialize(reader, vec[i]);

			return true;
		}

		//=============================================================================
		template<typename T>
		LM_EXPLICIT_STATIC bool DeserializeByType(StreamReader* reader, T& v)
		{
			if constexpr (Type::Described<T>::value)
			{
				return Type::Description<T>::Apply(
						[&reader](auto&... members)
				{
					return Deserialize(reader, members...);
				}
				, v);
			}
			else if constexpr (Type::is_array_v<T>)
			{
				return DeserializeVec(reader, v);
			}
			else
			{
				return DeserializeImpl(reader, v);
			}
		}
	} // namespace Impl

	template<typename... Ts>
	static bool Deserialize(StreamReader* reader, Ts&... vs)
	{
		return (Impl::DeserializeByType(reader, vs) && ...);
	}

	//==============================================================================
	/// Generic functions to Serialize and Deserialize any Described type to/from YAML

	enum EYAMLOptions
	{
		None = 0,
		WrapInAMap	// This option should be used when we serialize into Emitter that doesn't have the outer map.
	};

	// TODO: Versioning! (potentially add versioning directly to Type::Descriptor)

	template<class TDescribedType>
	bool Serialize(const TDescribedType& parameters, YAML::Emitter& out, EYAMLOptions opeions = EYAMLOptions::None) requires (Luma::Type::Described<TDescribedType>::value);

	template<class TDescribedType>
	bool Deserialize(TDescribedType& parameters, const YAML::Node& nodeOuter) requires (Luma::Type::Described<TDescribedType>::value);

	//==============================================================================
	//
	//   Code beyond this point is implementation detail...
	//
	//==============================================================================
} // namespace Luma::Serialization