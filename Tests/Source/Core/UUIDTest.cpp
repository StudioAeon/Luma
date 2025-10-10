#include <catch2/catch_test_macros.hpp>

#include "Luma/Core/UUID.hpp"

#include <algorithm>
#include <array>
#include <unordered_map>
#include <unordered_set>

using namespace Luma;

TEST_CASE("UUID default construction", "[unit][core][uuid]")
{
	SECTION("Default constructor generates random UUIDs")
	{
		UUID uuid1;
		UUID uuid2;

		uint64_t value1 = uuid1;
		uint64_t value2 = uuid2;

		REQUIRE(value1 != value2);

		REQUIRE(value1 != 0);
		REQUIRE(value2 != 0);
	}

	SECTION("Multiple random UUIDs are unique")
	{
		constexpr size_t NUM_UUIDS = 1000;
		std::unordered_set<uint64_t> uniqueValues;

		for (size_t i = 0; i < NUM_UUIDS; ++i)
		{
			UUID uuid;
			uint64_t value = uuid;

			REQUIRE(uniqueValues.find(value) == uniqueValues.end());
			uniqueValues.insert(value);
		}

		REQUIRE(uniqueValues.size() == NUM_UUIDS);
	}
}

TEST_CASE("UUID explicit construction", "[unit][core][uuid]")
{
	SECTION("Constructor with specific value")
	{
		constexpr uint64_t testValue = 0x123456789ABCDEF0ULL;
		UUID uuid(testValue);

		uint64_t retrievedValue = uuid;
		REQUIRE(retrievedValue == testValue);
	}

	SECTION("Constructor with zero value")
	{
		UUID uuid(0);
		uint64_t value = uuid;
		REQUIRE(value == 0);
	}

	SECTION("Constructor with maximum value")
	{
		constexpr uint64_t maxValue = UINT64_MAX;
		UUID uuid(maxValue);
		uint64_t value = uuid;
		REQUIRE(value == maxValue);
	}
}

TEST_CASE("UUID copy semantics", "[unit][core][uuid]")
{
	SECTION("Copy constructor")
	{
		constexpr uint64_t testValue = 0xDEADBEEFCAFEBABEULL;
		UUID original(testValue);
		UUID copy(original);

		uint64_t originalValue = original;
		uint64_t copyValue = copy;

		REQUIRE(originalValue == copyValue);
		REQUIRE(originalValue == testValue);
	}

	SECTION("Copy constructor with random UUID")
	{
		UUID original;
		UUID copy(original);

		uint64_t originalValue = original;
		uint64_t copyValue = copy;

		REQUIRE(originalValue == copyValue);
	}

	SECTION("Assignment operator (implicit)")
	{
		constexpr uint64_t testValue = 0x0123456789ABCDEFULL;
		UUID original(testValue);
		UUID assigned;

		uint64_t initialAssignedValue = assigned;

		assigned = original;
		uint64_t assignedValue = assigned;

		REQUIRE(assignedValue == testValue);
		REQUIRE(assignedValue != initialAssignedValue);
	}
}

TEST_CASE("UUID conversion operators", "[unit][core][uuid]")
{
	SECTION("Implicit conversion to uint64_t")
	{
		constexpr uint64_t testValue = 0xFEDCBA9876543210ULL;
		UUID uuid(testValue);

		uint64_t value1 = uuid;
		REQUIRE(value1 == testValue);

		const UUID constUuid(testValue);
		uint64_t value2 = constUuid;
		REQUIRE(value2 == testValue);
	}

	SECTION("Use in arithmetic operations")
	{
		UUID uuid(100);
		uint64_t result = uuid + 50;
		REQUIRE(result == 150);
	}

	SECTION("Use in comparisons")
	{
		UUID uuid1(100);
		UUID uuid2(200);

		REQUIRE(uuid1 < uuid2);
		REQUIRE(uuid2 > uuid1);
		REQUIRE(uuid1 == 100);
		REQUIRE(uuid2 == 200);
	}
}

TEST_CASE("UUID hash support", "[unit][core][uuid]")
{
	SECTION("std::hash specialization works")
	{
		constexpr uint64_t testValue = 0x1234567890ABCDEFULL;
		UUID uuid(testValue);

		std::hash<UUID> hasher;
		std::hash<uint64_t> uint64Hasher;

		size_t uuidHash = hasher(uuid);
		size_t uint64Hash = uint64Hasher(testValue);

		REQUIRE(uuidHash == uint64Hash);
	}

	SECTION("Use in std::unordered_set")
	{
		std::unordered_set<UUID> uuidSet;

		UUID uuid1(100);
		UUID uuid2(200);
		UUID uuid3(100);

		uuidSet.insert(uuid1);
		uuidSet.insert(uuid2);
		uuidSet.insert(uuid3);

		REQUIRE(uuidSet.size() == 2);
		REQUIRE(uuidSet.find(uuid1) != uuidSet.end());
		REQUIRE(uuidSet.find(uuid2) != uuidSet.end());
		REQUIRE(uuidSet.find(uuid3) != uuidSet.end());
	}

	SECTION("Use in std::unordered_map")
	{
		std::unordered_map<UUID, std::string> uuidMap;

		UUID uuid1(42);
		UUID uuid2(84);

		uuidMap[uuid1] = "Answer";
		uuidMap[uuid2] = "Double Answer";

		REQUIRE(uuidMap.size() == 2);
		REQUIRE(uuidMap[uuid1] == "Answer");
		REQUIRE(uuidMap[uuid2] == "Double Answer");

		auto it = uuidMap.find(uuid1);
		REQUIRE(it != uuidMap.end());
		REQUIRE(it->second == "Answer");
	}
}

TEST_CASE("UUID consistency", "[unit][core][uuid]")
{
	SECTION("Same input produces same UUID")
	{
		constexpr uint64_t testValue = 0xABCDEF0123456789ULL;

		UUID uuid1(testValue);
		UUID uuid2(testValue);

		REQUIRE(uuid1 == testValue);
		REQUIRE(uuid2 == testValue);
	}

	SECTION("Zero is handled correctly")
	{
		UUID uuid(0);
		REQUIRE(uuid == 0);

		std::hash<UUID> hasher;
		REQUIRE(hasher(uuid) == std::hash<uint64_t>{}(0));
	}
}

TEST_CASE("UUID edge cases", "[unit][core][uuid]")
{
	SECTION("Maximum value handling")
	{
		UUID uuid(UINT64_MAX);
		REQUIRE(uuid == UINT64_MAX);

		std::hash<UUID> hasher;
		REQUIRE(hasher(uuid) == std::hash<uint64_t>{}(UINT64_MAX));
	}

	SECTION("Random distribution sanity check")
	{
		constexpr size_t NUM_SAMPLES = 10000;
		constexpr size_t NUM_BUCKETS = 100;

		std::array<size_t, NUM_BUCKETS> buckets{};

		for (size_t i = 0; i < NUM_SAMPLES; ++i)
		{
			UUID uuid;
			uint64_t value = uuid;
			size_t bucket = value % NUM_BUCKETS;
			buckets[bucket]++;
		}

		size_t expectedPerBucket = NUM_SAMPLES / NUM_BUCKETS;
		size_t tolerance = expectedPerBucket / 2;

		for (size_t count : buckets)
		{
			REQUIRE(count > 0);
			REQUIRE(count < expectedPerBucket * 3);
		}
	}
}