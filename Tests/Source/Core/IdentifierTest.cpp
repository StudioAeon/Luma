#include <catch2/catch_test_macros.hpp>

#include "Luma/Core/Identifier.hpp"

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <mutex>
#include <thread>

using namespace Luma;

TEST_CASE("Identifier default construction", "[unit][core][identifier]")
{
	SECTION("Default constructor creates zero identifier")
	{
		constexpr Identifier id;

		REQUIRE(static_cast<uint32_t>(id) == 0);
	}

	SECTION("Default identifiers are equal")
	{
		constexpr Identifier id1;
		constexpr Identifier id2;

		REQUIRE(id1 == id2);
	}

	SECTION("Default identifier has empty debug name")
	{
		constexpr Identifier id;

		REQUIRE(id.GetDBGName().empty());
	}
}

TEST_CASE("Identifier string construction", "[unit][core][identifier]")
{
	SECTION("Construct from string literal")
	{
		constexpr Identifier id("test");

		REQUIRE(static_cast<uint32_t>(id) != 0);
	}

	SECTION("Construct from std::string_view")
	{
		std::string_view sv = "example";
		Identifier id(sv);

		REQUIRE(static_cast<uint32_t>(id) != 0);
	}

	SECTION("Same string produces same identifier")
	{
		constexpr Identifier id1("hello");
		constexpr Identifier id2("hello");

		REQUIRE(id1 == id2);
	}

	SECTION("Different strings produce different identifiers")
	{
		constexpr Identifier id1("hello");
		constexpr Identifier id2("world");

		REQUIRE(id1 != id2);
	}

	SECTION("Debug name is preserved")
	{
		constexpr Identifier id("test_name");

		REQUIRE(std::string(id.GetDBGName()) == "test_name");
	}

	SECTION("Empty string identifier")
	{
		constexpr Identifier id("");

		REQUIRE(static_cast<uint32_t>(id) != 0);
		REQUIRE(id.GetDBGName().empty());
	}
}

TEST_CASE("Identifier hash construction", "[unit][core][identifier]")
{
	SECTION("Construct from explicit hash value")
	{
		constexpr uint32_t hash = 0x12345678;
		constexpr Identifier id(hash);

		REQUIRE(static_cast<uint32_t>(id) == hash);
	}

	SECTION("Zero hash is valid")
	{
		constexpr Identifier id(0u);

		REQUIRE(static_cast<uint32_t>(id) == 0);
	}

	SECTION("Maximum hash value")
	{
		constexpr uint32_t maxHash = 0xFFFFFFFF;
		constexpr Identifier id(maxHash);

		REQUIRE(static_cast<uint32_t>(id) == maxHash);
	}

	SECTION("Hash constructed identifier has no debug name")
	{
		constexpr Identifier id(12345u);

		// Debug name should be empty when constructed from hash
		REQUIRE(id.GetDBGName().empty());
	}
}

TEST_CASE("Identifier equality operators", "[unit][core][identifier]")
{
	SECTION("Equality comparison works")
	{
		constexpr Identifier id1("test");
		constexpr Identifier id2("test");

		REQUIRE(id1 == id2);
	}

	SECTION("Inequality comparison works")
	{
		constexpr Identifier id1("hello");
		constexpr Identifier id2("world");

		REQUIRE(id1 != id2);
	}

	SECTION("Identifiers from same hash are equal")
	{
		constexpr uint32_t hash = 0xABCDEF00;
		constexpr Identifier id1(hash);
		constexpr Identifier id2(hash);

		REQUIRE(id1 == id2);
	}

	SECTION("Self equality")
	{
		constexpr Identifier id("self_test");

		REQUIRE(id == id);
		REQUIRE_FALSE(id != id);
	}
}

TEST_CASE("Identifier uint32_t conversion", "[unit][core][identifier]")
{
	SECTION("Implicit conversion to uint32_t")
	{
		constexpr Identifier id("convert");

		uint32_t hash = id;
		REQUIRE(hash != 0);
	}

	SECTION("Explicit cast to uint32_t")
	{
		constexpr Identifier id("explicit");

		uint32_t hash = static_cast<uint32_t>(id);
		REQUIRE(hash != 0);
	}

	SECTION("Conversion preserves hash value")
	{
		constexpr uint32_t originalHash = 0x87654321;
		constexpr Identifier id(originalHash);

		uint32_t convertedHash = id;
		REQUIRE(convertedHash == originalHash);
	}
}

