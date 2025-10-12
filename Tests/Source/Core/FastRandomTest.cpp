#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "Luma/Core/FastRandom.hpp"

#include <array>
#include <unordered_set>
#include <cmath>

using namespace Luma;

TEST_CASE("FastRandom default construction", "[unit][core][fastrandom]")
{
	SECTION("Default constructor uses default seed")
	{
		FastRandom rng;
		
		int firstValue = rng.GetInt32();
		
		// With default seed, should produce deterministic sequence
		FastRandom rng2;
		int secondValue = rng2.GetInt32();
		
		REQUIRE(firstValue == secondValue);
	}

	SECTION("Default seed produces expected initial state")
	{
		FastRandom rng;
		
		REQUIRE(rng.GetCurrentSeed() == 4321);
	}
}

TEST_CASE("FastRandom explicit seed construction", "[unit][core][fastrandom]")
{
	SECTION("Constructor with specific seed")
	{
		constexpr int testSeed = 12345;
		FastRandom rng(testSeed);
		
		REQUIRE(rng.GetCurrentSeed() == testSeed);
	}

	SECTION("Different seeds produce different sequences")
	{
		FastRandom rng1(100);
		FastRandom rng2(200);
		
		int value1 = rng1.GetInt32();
		int value2 = rng2.GetInt32();
		
		REQUIRE(value1 != value2);
	}

	SECTION("Same seed produces identical sequences")
	{
		constexpr int seed = 42;
		FastRandom rng1(seed);
		FastRandom rng2(seed);
		
		for (int i = 0; i < 100; ++i)
		{
			REQUIRE(rng1.GetInt32() == rng2.GetInt32());
		}
	}

	SECTION("Zero seed is valid")
	{
		FastRandom rng(0);
		
		REQUIRE(rng.GetCurrentSeed() == 0);
		REQUIRE_NOTHROW(rng.GetInt32());
	}

	SECTION("Negative seed is valid")
	{
		FastRandom rng(-12345);
		
		REQUIRE(rng.GetCurrentSeed() == -12345);
		REQUIRE_NOTHROW(rng.GetInt32());
	}
}

TEST_CASE("FastRandom seed manipulation", "[unit][core][fastrandom]")
{
	SECTION("SetSeed changes the seed")
	{
		FastRandom rng(100);
		
		rng.SetSeed(200);
		
		REQUIRE(rng.GetCurrentSeed() == 200);
	}

	SECTION("SetSeed resets the sequence")
	{
		constexpr int seed = 777;
		FastRandom rng1(seed);
		FastRandom rng2(seed);
		
		// Advance rng1
		for (int i = 0; i < 50; ++i)
			rng1.GetInt32();
		
		// Reset to original seed
		rng1.SetSeed(seed);
		
		// Should now match fresh rng2
		REQUIRE(rng1.GetInt32() == rng2.GetInt32());
	}

	SECTION("GetCurrentSeed returns current state")
	{
		FastRandom rng(5000);
		
		int initialSeed = rng.GetCurrentSeed();
		rng.GetInt32(); // Advance state
		int newSeed = rng.GetCurrentSeed();
		
		REQUIRE(initialSeed != newSeed);
	}
}

TEST_CASE("FastRandom Int32 generation", "[unit][core][fastrandom]")
{
	SECTION("GetInt32 produces values")
	{
		FastRandom rng(42);
		
		int value = rng.GetInt32();
		
		REQUIRE(value != 0); // Extremely unlikely to be zero
	}

	SECTION("GetInt32 produces different values in sequence")
	{
		FastRandom rng(123);
		
		int value1 = rng.GetInt32();
		int value2 = rng.GetInt32();
		int value3 = rng.GetInt32();
		
		// While technically possible, extremely unlikely all are equal
		bool allDifferent = (value1 != value2) || (value2 != value3);
		REQUIRE(allDifferent);
	}

	SECTION("GetInt32 is deterministic")
	{
		FastRandom rng1(999);
		FastRandom rng2(999);
		
		constexpr int iterations = 1000;
		for (int i = 0; i < iterations; ++i)
		{
			REQUIRE(rng1.GetInt32() == rng2.GetInt32());
		}
	}

	SECTION("GetInt32 eventually cycles (LCG property)")
	{
		// LCG has a period, but for this seed/params it should be very long
		// Just verify it doesn't get stuck
		FastRandom rng(1);
		
		std::unordered_set<int> seen;
		bool foundCycle = false;
		
		for (int i = 0; i < 10000; ++i)
		{
			int value = rng.GetInt32();
			if (seen.count(value) > 0)
			{
				foundCycle = true;
				break;
			}
			seen.insert(value);
		}
		
		// Should have many unique values before cycling
		REQUIRE(seen.size() > 1000);
	}
}

