#pragma once

#include "Luma/Utilities/StringUtils.hpp"
#include "TypeName.hpp"
#include "TypeUtils.hpp"

#include <type_traits>
#include <tuple>
#include <array>
#include <variant>
#include <vector>
#include <iostream>
#include <optional>

namespace Luma::Type {

	struct TDummyTag {};

	template<typename T, typename TTag = TDummyTag>
	struct Description;


//#if __cplusplus >= 202002L // C++20
//	template<typename T>
//	concept Described = requires{ Description<T>{}; };
//#else
	template<typename T, typename TTag = TDummyTag>
	using Described = is_specialized<Description<std::remove_cvref_t<T>, TTag>>;
//#endif

	//==============================================================================
	/**
		Utility wrapper to operate on a list of member pointers.
	*/
	template<auto... MemberPointers>
	struct MemberList
	{
		//==============================================================================
		/// Helper type definitions

	public:
		using TTuple = decltype(std::tuple(MemberPointers...));

	private:
		template<size_t MemberIndex>
		using TMemberType = typename member_pointer::return_type<std::remove_cvref_t<decltype(std::get<MemberIndex>(TTuple()))>>::type;

		template<typename TMemberPtr>
		using TMemberPtrType = typename member_pointer::return_type<std::remove_cvref_t<TMemberPtr>>::type;

	public:
		using TVariant = std::variant<filter_void_t<TMemberPtrType<decltype(MemberPointers)>>...>;

	public:
		static constexpr size_t Count() { return sizeof ... (MemberPointers); }

		//==============================================================================
		/// Call function on member list, or for each member

		/** Apply a function to variadic pack of the member list. */
		template<typename TObj, typename TFunc>
		static constexpr auto Apply(TFunc func, TObj& obj)
		{
			return func(obj.*MemberPointers...);
		}

		/** Apply a function to variadic pack of the member list. */
		template<typename TObj, typename TFunc>
		static constexpr auto Apply(TFunc func, const TObj& obj)
		{
			return func(obj.*MemberPointers...);
		}

		/** Apply a function to each member that's not a member function. */
		template<typename TObj, typename TFunc>
		static constexpr auto ApplyForEach(TFunc func, TObj& obj)
		{
			return (ApplyIfMemberNotFunction(func, MemberPointers, obj), ...);
		}

		/** Apply function to default initialized variables for each member type.
			This version does not require instance of the object of the described type.
			@param f		- function to apply for each member type
			@return		function return type
		*/
		template<typename TFunc>
		static constexpr auto ApplyToStaticType(TFunc f)
		{
			return f(MemberPointers...);
		}

	private:
		template<typename TFunc, typename TMemberPtr, typename TObj>
		static constexpr auto ApplyIfMemberNotFunction(TFunc func, TMemberPtr member, TObj& obj)
		{
			if constexpr (!std::is_member_function_pointer_v<decltype(member)>)
				func(obj.*member);
		}

		template<typename TObj, typename TFunc>
		static constexpr auto ApplyToMember(size_t memberIndex, TFunc&& f, TObj&& obj)
		{
			int memberCounter = 0;

			// Utility to construct the 'unwrap' function and increment
			// the counter within the variadic unpack.
			auto unwrapWithCounter = [&memberCounter, &f, memberIndex]()
			{
				auto unwrap = [memberCounter, &f, memberIndex](auto& memb)
				{
					if (memberCounter == memberIndex)
						f(memb);
				};

				memberCounter++;

				return unwrap;
			};

			(ApplyIfMemberNotFunction(unwrapWithCounter(), MemberPointers, std::forward<TObj>(obj)), ...);
		}

		template<size_t MemberIndex, typename TObj, typename TFunc>
		constexpr static auto ApplyToMember(TFunc&& f, TObj&& obj)
		{
			f(obj.*Type::nth_element<MemberIndex>(MemberPointers...));
		}

	public:
		//==============================================================================
		/// Set member values

		template<typename TValue, typename TObj>
		static constexpr bool SetMemberValue(size_t memberIndex, const TValue& value, TObj&& Obj)
		{
			bool valueSet = false;

			ApplyToMember(memberIndex,
				[&](auto& memb)
			{
				//? DBG
				//const char* idM = typeid(decltype(memb)).name();
				//const char* idV = typeid(decltype(value)).name();

				using TMemberNoCVR = std::remove_cvref_t<decltype(memb)>;
				using TValueNoCVR = std::remove_cvref_t<decltype(value)>;

				if constexpr (std::is_same_v<TValueNoCVR, TMemberNoCVR>) // SFINAE
				{
					memb = std::forward<decltype(value)>(value);
					valueSet = true;
				}
			}, std::forward<decltype(Obj)>(Obj));

			return valueSet;
		}