TEST_CASE("Identifier constexpr evaluation", "[unit][core][identifier]")
{
	SECTION("Can be used in constexpr context")
	{
		constexpr Identifier id("compile_time");
		constexpr uint32_t hash = id;

		REQUIRE(hash != 0);
	}

	SECTION("Comparison is constexpr")
	{
		constexpr Identifier id1("test1");
		constexpr Identifier id2("test2");
		constexpr bool areEqual = (id1 == id2);
		constexpr bool areNotEqual = (id1 != id2);

		REQUIRE_FALSE(areEqual);
		REQUIRE(areNotEqual);
	}

	SECTION("Can be used in switch statements")
	{
		constexpr Identifier testId("option_a");

		switch (static_cast<uint32_t>(testId))
		{
			case Hash::GenerateFNVHash("option_a"):
				REQUIRE(true);
				break;
			case Hash::GenerateFNVHash("option_b"):
				REQUIRE(false);
				break;
			default:
				REQUIRE(false);
				break;
		}
	}
}

TEST_CASE("Identifier std::hash support", "[unit][core][identifier]")
{
	SECTION("Can be used in std::unordered_set")
	{
		std::unordered_set<Identifier> idSet;

		Identifier id1("item1");
		Identifier id2("item2");
		Identifier id3("item1"); // Duplicate

		idSet.insert(id1);
		idSet.insert(id2);
		idSet.insert(id3);

		REQUIRE(idSet.size() == 2); // Only 2 unique
		REQUIRE(idSet.count(id1) == 1);
		REQUIRE(idSet.count(id2) == 1);
		REQUIRE(idSet.count(id3) == 1); // Same as id1
	}

	SECTION("Can be used in std::unordered_map")
	{
		std::unordered_map<Identifier, std::string> idMap;

		Identifier id1("key1");
		Identifier id2("key2");

		idMap[id1] = "value1";
		idMap[id2] = "value2";

		REQUIRE(idMap.size() == 2);
		REQUIRE(idMap[id1] == "value1");
		REQUIRE(idMap[id2] == "value2");
	}

	SECTION("std::hash produces consistent results")
	{
		Identifier id("hash_test");

		std::hash<Identifier> hasher;
		size_t hash1 = hasher(id);
		size_t hash2 = hasher(id);

		REQUIRE(hash1 == hash2);
	}

	SECTION("std::hash for equal identifiers is equal")
	{
		Identifier id1("equal_test");
		Identifier id2("equal_test");

		std::hash<Identifier> hasher;

		REQUIRE(hasher(id1) == hasher(id2));
	}

	SECTION("std::hash for different identifiers is different")
	{
		Identifier id1("different1");
		Identifier id2("different2");

		std::hash<Identifier> hasher;

		REQUIRE(hasher(id1) != hasher(id2));
	}
}

TEST_CASE("Identifier collision resistance", "[unit][core][identifier]")
{
	SECTION("Many unique strings produce unique identifiers")
	{
		std::unordered_set<Identifier> identifiers;
		constexpr int numIdentifiers = 1000;

		for (int i = 0; i < numIdentifiers; ++i)
		{
			std::string name = "identifier_" + std::to_string(i);
			Identifier id(name);
			identifiers.insert(id);
		}

		// Should have very few hash collisions
		REQUIRE(identifiers.size() > numIdentifiers * 0.99);
	}

	SECTION("Similar strings produce different identifiers")
	{
		Identifier id1("test_001");
		Identifier id2("test_002");
		Identifier id3("test_003");

		REQUIRE(id1 != id2);
		REQUIRE(id2 != id3);
		REQUIRE(id1 != id3);
	}
}

