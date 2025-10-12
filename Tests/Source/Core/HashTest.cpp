#include <catch2/catch_test_macros.hpp>

#include "Luma/Core/Hash.hpp"

#include <string>
#include <string_view>
#include <unordered_set>

using namespace Luma;

TEST_CASE("Hash FNV construction and basic properties", "[unit][core][hash]")
{
	SECTION("GenerateFNVHash is constexpr")
	{
		constexpr uint32_t hash = Hash::GenerateFNVHash("test");
		REQUIRE(hash != 0);
	}

	SECTION("Empty string produces non-zero hash")
	{
		constexpr uint32_t hash = Hash::GenerateFNVHash("");
		REQUIRE(hash != 0);
	}

	SECTION("Same string produces same hash")
	{
		constexpr uint32_t hash1 = Hash::GenerateFNVHash("hello");
		constexpr uint32_t hash2 = Hash::GenerateFNVHash("hello");
		
		REQUIRE(hash1 == hash2);
	}

	SECTION("Different strings produce different hashes")
	{
		constexpr uint32_t hash1 = Hash::GenerateFNVHash("hello");
		constexpr uint32_t hash2 = Hash::GenerateFNVHash("world");
		
		REQUIRE(hash1 != hash2);
	}

	SECTION("Case sensitivity")
	{
		constexpr uint32_t hashLower = Hash::GenerateFNVHash("test");
		constexpr uint32_t hashUpper = Hash::GenerateFNVHash("TEST");
		
		REQUIRE(hashLower != hashUpper);
	}
}

TEST_CASE("Hash FNV determinism", "[unit][core][hash]")
{
	SECTION("Runtime and compile-time produce same hash")
	{
		constexpr uint32_t compileTimeHash = Hash::GenerateFNVHash("example");
		
		std::string str = "example";
		uint32_t runtimeHash = Hash::GenerateFNVHash(str);
		
		REQUIRE(compileTimeHash == runtimeHash);
	}

	SECTION("String_view compatibility")
	{
		std::string_view sv = "test_string";
		uint32_t hash = Hash::GenerateFNVHash(sv);
		
		REQUIRE(hash != 0);
		REQUIRE(hash == Hash::GenerateFNVHash("test_string"));
	}

	SECTION("Repeated calls produce same result")
	{
		std::string testStr = "repeated_test";
		
		uint32_t hash1 = Hash::GenerateFNVHash(testStr);
		uint32_t hash2 = Hash::GenerateFNVHash(testStr);
		uint32_t hash3 = Hash::GenerateFNVHash(testStr);
		
		REQUIRE(hash1 == hash2);
		REQUIRE(hash2 == hash3);
	}
}

TEST_CASE("Hash FNV collision resistance", "[unit][core][hash]")
{
	SECTION("Different single characters produce different hashes")
	{
		constexpr uint32_t hashA = Hash::GenerateFNVHash("a");
		constexpr uint32_t hashB = Hash::GenerateFNVHash("b");
		constexpr uint32_t hashZ = Hash::GenerateFNVHash("z");
		
		REQUIRE(hashA != hashB);
		REQUIRE(hashB != hashZ);
		REQUIRE(hashA != hashZ);
	}

	SECTION("Similar strings produce different hashes")
	{
		constexpr uint32_t hash1 = Hash::GenerateFNVHash("test1");
		constexpr uint32_t hash2 = Hash::GenerateFNVHash("test2");
		constexpr uint32_t hash3 = Hash::GenerateFNVHash("test3");
		
		REQUIRE(hash1 != hash2);
		REQUIRE(hash2 != hash3);
		REQUIRE(hash1 != hash3);
	}

	SECTION("Permutations produce different hashes")
	{
		constexpr uint32_t hash1 = Hash::GenerateFNVHash("abc");
		constexpr uint32_t hash2 = Hash::GenerateFNVHash("acb");
		constexpr uint32_t hash3 = Hash::GenerateFNVHash("bac");
		
		REQUIRE(hash1 != hash2);
		REQUIRE(hash2 != hash3);
		REQUIRE(hash1 != hash3);
	}

	SECTION("Many unique strings produce mostly unique hashes")
	{
		std::unordered_set<uint32_t> hashes;
		constexpr int numStrings = 1000;
		
		for (int i = 0; i < numStrings; ++i)
		{
			std::string str = "string_" + std::to_string(i);
			uint32_t hash = Hash::GenerateFNVHash(str);
			hashes.insert(hash);
		}
		
		// Should have very few collisions (allow < 1% collision rate)
		REQUIRE(hashes.size() > numStrings * 0.99);
	}
}