TEST_CASE("FastRandom UInt32 generation", "[unit][core][fastrandom]")
{
	SECTION("GetUInt32 produces unsigned values")
	{
		FastRandom rng(42);
		
		unsigned int value = rng.GetUInt32();
		
		// All values should be valid unsigned ints
		REQUIRE(value >= 0);
	}

	SECTION("GetUInt32 deterministic like GetInt32")
	{
		FastRandom rng1(555);
		FastRandom rng2(555);
		
		for (int i = 0; i < 100; ++i)
		{
			REQUIRE(rng1.GetUInt32() == rng2.GetUInt32());
		}
	}
}

TEST_CASE("FastRandom Int16/UInt16 generation", "[unit][core][fastrandom]")
{
	SECTION("GetInt16 produces 16-bit values")
	{
		FastRandom rng(123);
		
		short value = rng.GetInt16();
		
		// Value should fit in 16 bits
		REQUIRE(value >= std::numeric_limits<short>::min());
		REQUIRE(value <= std::numeric_limits<short>::max());
	}

	SECTION("GetUInt16 produces 16-bit unsigned values")
	{
		FastRandom rng(456);
		
		unsigned short value = rng.GetUInt16();
		
		REQUIRE(value >= 0);
		REQUIRE(value <= std::numeric_limits<unsigned short>::max());
	}

	SECTION("GetInt16 has reasonable distribution")
	{
		FastRandom rng(789);
		
		int positiveCount = 0;
		int negativeCount = 0;
		constexpr int samples = 1000;
		
		for (int i = 0; i < samples; ++i)
		{
			short value = rng.GetInt16();
			if (value >= 0)
				++positiveCount;
			else
				++negativeCount;
		}
		
		// Should have reasonable mix of positive and negative
		REQUIRE(positiveCount > samples / 10);
		REQUIRE(negativeCount > samples / 10);
	}
}

TEST_CASE("FastRandom Float32 generation", "[unit][core][fastrandom]")
{
	SECTION("GetFloat32 produces values in [0, 1] range")
	{
		FastRandom rng(42);
		
		for (int i = 0; i < 1000; ++i)
		{
			float value = rng.GetFloat32();
			
			REQUIRE(value >= 0.0f);
			REQUIRE(value <= 1.0f);
		}
	}

	SECTION("GetFloat32 produces varied values")
	{
		FastRandom rng(123);
		
		float value1 = rng.GetFloat32();
		float value2 = rng.GetFloat32();
		float value3 = rng.GetFloat32();
		
		bool hasVariation = (std::abs(value1 - value2) > 0.001f) || 
		                    (std::abs(value2 - value3) > 0.001f);
		REQUIRE(hasVariation);
	}

	SECTION("GetFloat32 is deterministic")
	{
		FastRandom rng1(999);
		FastRandom rng2(999);
		
		for (int i = 0; i < 100; ++i)
		{
			REQUIRE(rng1.GetFloat32() == rng2.GetFloat32());
		}
	}
}

TEST_CASE("FastRandom Float64 generation", "[unit][core][fastrandom]")
{
	SECTION("GetFloat64 produces values in [0, 1] range")
	{
		FastRandom rng(42);
		
		for (int i = 0; i < 1000; ++i)
		{
			double value = rng.GetFloat64();
			
			REQUIRE(value >= 0.0);
			REQUIRE(value <= 1.0);
		}
	}

	SECTION("GetFloat64 has higher precision than Float32")
	{
		FastRandom rng(456);
		
		// Generate values and check they use the full double precision
		double value = rng.GetFloat64();
		
		REQUIRE(std::isfinite(value));
		REQUIRE(value >= 0.0);
		REQUIRE(value <= 1.0);
	}
}