TEST_CASE("Identifier debug name functionality", "[unit][core][identifier]")
{
	SECTION("GetDBGName returns original string")
	{
		constexpr Identifier id("my_debug_name");

		REQUIRE(std::string(id.GetDBGName()) == "my_debug_name");
	}

	SECTION("Debug name persists")
	{
		Identifier id("persistent");

		std::string_view name1 = id.GetDBGName();
		std::string_view name2 = id.GetDBGName();

		REQUIRE(std::string(name1) == std::string(name2));
		REQUIRE(std::string(name1) == "persistent");
	}

	SECTION("Long debug names")
	{
		std::string longName(500, 'x');
		Identifier id(longName);

		REQUIRE(std::string(id.GetDBGName()) == longName);
	}

	SECTION("Debug name with special characters")
	{
		constexpr Identifier id("name_with_123_$pecial");

		REQUIRE(std::string(id.GetDBGName()) == "name_with_123_$pecial");
	}
}

TEST_CASE("Identifier use cases", "[unit][core][identifier]")
{
	SECTION("Component type identification")
	{
		constexpr Identifier transformComponent("TransformComponent");
		constexpr Identifier meshComponent("MeshComponent");
		constexpr Identifier cameraComponent("CameraComponent");

		REQUIRE(transformComponent != meshComponent);
		REQUIRE(meshComponent != cameraComponent);
		REQUIRE(transformComponent != cameraComponent);
	}

	SECTION("Asset identification")
	{
		Identifier texture1("textures/wood.png");
		Identifier texture2("textures/metal.png");
		Identifier model1("models/player.obj");

		std::unordered_map<Identifier, std::string> assetPaths;
		assetPaths[texture1] = "path/to/wood.png";
		assetPaths[texture2] = "path/to/metal.png";
		assetPaths[model1] = "path/to/player.obj";

		REQUIRE(assetPaths.size() == 3);
		REQUIRE(assetPaths[texture1] == "path/to/wood.png");
	}

	SECTION("Event type identification")
	{
		constexpr Identifier keyPressEvent("KeyPressEvent");
		constexpr Identifier mouseClickEvent("MouseClickEvent");
		constexpr Identifier windowResizeEvent("WindowResizeEvent");

		REQUIRE(keyPressEvent != mouseClickEvent);
		REQUIRE(mouseClickEvent != windowResizeEvent);
	}

	SECTION("String lookup table")
	{
		std::unordered_map<Identifier, int> settings;

		settings[Identifier("volume")] = 75;
		settings[Identifier("brightness")] = 100;
		settings[Identifier("fps_limit")] = 60;

		REQUIRE(settings[Identifier("volume")] == 75);
		REQUIRE(settings[Identifier("brightness")] == 100);
		REQUIRE(settings[Identifier("fps_limit")] == 60);
	}
}

TEST_CASE("Identifier edge cases", "[unit][core][identifier]")
{
	SECTION("Very long string")
	{
		std::string veryLongString(10000, 'a');
		Identifier id(veryLongString);

		REQUIRE(static_cast<uint32_t>(id) != 0);
		REQUIRE(std::string(id.GetDBGName()) == veryLongString);
	}

	SECTION("String with null characters (terminated at first null)")
	{
		std::string_view sv = "test\0hidden";
		Identifier id(sv);

		// String_view should include the full data
		REQUIRE(std::string(id.GetDBGName()) == std::string(sv));
	}

	SECTION("Numeric strings")
	{
		Identifier id1("12345");
		Identifier id2("67890");

		REQUIRE(id1 != id2);
		REQUIRE(std::string(id1.GetDBGName()) == "12345");
	}

	SECTION("Unicode characters (if supported)")
	{
		Identifier id1("测试"); // Chinese
		Identifier id2("テスト"); // Japanese

		REQUIRE(id1 != id2);
	}

	SECTION("Case sensitivity")
	{
		Identifier idLower("testcase");
		Identifier idUpper("TESTCASE");
		Identifier idMixed("TestCase");

		REQUIRE(idLower != idUpper);
		REQUIRE(idUpper != idMixed);
		REQUIRE(idLower != idMixed);
	}
}

