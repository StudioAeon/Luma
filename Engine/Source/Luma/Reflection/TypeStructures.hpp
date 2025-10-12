#pragma once

#include "TypeDescriptor.hpp"

#include <string>
#include <vector>

namespace Luma {
	namespace Reflection {

		struct Member
		{
			std::string Name;
			size_t Size;		// 0 if function

			std::string TypeName;

			enum EType
			{
				Function,
				Data
			} Type;

			bool operator==(const Member& other) const
			{
				return Name == other.Name
					&& Size == other.Size
					&& TypeName == other.TypeName
					&& Type == other.Type;
			}
		};

		struct ClassInfo
		{
			std::string Name;
			size_t Size;					// sizeof(T)

			std::vector<Member> Members;	// "described" members

			template<class T>
			static ClassInfo Of() requires (Type::Described<T>::value) // Type must be 'Described'
			{
				// Parse info using type Description

				ClassInfo info;
				using Descr = Type::Description<T>;

				info.Name = Descr::ClassName;
				info.Size = sizeof(T);

				for (const std::string_view& memberName : Descr::MemberNames)
				{
					const bool isFunction = *Descr::IsFunctionByName(memberName);
					const auto typeName = *Descr::GetTypeNameByName(memberName);

					std::string nameStr(memberName);
					std::string typeStr(typeName.data(), typeName.size());

					info.Members.push_back(
						Member{
							.Name = nameStr,
							.Size = *Descr::GetMemberSizeByName(memberName) * !isFunction,
							.TypeName = typeStr,
							.Type = isFunction ? Member::Function : Member::Data
						});
				}

				return info;
			}

			bool operator==(const ClassInfo& other) const
			{
				return Name == other.Name
					&& Size == other.Size
					&& Members == other.Members;
			}
		};
	} // namespace Reflection

} // namespace Luma