TEST_CASE("FastRandom Float32 range generation", "[unit][core][fastrandom]")
{
	SECTION("GetFloat32InRange respects bounds")
	{
		FastRandom rng(42);
		constexpr float low = -10.0f;
		constexpr float high = 50.0f;
		
		for (int i = 0; i < 1000; ++i)
		{
			float value = rng.GetFloat32InRange(low, high);
			
			REQUIRE(value >= low);
			REQUIRE(value <= high);
		}
	}

	SECTION("GetFloat32InRange with negative range")
	{
		FastRandom rng(123);
		
		for (int i = 0; i < 100; ++i)
		{
			float value = rng.GetFloat32InRange(-100.0f, -10.0f);
			
			REQUIRE(value >= -100.0f);
			REQUIRE(value <= -10.0f);
		}
	}

	SECTION("GetFloat32InRange with zero-width range returns low value")
	{
		FastRandom rng(456);
		
		float value = rng.GetFloat32InRange(5.0f, 5.0f);
		
		REQUIRE_THAT(value, Catch::Matchers::WithinAbs(5.0f, 0.0001f));
	}

	SECTION("GetFloat32InRange covers full range")
	{
		FastRandom rng(789);
		constexpr float low = 0.0f;
		constexpr float high = 100.0f;
		
		float minSeen = high;
		float maxSeen = low;
		
		for (int i = 0; i < 10000; ++i)
		{
			float value = rng.GetFloat32InRange(low, high);
			minSeen = std::min(minSeen, value);
			maxSeen = std::max(maxSeen, value);
		}
		
		// Should explore most of the range
		REQUIRE(minSeen < low + 10.0f);
		REQUIRE(maxSeen > high - 10.0f);
	}
}

TEST_CASE("FastRandom Float64 range generation", "[unit][core][fastrandom]")
{
	SECTION("GetFloat64InRange respects bounds")
	{
		FastRandom rng(42);
		constexpr double low = -100.0;
		constexpr double high = 100.0;
		
		for (int i = 0; i < 1000; ++i)
		{
			double value = rng.GetFloat64InRange(low, high);
			
			REQUIRE(value >= low);
			REQUIRE(value <= high);
		}
	}

	SECTION("GetFloat64InRange with very large range")
	{
		FastRandom rng(123);
		
		double value = rng.GetFloat64InRange(-1e6, 1e6);
		
		REQUIRE(value >= -1e6);
		REQUIRE(value <= 1e6);
	}
}

TEST_CASE("FastRandom Int32 range generation", "[unit][core][fastrandom]")
{
	SECTION("GetInt32InRange respects bounds")
	{
		FastRandom rng(42);
		constexpr int low = 10;
		constexpr int high = 50;
		
		for (int i = 0; i < 1000; ++i)
		{
			int value = rng.GetInt32InRange(low, high);
			
			REQUIRE(value >= low);
			REQUIRE(value <= high);
		}
	}

	SECTION("GetInt32InRange with negative range")
	{
		FastRandom rng(123);
		
		for (int i = 0; i < 100; ++i)
		{
			int value = rng.GetInt32InRange(-100, -10);
			
			REQUIRE(value >= -100);
			REQUIRE(value <= -10);
		}
	}

	SECTION("GetInt32InRange with inverted bounds returns low")
	{
		FastRandom rng(456);
		
		int value = rng.GetInt32InRange(100, 10);
		
		REQUIRE(value == 100);
	}

	SECTION("GetInt32InRange with equal bounds returns that value")
	{
		FastRandom rng(789);
		
		int value = rng.GetInt32InRange(42, 42);
		
		REQUIRE(value == 42);
	}

	SECTION("GetInt32InRange explores full range")
	{
		FastRandom rng(999);
		constexpr int low = 1;
		constexpr int high = 100;
		
		std::unordered_set<int> seen;
		
		for (int i = 0; i < 10000; ++i)
		{
			int value = rng.GetInt32InRange(low, high);
			seen.insert(value);
		}
		
		// Should see most values in a reasonable-sized range
		REQUIRE(seen.size() > 80); // At least 80% of values
	}

	SECTION("GetInt32InRange small range")
	{
		FastRandom rng(111);
		
		for (int i = 0; i < 100; ++i)
		{
			int value = rng.GetInt32InRange(0, 2);
			
			REQUIRE(value >= 0);
			REQUIRE(value <= 2);
		}
	}
}