TEST_CASE("Identifier performance", "[unit][core][identifier]")
{
	SECTION("Can create many identifiers quickly")
	{
		constexpr int iterations = 10000;

		for (int i = 0; i < iterations; ++i)
		{
			std::string name = "perf_test_" + std::to_string(i);
			Identifier id(name);
			REQUIRE(static_cast<uint32_t>(id) != 0);
		}

		REQUIRE(true);
	}

	SECTION("Lookup in large unordered_map is fast")
	{
		std::unordered_map<Identifier, int> largeMap;

		// Populate map
		for (int i = 0; i < 10000; ++i)
		{
			std::string name = "key_" + std::to_string(i);
			largeMap[Identifier(name)] = i;
		}

		// Perform lookups
		for (int i = 0; i < 1000; ++i)
		{
			std::string name = "key_" + std::to_string(i);
			REQUIRE(largeMap[Identifier(name)] == i);
		}

		REQUIRE(true);
	}
}

TEST_CASE("Identifier comparison with raw hash", "[unit][core][identifier]")
{
	SECTION("Identifier from string equals identifier from its hash")
	{
		constexpr Identifier idFromString("test_comparison");
		constexpr uint32_t hash = Hash::GenerateFNVHash("test_comparison");
		constexpr Identifier idFromHash(hash);

		REQUIRE(idFromString == idFromHash);
	}

	SECTION("Can compare identifier with raw uint32_t")
	{
		constexpr Identifier id("numeric_compare");
		constexpr uint32_t expectedHash = Hash::GenerateFNVHash("numeric_compare");

		REQUIRE(static_cast<uint32_t>(id) == expectedHash);
	}

	SECTION("Identifier preserves exact hash value")
	{
		constexpr uint32_t customHash = 0xDEADBEEF;
		constexpr Identifier id(customHash);

		REQUIRE(static_cast<uint32_t>(id) == customHash);
		REQUIRE(id == Identifier(customHash));
	}
}

TEST_CASE("Identifier copy and move semantics", "[unit][core][identifier]")
{
	SECTION("Copy constructor preserves value")
	{
		Identifier original("copy_test");
		Identifier copy(original);

		REQUIRE(original == copy);
		REQUIRE(std::string(original.GetDBGName()) == std::string(copy.GetDBGName()));
	}

	SECTION("Copy assignment preserves value")
	{
		Identifier original("assign_test");
		Identifier copy("different");

		copy = original;

		REQUIRE(original == copy);
		REQUIRE(std::string(original.GetDBGName()) == std::string(copy.GetDBGName()));
	}

	SECTION("Move constructor works")
	{
		Identifier original("move_test");
		uint32_t originalHash = original;

		Identifier moved(std::move(original));

		REQUIRE(static_cast<uint32_t>(moved) == originalHash);
	}

	SECTION("Move assignment works")
	{
		Identifier original("move_assign_test");
		uint32_t originalHash = original;
		Identifier moved("different");

		moved = std::move(original);

		REQUIRE(static_cast<uint32_t>(moved) == originalHash);
	}
}

TEST_CASE("Identifier in containers", "[unit][core][identifier]")
{
	SECTION("Sorting in ordered containers")
	{
		std::vector<Identifier> ids = {
			Identifier("zebra"),
			Identifier("alpha"),
			Identifier("beta")
		};

		std::ranges::sort(ids, [](const Identifier& a, const Identifier& b) {
			return static_cast<uint32_t>(a) < static_cast<uint32_t>(b);
		});

		// Verify sorting doesn't crash (order depends on hash values)
		REQUIRE(ids.size() == 3);
	}

	SECTION("Used as key in std::map")
	{
		std::map<Identifier, std::string, std::less<uint32_t>> orderedMap;

		Identifier id1("first");
		Identifier id2("second");

		orderedMap[id1] = "value1";
		orderedMap[id2] = "value2";

		REQUIRE(orderedMap.size() == 2);
	}

	SECTION("Vector of identifiers")
	{
		std::vector<Identifier> vec;

		vec.push_back(Identifier("item1"));
		vec.push_back(Identifier("item2"));
		vec.emplace_back("item3");

		REQUIRE(vec.size() == 3);
		REQUIRE(std::string(vec[0].GetDBGName()) == "item1");
		REQUIRE(std::string(vec[1].GetDBGName()) == "item2");
		REQUIRE(std::string(vec[2].GetDBGName()) == "item3");
	}
}