		template<size_t MemberIndex, typename TValue, typename TObj>
		static constexpr bool SetMemberValue(const TValue& value, TObj&& Obj)
		{
			bool valueSet = false;

			ApplyToMember<MemberIndex>(
				[&](auto& memb)
			{
				//? DBG
				//const char* idM = typeid(decltype(memb)).name();
				//const char* idV = typeid(decltype(value)).name();

				using TMemberNoCVR = std::remove_cvref_t<decltype(memb)>;
				using TValueNoCVR = std::remove_cvref_t<decltype(value)>;

				if constexpr (std::is_same_v<TValueNoCVR, TMemberNoCVR>) // SFINAE
				{
					memb = std::forward<decltype(value)>(value);
					valueSet = true;
				}
			}, std::forward<decltype(Obj)>(Obj));

			return valueSet;
		}

		//==============================================================================
		/// Get member values

		template<size_t MemberIndex, typename TObj>
		static constexpr auto GetMemberValue(const TObj& obj)
		{
			static_assert(Count() > MemberIndex);

			auto filter = [&obj](auto member)
			{
				if constexpr (std::is_member_function_pointer_v<decltype(member)>)
					return;
				else
					return obj.*member;
			};
			return filter(Type::nth_element<MemberIndex>(MemberPointers...));
		}

		template<typename TValue, typename TObj>
		static constexpr std::optional<TValue> GetMemberValueOfType(size_t memberIndex, const TObj& Obj)
		{
			std::optional<TValue> value{};

			if (Count() > memberIndex)
			{
				ApplyToMember(memberIndex,
					[&](const auto& memb)
				{
					using TMember = std::remove_cvref_t<decltype(memb)>;
					//const char* typeID = typeid(TValue).name();
					//const char* typeID2 = typeid(TMember).name();

					if constexpr (std::is_same_v<TValue, TMember>) // SFINAE
					{
						value = memb;
					}
				}, std::forward<decltype(Obj)>(Obj));
			}

			return value;
		}

		template<size_t MemberIndex, typename TVariantType, typename TObj>
		static constexpr TVariantType GetMemberValue(const TObj& obj)
		{
			return TVariantType(GetMemberValue<MemberIndex>(obj));
		}

		template<typename TVariantType, typename TObj>
		static constexpr TVariantType GetMemberValue(size_t memberIndex, const TObj& obj)
		{
			TVariantType variant{};
			if (Count() > memberIndex)
			{
				ApplyToMember(memberIndex,
							  [&](const auto& memb)
				{
					//using TMember = std::remove_cvref_t<decltype(memb)>;
					//const char* typeID = typeid(TValue).name();
					//const char* typeID2 = typeid(TMember).name();

					variant = TVariantType(memb);
				}, std::forward<decltype(obj)>(obj));
			}

			return variant;
		}

		//==============================================================================
		/// Get pointer to member value

		template<size_t MemberIndex, typename TObj>
		static constexpr auto* GetMemberValuePtr(TObj& obj)
		{
			static_assert(Count() > MemberIndex);

			auto filter = [&obj](auto member)
			{
				if constexpr (std::is_member_function_pointer_v<decltype(member)>)
					return;
				else
					return &obj.*member;
			};
			return filter(Type::nth_element<MemberIndex>(MemberPointers...));
		}

		template<typename TValue, typename TObj>
		static constexpr TValue* GetMemberValuePtrOfType(size_t memberIndex, TObj&& Obj)
		{
			TValue* value = nullptr;

			if (Count() > memberIndex)
			{
				ApplyToMember(memberIndex,
					[&](auto& memb)
					{
						using TMember = std::remove_cvref_t<decltype(memb)>;
						//const char* typeID = typeid(TValue).name();
						//const char* typeID2 = typeid(TMember).name();

						if constexpr (std::is_same_v<TValue, TMember>) // SFINAE
						{
							value = std::addressof(memb);
						}
					}, std::forward<decltype(Obj)>(Obj));
			}

			return value;
		}