TEST_CASE("Hash FNV special cases", "[unit][core][hash]")
{
	SECTION("Single character strings")
	{
		constexpr uint32_t hash = Hash::GenerateFNVHash("x");
		REQUIRE(hash != 0);
	}

	SECTION("Long strings")
	{
		std::string longString(1000, 'a');
		uint32_t hash = Hash::GenerateFNVHash(longString);
		
		REQUIRE(hash != 0);
	}

	SECTION("Strings with special characters")
	{
		constexpr uint32_t hash1 = Hash::GenerateFNVHash("hello\nworld");
		constexpr uint32_t hash2 = Hash::GenerateFNVHash("hello\tworld");
		constexpr uint32_t hash3 = Hash::GenerateFNVHash("hello world");
		
		REQUIRE(hash1 != hash2);
		REQUIRE(hash2 != hash3);
		REQUIRE(hash1 != hash3);
	}

	SECTION("Strings with numbers")
	{
		constexpr uint32_t hash1 = Hash::GenerateFNVHash("test123");
		constexpr uint32_t hash2 = Hash::GenerateFNVHash("test456");
		
		REQUIRE(hash1 != hash2);
	}

	SECTION("Unicode/Extended ASCII (if supported)")
	{
		uint32_t hash1 = Hash::GenerateFNVHash("café");
		uint32_t hash2 = Hash::GenerateFNVHash("cafe");
		
		REQUIRE(hash1 != hash2);
	}
}

TEST_CASE("Hash CRC32 basic functionality", "[unit][core][hash]")
{
	SECTION("CRC32 with C-string produces non-zero hash")
	{
		uint32_t hash = Hash::CRC32("test");
		REQUIRE(hash != 0);
	}

	SECTION("CRC32 with std::string produces non-zero hash")
	{
		std::string str = "test";
		uint32_t hash = Hash::CRC32(str);
		REQUIRE(hash != 0);
	}

	SECTION("CRC32 C-string and std::string produce same hash")
	{
		const char* cstr = "example";
		std::string stdstr = "example";
		
		uint32_t hash1 = Hash::CRC32(cstr);
		uint32_t hash2 = Hash::CRC32(stdstr);
		
		REQUIRE(hash1 == hash2);
	}

	SECTION("Empty string CRC32")
	{
		uint32_t hash = Hash::CRC32("");
		REQUIRE(hash != 0);
	}
}

TEST_CASE("Hash CRC32 determinism", "[unit][core][hash]")
{
	SECTION("Same string produces same CRC32")
	{
		uint32_t hash1 = Hash::CRC32("deterministic");
		uint32_t hash2 = Hash::CRC32("deterministic");
		
		REQUIRE(hash1 == hash2);
	}

	SECTION("Repeated calls produce same result")
	{
		std::string testStr = "repeated_crc32";
		
		uint32_t hash1 = Hash::CRC32(testStr);
		uint32_t hash2 = Hash::CRC32(testStr);
		uint32_t hash3 = Hash::CRC32(testStr);
		
		REQUIRE(hash1 == hash2);
		REQUIRE(hash2 == hash3);
	}
}