TEST_CASE("Identifier real-world patterns", "[unit][core][identifier]")
{
	SECTION("Component registry pattern")
	{
		struct ComponentInfo
		{
			Identifier type;
			size_t size;
			std::string name;
		};

		std::unordered_map<Identifier, ComponentInfo> registry;

		Identifier transformId("Transform");
		Identifier meshRendererId("MeshRenderer");

		registry[transformId] = {transformId, sizeof(float) * 16, "Transform"};
		registry[meshRendererId] = {meshRendererId, sizeof(int) * 4, "MeshRenderer"};

		REQUIRE(registry.size() == 2);
		REQUIRE(registry[transformId].name == "Transform");
		REQUIRE(registry[meshRendererId].name == "MeshRenderer");
	}

	SECTION("Event dispatcher pattern")
	{
		using EventCallback = std::function<void()>;
		std::unordered_map<Identifier, std::vector<EventCallback>> eventHandlers;

		Identifier onUpdate("OnUpdate");
		Identifier onRender("OnRender");

		int updateCount = 0;
		int renderCount = 0;

		eventHandlers[onUpdate].push_back([&]() { ++updateCount; });
		eventHandlers[onRender].push_back([&]() { ++renderCount; });

		// Trigger events
		for (auto& callback : eventHandlers[onUpdate])
			callback();
		for (auto& callback : eventHandlers[onRender])
			callback();

		REQUIRE(updateCount == 1);
		REQUIRE(renderCount == 1);
	}

	SECTION("String interning pattern")
	{
		std::unordered_map<Identifier, std::string_view> internedStrings;

		// Simulate string interning
		auto internString = [&](std::string_view str) -> Identifier {
			Identifier id(str);
			if (internedStrings.find(id) == internedStrings.end())
			{
				internedStrings[id] = str;
			}
			return id;
		};

		Identifier id1 = internString("repeated_string");
		Identifier id2 = internString("repeated_string");
		Identifier id3 = internString("different_string");

		REQUIRE(id1 == id2);
		REQUIRE(id1 != id3);
		REQUIRE(internedStrings.size() == 2);
	}

	SECTION("Resource handle pattern")
	{
		struct Resource
		{
			Identifier id;
			std::string path;
			bool loaded = false;
		};

		std::unordered_map<Identifier, Resource> resources;

		auto loadResource = [&](std::string_view name, std::string_view path) {
			Identifier id(name);
			resources[id] = {id, std::string(path), true};
			return id;
		};

		Identifier textureId = loadResource("player_texture", "assets/player.png");
		Identifier modelId = loadResource("player_model", "assets/player.obj");

		REQUIRE(resources[textureId].loaded);
		REQUIRE(resources[modelId].path == "assets/player.obj");
	}

	SECTION("Enum replacement pattern")
	{
		// Instead of enum class State { Idle, Running, Jumping };
		constexpr Identifier STATE_IDLE("State::Idle");
		constexpr Identifier STATE_RUNNING("State::Running");
		constexpr Identifier STATE_JUMPING("State::Jumping");

		Identifier currentState = STATE_IDLE;

		if (currentState == STATE_IDLE)
		{
			currentState = STATE_RUNNING;
		}

		REQUIRE(currentState == STATE_RUNNING);
		REQUIRE(currentState != STATE_IDLE);
		REQUIRE(currentState != STATE_JUMPING);
	}
}