		//==============================================================================
		/// Query type information

		template<size_t MemberIndex>
		static constexpr auto IsFunction()
		{
			static_assert(Count() > MemberIndex);
			return std::is_member_function_pointer_v<decltype(Type::nth_element<MemberIndex>(MemberPointers...))>;
		}

		static constexpr std::optional<bool> IsFunction(size_t memberIndex)
		{
			std::optional<bool> isFunction;

			if (Count() > memberIndex)
			{
				int memberCounter = 0;
				auto unwrap = [&isFunction, memberIndex](auto memb, int counter)
				{
					if (counter == memberIndex)
					{
						isFunction = std::is_member_function_pointer_v<decltype(memb)>;
					}
				};

				(unwrap(MemberPointers, memberCounter++), ...);
			}

			return isFunction;
		}

		template<size_t MemberIndex>
		static constexpr auto GetMemberSize()
		{
			return sizeof(TMemberType<MemberIndex>);
		}

		static constexpr auto GetMemberSize(size_t memberIndex)
		{
			std::optional<size_t> size;

			if (Count() > memberIndex)
			{
				int memberCounter = 0;
				auto unwrap = [&size, memberIndex](auto memb, int counter)
				{
					if (counter == memberIndex)
					{
						size = sizeof(filter_void_t<TMemberPtrType<decltype(memb)>>);
					}
				};

				(unwrap(MemberPointers, memberCounter++), ...);
			}

			return size;
		}

		template<size_t MemberIndex>
		static constexpr auto GetTypeName()
		{
			static_assert(Count() > MemberIndex);
			return type::type_name<TMemberType<MemberIndex>>();
		}

		static constexpr auto GetTypeName(size_t memberIndex)
		{
			std::optional<std::string_view> name;

			if (Count() > memberIndex)
			{
				int memberCounter = 0;
				auto unwrap = [&name, memberIndex](auto memb, int counter)
				{
					if (counter == memberIndex)
					{
						name = type::type_name<TMemberPtrType<decltype(memb)>>();
					}
				};

				(unwrap(MemberPointers, memberCounter++), ...);
			}

			return name;
		}
	};

