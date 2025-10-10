#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "Luma/Core/TimeStep.hpp"

using namespace Luma;

TEST_CASE("Timestep default construction", "[unit][core][timestep]")
{
	SECTION("Default constructor initializes to zero")
	{
		Timestep ts;

		REQUIRE(ts.GetSeconds() == 0.0f);
		REQUIRE(ts.GetMilliseconds() == 0.0f);
	}

	SECTION("Default timestep converts to zero float")
	{
		Timestep ts;
		float value = ts;

		REQUIRE(value == 0.0f);
	}
}

TEST_CASE("Timestep explicit construction", "[unit][core][timestep]")
{
	SECTION("Constructor with specific value in seconds")
	{
		Timestep ts(1.0f);

		REQUIRE(ts.GetSeconds() == 1.0f);
		REQUIRE(ts.GetMilliseconds() == 1000.0f);
	}
	
	SECTION("Constructor with fractional seconds")
	{
		Timestep ts(0.016667f); // ~60 FPS

		REQUIRE_THAT(ts.GetSeconds(), Catch::Matchers::WithinAbs(0.016667f, 0.00001f));
		REQUIRE_THAT(ts.GetMilliseconds(), Catch::Matchers::WithinAbs(16.667f, 0.01f));
	}

	SECTION("Constructor with small delta time")
	{
		Timestep ts(0.001f); // 1ms

		REQUIRE_THAT(ts.GetSeconds(), Catch::Matchers::WithinAbs(0.001f, 0.00001f));
		REQUIRE_THAT(ts.GetMilliseconds(), Catch::Matchers::WithinAbs(1.0f, 0.01f));
	}

	SECTION("Constructor with large delta time")
	{
		Timestep ts(5.0f); // 5 seconds

		REQUIRE(ts.GetSeconds() == 5.0f);
		REQUIRE(ts.GetMilliseconds() == 5000.0f);
	}
}

TEST_CASE("Timestep time conversions", "[unit][core][timestep]")
{
	SECTION("Seconds to milliseconds conversion")
	{
		Timestep ts(1.5f);

		REQUIRE(ts.GetSeconds() == 1.5f);
		REQUIRE(ts.GetMilliseconds() == 1500.0f);
	}

	SECTION("Common frame rates in seconds")
	{
		// 60 FPS
		Timestep ts60(1.0f / 60.0f);
		REQUIRE_THAT(ts60.GetSeconds(), Catch::Matchers::WithinAbs(0.016667f, 0.00001f));
		REQUIRE_THAT(ts60.GetMilliseconds(), Catch::Matchers::WithinAbs(16.667f, 0.01f));

		// 30 FPS
		Timestep ts30(1.0f / 30.0f);
		REQUIRE_THAT(ts30.GetSeconds(), Catch::Matchers::WithinAbs(0.033333f, 0.00001f));
		REQUIRE_THAT(ts30.GetMilliseconds(), Catch::Matchers::WithinAbs(33.333f, 0.01f));

		// 144 FPS
		Timestep ts144(1.0f / 144.0f);
		REQUIRE_THAT(ts144.GetSeconds(), Catch::Matchers::WithinAbs(0.006944f, 0.00001f));
		REQUIRE_THAT(ts144.GetMilliseconds(), Catch::Matchers::WithinAbs(6.944f, 0.01f));
	}

	SECTION("Zero timestep conversions")
	{
		Timestep ts(0.0f);

		REQUIRE(ts.GetSeconds() == 0.0f);
		REQUIRE(ts.GetMilliseconds() == 0.0f);
	}
}

TEST_CASE("Timestep implicit float conversion", "[unit][core][timestep]")
{
	SECTION("Conversion operator returns seconds")
	{
		Timestep ts(2.5f);
		float value = ts;

		REQUIRE(value == 2.5f);
	}

	SECTION("Use in arithmetic operations")
	{
		Timestep ts(0.016f);

		float doubled = ts * 2.0f;
		REQUIRE_THAT(doubled, Catch::Matchers::WithinAbs(0.032f, 0.0001f));

		float halved = ts / 2.0f;
		REQUIRE_THAT(halved, Catch::Matchers::WithinAbs(0.008f, 0.0001f));

		float added = ts + 0.004f;
		REQUIRE_THAT(added, Catch::Matchers::WithinAbs(0.020f, 0.0001f));
	}

	SECTION("Use in comparisons")
	{
		Timestep ts1(0.016f);
		Timestep ts2(0.032f);

		REQUIRE(ts1 < 0.020f);
		REQUIRE(ts2 > 0.020f);
		REQUIRE_THAT(static_cast<float>(ts1), Catch::Matchers::WithinAbs(0.016f, 0.0001f));
	}

	SECTION("Use in function calls expecting float")
	{
		auto calculateFPS = [](float deltaTime) -> float {
			return deltaTime > 0.0f ? 1.0f / deltaTime : 0.0f;
		};

		Timestep ts(0.016667f); // ~60 FPS
		float fps = calculateFPS(ts);

		REQUIRE_THAT(fps, Catch::Matchers::WithinAbs(60.0f, 0.1f));
	}
}