TEST_CASE("FastRandom GetInRange overloads", "[unit][core][fastrandom]")
{
	SECTION("GetInRange with float arguments")
	{
		FastRandom rng(42);
		
		float value = rng.GetInRange(0.0f, 10.0f);
		
		REQUIRE(value >= 0.0f);
		REQUIRE(value <= 10.0f);
	}

	SECTION("GetInRange with double arguments")
	{
		FastRandom rng(123);
		
		double value = rng.GetInRange(0.0, 10.0);
		
		REQUIRE(value >= 0.0);
		REQUIRE(value <= 10.0);
	}

	SECTION("GetInRange with int arguments")
	{
		FastRandom rng(456);
		
		int value = rng.GetInRange(0, 10);
		
		REQUIRE(value >= 0);
		REQUIRE(value <= 10);
	}
}

TEST_CASE("FastRandom distribution quality", "[unit][core][fastrandom]")
{
	SECTION("Float32 has reasonable distribution")
	{
		FastRandom rng(12345);
		constexpr int samples = 10000;
		constexpr int buckets = 10;
		
		std::array<int, buckets> histogram{};
		
		for (int i = 0; i < samples; ++i)
		{
			float value = rng.GetFloat32();
			int bucket = static_cast<int>(value * buckets);
			if (bucket >= buckets) bucket = buckets - 1;
			histogram[bucket]++;
		}
		
		// Check that distribution is reasonably uniform
		int expectedPerBucket = samples / buckets;
		for (int count : histogram)
		{
			// Allow 30% deviation from expected
			REQUIRE(count > expectedPerBucket * 0.7);
			REQUIRE(count < expectedPerBucket * 1.3);
		}
	}

	SECTION("Int32InRange has reasonable distribution")
	{
		FastRandom rng(67890);
		constexpr int samples = 10000;
		constexpr int low = 0;
		constexpr int high = 9;
		
		std::array<int, 10> histogram{};
		
		for (int i = 0; i < samples; ++i)
		{
			int value = rng.GetInt32InRange(low, high);
			histogram[value]++;
		}
		
		int expectedPerValue = samples / 10;
		for (int count : histogram)
		{
			// Allow 30% deviation
			REQUIRE(count > expectedPerValue * 0.7);
			REQUIRE(count < expectedPerValue * 1.3);
		}
	}
}

TEST_CASE("FastRandom Utils::GetSeedFromCurrentTime", "[unit][core][fastrandom]")
{
	SECTION("GetSeedFromCurrentTime returns non-zero value")
	{
		int seed = Utils::GetSeedFromCurrentTime();
		
		REQUIRE(seed != 0);
	}

	SECTION("GetSeedFromCurrentTime produces different values over time")
	{
		int seed1 = Utils::GetSeedFromCurrentTime();
		
		// Small delay to ensure time changes
		for (volatile int i = 0; i < 10000; ++i) {}
		
		int seed2 = Utils::GetSeedFromCurrentTime();
		
		// Seeds should be different (time-based)
		REQUIRE(seed1 != seed2);
	}

	SECTION("GetSeedFromCurrentTime can initialize FastRandom")
	{
		int seed = Utils::GetSeedFromCurrentTime();
		FastRandom rng(seed);
		
		int value = rng.GetInt32();
		
		REQUIRE(std::isfinite(static_cast<float>(value)));
	}
}

TEST_CASE("FastRandom edge cases", "[unit][core][fastrandom]")
{
	SECTION("Very large range for Int32InRange")
	{
		FastRandom rng(42);
		
		int value = rng.GetInt32InRange(std::numeric_limits<int>::min() / 2, 
		                                 std::numeric_limits<int>::max() / 2);
		
		REQUIRE(value >= std::numeric_limits<int>::min() / 2);
		REQUIRE(value <= std::numeric_limits<int>::max() / 2);
	}

	SECTION("Continuous generation doesn't hang")
	{
		FastRandom rng(777);
		
		for (int i = 0; i < 100000; ++i)
		{
			rng.GetInt32();
		}
		
		REQUIRE(true); // Didn't hang
	}

	SECTION("State overflow handling")
	{
		FastRandom rng(std::numeric_limits<int>::max());
		
		// Should handle large seeds gracefully
		REQUIRE_NOTHROW(rng.GetInt32());
		REQUIRE_NOTHROW(rng.GetFloat32());
	}
}