	template<class TDescription, class TObjType, class TTag, class TList>
	struct DescriptionInterface;

#ifndef DESCRIBED
	//==============================================================================
	/**
		Description provides reflection interface with access by strings.
	 */
#define DESCRIBED_TAGGED(Class, Tag, ...) template<>								\
struct Luma::Type::Description<Class, Tag> : Luma::Type::MemberList<__VA_ARGS__>, Luma::Type::DescriptionInterface<Luma::Type::Description<Class,Tag>, Class, Tag, Luma::Type::MemberList<__VA_ARGS__>>	\
{																		\
private:																\
	static constexpr std::string_view MemberStr{ #__VA_ARGS__ };		\
	static constexpr std::string_view ClassStr{ #Class };				\
	static constexpr std::string_view Delimiter{ ", " };				\
																		\
public:																	\
	static constexpr std::string_view Namespace{ ClassStr.data(),													\
													ClassStr.find("::") == std::string_view::npos					\
														? 0															\
														: ClassStr.find_last_of(':') - 1 };							\
																													\
	static constexpr std::string_view ClassName{ ClassStr.data() + (Namespace.size() ? Namespace.size() + 2 : 0) };	\
																													\
	static constexpr std::array<std::string_view, NumberOfMembers> MemberNames										\
		= Luma::Utils::RemoveNamespace<NumberOfMembers>(																	\
			Luma::Utils::SplitString<NumberOfMembers>(MemberStr, Delimiter));												\
};
#endif // !DESCRIBED


#ifndef DESCRIBED
#define DESCRIBED(Class, ...) DESCRIBED_TAGGED(Class, Luma::Type::TDummyTag, __VA_ARGS__)
#endif

	 //==============================================================================
	 /*
		 A convinience interface to move some stuff out of the macro.
	 */
	template<class TDescription, class TObjType, class TTag, class TList>
	struct DescriptionInterface
	{
		static_assert(std::is_same_v<TDescription, Description<TObjType, TTag>>);

		static constexpr size_t NumberOfMembers = TList::Count();

		static constexpr size_t IndexOf(std::string_view memberName)
		{
			for (size_t i = 0; i < TDescription::Count(); ++i)
			{
				if (TDescription::MemberNames[i] == memberName)
					return i;
			}

			return TDescription::INVALID_INDEX;
		}

		static constexpr bool HasMember(std::string_view memberName)
		{
			for (const auto& name : TDescription::MemberNames)
			{
				if (name == memberName)
					return true;
			}
			return false;
		}

		static constexpr size_t INVALID_INDEX = ~size_t(0);

		static constexpr std::optional<std::string_view> GetMemberName(size_t memberIndex)
		{
			return (NumberOfMembers > memberIndex) ? std::optional<std::string_view>(TDescription::MemberNames[memberIndex]) : std::nullopt;
		}

		template<size_t MemberIndex>
		static constexpr std::string_view GetMemberName()
		{
			static_assert(NumberOfMembers > MemberIndex);
			return TDescription::MemberNames[MemberIndex];
		}

		template<typename TValue>
		static constexpr std::optional<TValue> GetMemberValueByName(std::string_view memberName, const TObjType& object)
		{
			const auto index = IndexOf(memberName);
			return (NumberOfMembers > index) ? TList::template GetMemberValueOfType<TValue>(index, object) : std::nullopt;
		}

		template<typename TVariantType>
		static constexpr TVariantType GetMemberValueVariantByName(std::string_view memberName, const TObjType& object)
		{
			const auto index = IndexOf(memberName);
			return (NumberOfMembers > index) ? TList::template GetMemberValue<TVariantType>(index, object) : TVariantType{};
		}

		template<typename TValue>
		static constexpr TValue* GetMemberValuePtrByName(std::string_view memberName, TObjType& object)
		{
			const auto index = IndexOf(memberName);
			return (NumberOfMembers > index) ? TList::template GetMemberValuePtrOfType<TValue>(index, object) : nullptr;
		}

		template<typename TValue>
		static constexpr bool SetMemberValueByName(std::string_view memberName, const TValue& value, TObjType& object)
		{
			const auto index = IndexOf(memberName);
			return (NumberOfMembers > index) ? TList::template SetMemberValue<TValue>(index, value, object) : false;
		}

		static constexpr std::optional<bool> IsFunctionByName(std::string_view memberName)
		{
			const auto index = IndexOf(memberName);
			return (NumberOfMembers > index) ? TList::IsFunction(index) : std::nullopt;
		}

		static constexpr std::optional<size_t> GetMemberSizeByName(std::string_view memberName)
		{
			const auto index = IndexOf(memberName);
			return (NumberOfMembers > index) ? TList::GetMemberSize(index) : std::nullopt;
		}

		static constexpr std::optional<std::string_view> GetTypeNameByName(std::string_view memberName)
		{
			const auto index = IndexOf(memberName);
			return (NumberOfMembers > index) ? std::optional<std::string_view>(TList::GetTypeName(index)) : std::nullopt;
		}

		/** Apply a function to each member that's not a member function and get name of the member as function argument. */
		template<typename TObj, typename TFunc>
		static constexpr auto ApplyForEachWithName(TFunc func, TObj& obj)
		{
			auto unwrapOuter = [&func, &obj](auto&... members)
			{
				uint32_t memberIndex = 0;
				auto unwrap = [&func]<typename TMember>(TMember& member, uint32_t index)
				{
					if constexpr (!std::is_member_function_pointer_v<TMember>)
						func(member, TDescription::MemberNames[index]);
				};
				(unwrap(members, memberIndex++), ...);
			};

			return TDescription::Apply(unwrapOuter, obj);
		}

		template<typename TObj, typename TValue>
		static size_t GetMemberIndex(TObj& obj, TValue* pointerToMember)
		{
			auto unwrapOuter = [&](auto&... members)
			{
				size_t returnIndex = TDescription::INVALID_INDEX;

				uint32_t memberIndex = 0;
				auto unwrap = [&]<typename TMember>(TMember& member, uint32_t index)
				{
					if constexpr (!std::is_member_function_pointer_v<TMember>)
					{
						if (pointerToMember == &member)
							returnIndex = memberIndex;
					}
				};
				(unwrap(members, memberIndex++), ...);

				return returnIndex;
			};

			return TDescription::Apply(unwrapOuter, obj);
		}

		template<class TObj, class TFunction>
		static bool FindOwnerOfPtrAndApply(TObj& obj, void* pointerToMember, TFunction func)
		{
			// Find child of obj that owns the member and apply function to its members

			auto unwrapOuter = [pointerToMember, &func](auto&... members)
			{
				bool ownerFound = false;
				uint32_t memberIndex = 0;
				auto unwrap = [&]<typename TMember>(TMember & member, uint32_t index)
				{
					if constexpr (!std::is_member_function_pointer_v<TMember>)
					{
						if (pointerToMember == &member)
						{
							TDescription::ApplyForEach(func, member);
							ownerFound = true;
						}
						else
						{
							if constexpr (Type::Described<TMember>::value)
							{
								if (pointerToMember == &member)
								{
									Type::Description<TMember>::ApplyForEach(func, member);
									ownerFound = true;
								}
							}
						}
					}

				};
				(unwrap(members, memberIndex++), ...);

				return ownerFound;
			};

			return TDescription::Apply(unwrapOuter, obj);
		}

		// Use pointerToMember to find member of the original object and call function on the same member of other object
		template<class TObj, class TFunction>
		static bool FindOwnerOfPtrAndApplyToOther(TObj& obj, void* pointerToMember, TObj& other, TFunction func)
		{
			// Find child of obj that owns the member and apply function to its members
			auto unwrapOuter = [&](auto&... members)
			{
				bool ownerFound = false;
				uint32_t memberIndex = 0;
				auto unwrap = [&]<typename TMember>(TMember& member, uint32_t index)
				{
					if constexpr (!std::is_member_function_pointer_v<TMember>)
					{
						if (pointerToMember == &member)
						{
							TMember* otherMember = TList::template GetMemberValuePtrOfType<TMember>(index, other);

							func(*otherMember);
							ownerFound = true;
						}
						else
						{
							if constexpr (Type::Described<TMember>::value)
							{
								TMember* otherMember = TList::template GetMemberValuePtrOfType<TMember>(index, other);

								ownerFound = Type::Description<TMember>::FindOwnerOfPtrAndApplyToOther(member, pointerToMember, *otherMember, func);
							}
						}
					}

				};
				(unwrap(members, memberIndex++), ...);

				return ownerFound;
			};

			return TDescription::Apply(unwrapOuter, obj);
		}

		static void Print(std::ostream& stream)
		{
			stream << "Class Name: '" << TDescription::ClassName << '\'' << '\n';
			stream << "Namespace: '" << TDescription::Namespace << '\'' << '\n';
			stream << "Number of members: " << NumberOfMembers << '\n';
			stream << "Members:" << '\n';
			stream << "---" << '\n';

			for (int i = 0; i < (int)NumberOfMembers; ++i)
			{
				stream << *TList::GetTypeName(i) << ' ' << TDescription::MemberNames[i] << ' ' << "(" << *TList::GetMemberSize(i) << " bytes)";

				if (*TList::IsFunction(i))
				{
					stream << " (function)";
				}

				stream << '\n';
			}
			stream << "---" << '\n';
		}

		static void Print(std::ostream& stream, const TObjType& obj)
		{
			stream << "Class Name: '" << TDescription::ClassName << '\'' << '\n';
			stream << "Namespace: '" << TDescription::Namespace << '\'' << '\n';
			stream << "Number of members: " << NumberOfMembers << '\n';
			stream << "Members:" << '\n';
			stream << "---" << '\n';

			auto unwrapOuter = [&stream, &obj](auto... members)
			{
				int memberCounter = 0;
				auto unwrap = [&stream, &obj](auto memb, int index)
				{
					stream << *TList::GetTypeName(index) << ' ';
					stream << TDescription::MemberNames[index];

					if constexpr (!std::is_member_function_pointer_v<decltype(memb)> && is_streamable_v<typename member_pointer::return_type<decltype(memb)>::type>)
					{
						stream << "{ " << (obj.*memb) << " }";
					}

					stream << ' ';

					stream << "(" << *TList::GetMemberSize(index) << " bytes)";

					if (*TList::IsFunction(index))
					{
						stream << " (function)";
					}

					stream << '\n';
				};
				(unwrap(members, memberCounter++), ...);

			};
			TList::ApplyToStaticType(unwrapOuter);

			stream << "---" << '\n';
		}
	};

	template<typename T, typename... Ts>
	std::ostream& operator<<(std::ostream& os, const std::variant<T, Ts...>& v)
	{
		std::visit([&os](auto&& arg)
		{
			os << arg;
		}, v);
		return os;
	}
} // namespace Luma::Type