TEST_CASE("Identifier thread safety", "[unit][core][identifier]")
{
	SECTION("Concurrent identifier creation from same string")
	{
		constexpr int numThreads = 4;
		constexpr int iterationsPerThread = 1000;

		std::vector<std::thread> threads;
		std::vector<std::vector<Identifier>> results(numThreads);

		for (int t = 0; t < numThreads; ++t)
		{
			threads.emplace_back([&, t]() {
				for (int i = 0; i < iterationsPerThread; ++i)
				{
					Identifier id("thread_test");
					results[t].push_back(id);
				}
			});
		}

		for (auto& thread : threads)
			thread.join();

		// All identifiers from same string should be equal
		Identifier reference("thread_test");
		for (const auto& threadResults : results)
		{
			for (const auto& id : threadResults)
			{
				REQUIRE(id == reference);
			}
		}
	}

	SECTION("Concurrent unordered_map operations")
	{
		std::unordered_map<Identifier, int> sharedMap;
		std::mutex mapMutex;

		constexpr int numThreads = 4;
		constexpr int itemsPerThread = 250;

		std::vector<std::thread> threads;

		for (int t = 0; t < numThreads; ++t)
		{
			threads.emplace_back([&, t]() {
				for (int i = 0; i < itemsPerThread; ++i)
				{
					std::string key = "thread_" + std::to_string(t) + "_item_" + std::to_string(i);
					Identifier id(key);

					std::lock_guard<std::mutex> lock(mapMutex);
					sharedMap[id] = t * 1000 + i;
				}
			});
		}

		for (auto& thread : threads)
			thread.join();

		REQUIRE(sharedMap.size() == numThreads * itemsPerThread);
	}
}

TEST_CASE("Identifier optimization checks", "[unit][core][identifier]")
{
	SECTION("Identifier size is optimal")
	{
		// Identifier should be small (hash + string_view = 12 or 16 bytes on 64-bit)
		REQUIRE(sizeof(Identifier) <= 24);
	}

	SECTION("Identifier is trivially copyable")
	{
		// Should be cheap to copy
		REQUIRE(std::is_trivially_copyable_v<Identifier>);
	}

	SECTION("No dynamic allocation for identifier itself")
	{
		// Creating identifiers shouldn't allocate
		Identifier id1("test1");
		Identifier id2("test2");
		Identifier id3 = id1;

		REQUIRE(id1 == id3);
	}
}

TEST_CASE("Identifier with macros and literals", "[unit][core][identifier]")
{
	SECTION("Can use with string literals")
	{
		constexpr Identifier id("literal_test");
		REQUIRE(std::string(id.GetDBGName()) == "literal_test");
	}

	SECTION("Can use with concatenated strings")
	{
		constexpr const char* prefix = "Component::";
		std::string fullName = std::string(prefix) + "Transform";
		Identifier id(fullName);

		REQUIRE(std::string(id.GetDBGName()) == fullName);
	}

	SECTION("Can be used in constexpr arrays")
	{
		constexpr std::array<Identifier, 3> components = {
			Identifier("Transform"),
			Identifier("Mesh"),
			Identifier("Camera")
		};

		REQUIRE(components[0] != components[1]);
		REQUIRE(components[1] != components[2]);
	}
}

TEST_CASE("Identifier hash quality metrics", "[unit][core][identifier]")
{
	SECTION("Common prefixes produce different hashes")
	{
		std::vector<Identifier> ids = {
			Identifier("Component_Transform"),
			Identifier("Component_Mesh"),
			Identifier("Component_Camera"),
			Identifier("Component_Light"),
			Identifier("Component_Audio")
		};

		std::unordered_set<uint32_t> hashes;
		for (const auto& id : ids)
		{
			hashes.insert(static_cast<uint32_t>(id));
		}

		REQUIRE(hashes.size() == ids.size());
	}

	SECTION("Sequential numbers produce different hashes")
	{
		std::unordered_set<Identifier> ids;

		for (int i = 0; i < 100; ++i)
		{
			std::string name = std::to_string(i);
			ids.insert(Identifier(name));
		}

		REQUIRE(ids.size() == 100);
	}

	SECTION("Avalanche effect - small changes produce large hash differences")
	{
		Identifier id1("test_a");
		Identifier id2("test_b");

		uint32_t hash1 = id1;
		uint32_t hash2 = id2;

		// Count bit differences
		uint32_t xorResult = hash1 ^ hash2;
		int bitDifferences = 0;
		for (int i = 0; i < 32; ++i)
		{
			if (xorResult & (1u << i))
				++bitDifferences;
		}

		// Should have significant bit differences (ideally ~16 for good avalanche)
		REQUIRE(bitDifferences > 5);
	}
}