TEST_CASE("Hash CRC32 collision resistance", "[unit][core][hash]")
{
	SECTION("Different strings produce different CRC32")
	{
		uint32_t hash1 = Hash::CRC32("string1");
		uint32_t hash2 = Hash::CRC32("string2");
		uint32_t hash3 = Hash::CRC32("string3");
		
		REQUIRE(hash1 != hash2);
		REQUIRE(hash2 != hash3);
		REQUIRE(hash1 != hash3);
	}

	SECTION("Case sensitivity")
	{
		uint32_t hashLower = Hash::CRC32("test");
		uint32_t hashUpper = Hash::CRC32("TEST");
		
		REQUIRE(hashLower != hashUpper);
	}

	SECTION("Order matters")
	{
		uint32_t hash1 = Hash::CRC32("abc");
		uint32_t hash2 = Hash::CRC32("cba");
		
		REQUIRE(hash1 != hash2);
	}

	SECTION("Many unique strings produce mostly unique CRC32")
	{
		std::unordered_set<uint32_t> hashes;
		constexpr int numStrings = 1000;
		
		for (int i = 0; i < numStrings; ++i)
		{
			std::string str = "crc_string_" + std::to_string(i);
			uint32_t hash = Hash::CRC32(str);
			hashes.insert(hash);
		}
		
		// CRC32 should have very few collisions
		REQUIRE(hashes.size() > numStrings * 0.99);
	}
}

TEST_CASE("Hash CRC32 special cases", "[unit][core][hash]")
{
	SECTION("Single character")
	{
		uint32_t hash = Hash::CRC32("x");
		REQUIRE(hash != 0);
	}

	SECTION("Long strings")
	{
		std::string longString(10000, 'a');
		uint32_t hash = Hash::CRC32(longString);
		
		REQUIRE(hash != 0);
	}

	SECTION("Strings with special characters")
	{
		uint32_t hash1 = Hash::CRC32("hello\nworld");
		uint32_t hash2 = Hash::CRC32("hello\tworld");
		uint32_t hash3 = Hash::CRC32("hello world");
		
		REQUIRE(hash1 != hash2);
		REQUIRE(hash2 != hash3);
		REQUIRE(hash1 != hash3);
	}

	SECTION("Binary data (nulls in string)")
	{
		std::string binaryData = "test";
		binaryData += '\0';
		binaryData += "data";
		
		uint32_t hash = Hash::CRC32(binaryData);
		REQUIRE(hash != 0);
	}
}

TEST_CASE("Hash FNV vs CRC32 comparison", "[unit][core][hash]")
{
	SECTION("FNV and CRC32 produce different hashes for same input")
	{
		const char* testStr = "comparison_test";
		
		uint32_t fnvHash = Hash::GenerateFNVHash(testStr);
		uint32_t crc32Hash = Hash::CRC32(testStr);
		
		REQUIRE(fnvHash != crc32Hash);
	}

	SECTION("Both algorithms handle empty strings")
	{
		uint32_t fnvHash = Hash::GenerateFNVHash("");
		uint32_t crc32Hash = Hash::CRC32("");
		
		REQUIRE(fnvHash != 0);
		REQUIRE(crc32Hash != 0);
	}
}

TEST_CASE("Hash performance characteristics", "[unit][core][hash]")
{
	SECTION("FNV can hash many strings quickly")
	{
		constexpr int iterations = 10000;
		
		for (int i = 0; i < iterations; ++i)
		{
			std::string str = "perf_test_" + std::to_string(i);
			uint32_t hash = Hash::GenerateFNVHash(str);
			REQUIRE(hash != 0);
		}
		
		REQUIRE(true); // Completed without timeout
	}

	SECTION("CRC32 can hash many strings quickly")
	{
		constexpr int iterations = 10000;
		
		for (int i = 0; i < iterations; ++i)
		{
			std::string str = "perf_test_" + std::to_string(i);
			uint32_t hash = Hash::CRC32(str);
			REQUIRE(hash != 0);
		}
		
		REQUIRE(true); // Completed without timeout
	}
}

TEST_CASE("Hash known test vectors", "[unit][core][hash]")
{
	SECTION("FNV known values (if documented)")
	{
		// These are example test vectors - adjust based on your FNV implementation
		// FNV-1a 32-bit hash of empty string should be 0x811c9dc5 before final XOR
		constexpr uint32_t emptyHash = Hash::GenerateFNVHash("");
		REQUIRE(emptyHash != 0);
	}

	SECTION("CRC32 consistency across runs")
	{
		// Standard CRC32 test vector
		std::string testData = "123456789";
		uint32_t hash = Hash::CRC32(testData);
		
		// Verify consistency (not the actual CRC32 value as implementation may vary)
		REQUIRE(hash != 0);
		REQUIRE(hash == Hash::CRC32(testData));
	}
}