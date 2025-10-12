#include <catch2/catch_test_macros.hpp>

#include "Luma/Reflection/TypeStructures.hpp"

#include <algorithm>

using namespace Luma;

struct TestStruct
{
	int i = 5;
	float f = 3.2f;
	char ch = 'c';
	int* pi = nullptr;

	inline void vfunc() {}
	inline bool bfunc() const { return true; }
};

DESCRIBED(TestStruct,
	&TestStruct::i,
	&TestStruct::f,
	&TestStruct::ch,
	&TestStruct::pi,
	&TestStruct::vfunc,
	&TestStruct::bfunc
);

TEST_CASE("ClassInfo generation", "[unit][reflection]")
{
	auto info = Reflection::ClassInfo::Of<TestStruct>();

	REQUIRE(info.Name == "TestStruct");
	REQUIRE(info.Size == sizeof(TestStruct));
	REQUIRE(info.Members.size() == 6);
}

TEST_CASE("Data member reflection", "[unit][reflection]")
{
	auto info = Reflection::ClassInfo::Of<TestStruct>();

	auto findMember = [&](const std::string& name) {
		return std::find_if(info.Members.begin(), info.Members.end(),
			[&](const auto& member) { return member.Name == name; });
	};

	SECTION("Integer member")
	{
		auto intMember = findMember("i");
		REQUIRE(intMember != info.Members.end());
		REQUIRE(intMember->TypeName == "int");
		REQUIRE(intMember->Size == sizeof(int));
		REQUIRE(intMember->Type == Reflection::Member::Data);
	}

	SECTION("Float member")
	{
		auto floatMember = findMember("f");
		REQUIRE(floatMember != info.Members.end());
		REQUIRE(floatMember->TypeName == "float");
		REQUIRE(floatMember->Size == sizeof(float));
		REQUIRE(floatMember->Type == Reflection::Member::Data);
	}

	SECTION("Pointer member")
	{
		auto ptrMember = findMember("pi");
		REQUIRE(ptrMember != info.Members.end());
		REQUIRE(ptrMember->TypeName == "int*");
		REQUIRE(ptrMember->Size == sizeof(int*));
		REQUIRE(ptrMember->Type == Reflection::Member::Data);
	}
}

TEST_CASE("Function member reflection", "[unit][reflection]")
{
	auto info = Reflection::ClassInfo::Of<TestStruct>();

	auto findMember = [&](const std::string& name) {
		return std::find_if(info.Members.begin(), info.Members.end(),
			[&](const auto& member) { return member.Name == name; });
	};

	SECTION("Void function")
	{
		auto vfuncMember = findMember("vfunc");
		REQUIRE(vfuncMember != info.Members.end());
		REQUIRE(vfuncMember->TypeName == "void");
		REQUIRE(vfuncMember->Size == 0);
		REQUIRE(vfuncMember->Type == Reflection::Member::Function);
	}

	SECTION("Bool function")
	{
		auto bfuncMember = findMember("bfunc");
		REQUIRE(bfuncMember != info.Members.end());
		REQUIRE(bfuncMember->TypeName == "bool");
		REQUIRE(bfuncMember->Size == 0);
		REQUIRE(bfuncMember->Type == Reflection::Member::Function);
	}
}

TEST_CASE("ClassInfo equality", "[unit][reflection]")
{
	auto info1 = Reflection::ClassInfo::Of<TestStruct>();
	auto info2 = Reflection::ClassInfo::Of<TestStruct>();

	REQUIRE(info1 == info2);
}