TEST_CASE("Timestep copy semantics", "[unit][core][timestep]")
{
	SECTION("Copy constructor")
	{
		Timestep original(1.5f);
		Timestep copy(original);

		REQUIRE(copy.GetSeconds() == original.GetSeconds());
		REQUIRE(copy.GetMilliseconds() == original.GetMilliseconds());
	}

	SECTION("Assignment operator (implicit)")
	{
		Timestep ts1(1.0f);
		Timestep ts2(2.0f);

		ts2 = ts1;

		REQUIRE(ts2.GetSeconds() == 1.0f);
		REQUIRE(ts2.GetMilliseconds() == 1000.0f);
	}
}

TEST_CASE("Timestep practical usage scenarios", "[unit][core][timestep]")
{
	SECTION("Calculate distance with velocity")
	{
		Timestep dt(0.016667f); // 60 FPS
		float velocity = 100.0f; // units per second

		float distance = velocity * dt;
		
		REQUIRE_THAT(distance, Catch::Matchers::WithinAbs(1.6667f, 0.001f));
	}

	SECTION("Accumulate time over frames")
	{
		Timestep frameTime(0.016667f);
		float totalTime = 0.0f;

		for (int i = 0; i < 60; ++i)
		{
			totalTime += frameTime;
		}

		// 60 frames at 60 FPS = 1 second
		REQUIRE_THAT(totalTime, Catch::Matchers::WithinAbs(1.0f, 0.01f));
	}

	SECTION("FPS calculation from timestep")
	{
		Timestep ts60(1.0f / 60.0f);
		Timestep ts30(1.0f / 30.0f);
		Timestep ts144(1.0f / 144.0f);

		float fps60 = 1.0f / ts60.GetSeconds();
		float fps30 = 1.0f / ts30.GetSeconds();
		float fps144 = 1.0f / ts144.GetSeconds();

		REQUIRE_THAT(fps60, Catch::Matchers::WithinAbs(60.0f, 0.1f));
		REQUIRE_THAT(fps30, Catch::Matchers::WithinAbs(30.0f, 0.1f));
		REQUIRE_THAT(fps144, Catch::Matchers::WithinAbs(144.0f, 0.1f));
	}

	SECTION("Delta time clamping simulation")
	{
		auto clampDeltaTime = [](Timestep dt, float maxDt) -> float {
			float clamped = dt.GetSeconds();
			if (clamped > maxDt)
				clamped = maxDt;
			return clamped;
		};

		Timestep normalFrame(0.016f);
		Timestep slowFrame(0.200f); // Spike

		float maxDeltaTime = 0.033f; // Cap at ~30 FPS

		float clampedNormal = clampDeltaTime(normalFrame, maxDeltaTime);
		float clampedSlow = clampDeltaTime(slowFrame, maxDeltaTime);

		REQUIRE_THAT(clampedNormal, Catch::Matchers::WithinAbs(0.016f, 0.001f));
		REQUIRE_THAT(clampedSlow, Catch::Matchers::WithinAbs(0.033f, 0.001f));
	}
}

TEST_CASE("Timestep edge cases", "[unit][core][timestep]")
{
	SECTION("Very small timestep values")
	{
		Timestep ts(0.000001f); // 1 microsecond

		REQUIRE(ts.GetSeconds() == 0.000001f);
		REQUIRE_THAT(ts.GetMilliseconds(), Catch::Matchers::WithinAbs(0.001f, 0.0001f));
	}

	SECTION("Very large timestep values")
	{
		Timestep ts(1000.0f); // 1000 seconds

		REQUIRE(ts.GetSeconds() == 1000.0f);
		REQUIRE(ts.GetMilliseconds() == 1000000.0f);
	}

	SECTION("Negative timestep values")
	{
		// While unlikely in practice, the class should handle it
		Timestep ts(-1.0f);

		REQUIRE(ts.GetSeconds() == -1.0f);
		REQUIRE(ts.GetMilliseconds() == -1000.0f);
	}
}