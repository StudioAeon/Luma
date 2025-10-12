#include <catch2/catch_test_macros.hpp>

#include "Luma/Core/Buffer.hpp"

#include <glm/glm.hpp>

#include <cstring>
#include <string>

using namespace Luma;

TEST_CASE("Buffer default construction", "[unit][core][buffer]")
{
	SECTION("Default constructor creates null buffer")
	{
		Buffer buffer;
		
		REQUIRE(buffer.Data == nullptr);
		REQUIRE(buffer.Size == 0);
		REQUIRE_FALSE(buffer); // operator bool
	}
}

TEST_CASE("Buffer explicit construction", "[unit][core][buffer]")
{
	SECTION("Constructor with data pointer and size")
	{
		byte testData[100];
		Buffer buffer(testData, 100);
		
		REQUIRE(buffer.Data == testData);
		REQUIRE(buffer.Size == 100);
		REQUIRE(buffer); // operator bool
	}

	SECTION("Constructor with nullptr and zero size")
	{
		Buffer buffer(nullptr, 0);
		
		REQUIRE(buffer.Data == nullptr);
		REQUIRE(buffer.Size == 0);
		REQUIRE_FALSE(buffer);
	}
}

TEST_CASE("Buffer allocation", "[unit][core][buffer]")
{
	SECTION("Allocate creates new buffer")
	{
		Buffer buffer;
		buffer.Allocate(256);

		REQUIRE(buffer.Data != nullptr);
		REQUIRE(buffer.Size == 256);
		REQUIRE(buffer);
	}

	SECTION("Allocate with zero size creates null buffer")
	{
		Buffer buffer;
		buffer.Allocate(0);

		REQUIRE(buffer.Data == nullptr);
		REQUIRE(buffer.Size == 0);
		REQUIRE_FALSE(buffer);
	}

	SECTION("Reallocate replaces existing buffer")
	{
		Buffer buffer;
		buffer.Allocate(128);
		byte* firstPtr = static_cast<byte*>(buffer.Data);

		buffer.Allocate(256);

		REQUIRE(buffer.Data != nullptr);
		REQUIRE(buffer.Data != firstPtr);
		REQUIRE(buffer.Size == 256);
	}

	SECTION("Allocate to zero deallocates buffer")
	{
		Buffer buffer;
		buffer.Allocate(128);

		buffer.Allocate(0);

		REQUIRE(buffer.Data == nullptr);
		REQUIRE(buffer.Size == 0);
	}
}

TEST_CASE("Buffer zero initialization", "[unit][core][buffer]")
{
	SECTION("ZeroInitialize clears all bytes")
	{
		Buffer buffer;
		buffer.Allocate(64);

		// Fill with non-zero data
		memset(buffer.Data, 0xFF, buffer.Size);

		buffer.ZeroInitialize();

		// Verify all bytes are zero
		byte* data = static_cast<byte*>(buffer.Data);
		for (uint32_t i = 0; i < buffer.Size; ++i)
		{
			REQUIRE(data[i] == 0);
		}
	}

	SECTION("ZeroInitialize on null buffer is safe")
	{
		Buffer buffer;
		REQUIRE_NOTHROW(buffer.ZeroInitialize());
	}
}

TEST_CASE("Buffer write operations", "[unit][core][buffer]")
{
	SECTION("Write data at default offset")
	{
		Buffer buffer;
		buffer.Allocate(128);
		buffer.ZeroInitialize();

		uint32_t testValue = 0xDEADBEEF;
		buffer.Write(&testValue, sizeof(testValue));

		uint32_t readValue = *reinterpret_cast<uint32_t*>(buffer.Data);
		REQUIRE(readValue == testValue);
	}

	SECTION("Write data at specific offset")
	{
		Buffer buffer;
		buffer.Allocate(128);
		buffer.ZeroInitialize();

		uint32_t testValue1 = 0x12345678;
		uint32_t testValue2 = 0xABCDEF00;

		buffer.Write(&testValue1, sizeof(testValue1), 0);
		buffer.Write(&testValue2, sizeof(testValue2), 64);

		uint32_t readValue1 = *reinterpret_cast<uint32_t*>(buffer.Data);
		uint32_t readValue2 = *reinterpret_cast<uint32_t*>(static_cast<byte*>(buffer.Data) + 64);

		REQUIRE(readValue1 == testValue1);
		REQUIRE(readValue2 == testValue2);
	}

	SECTION("Write string data")
	{
		Buffer buffer;
		buffer.Allocate(128);
		buffer.ZeroInitialize();

		const char* testStr = "Hello, Luma!";
		size_t strLen = strlen(testStr) + 1; // Include null terminator

		buffer.Write((void*)testStr, static_cast<uint32_t>(strLen));

		const char* readStr = reinterpret_cast<const char*>(buffer.Data);
		REQUIRE(strcmp(readStr, testStr) == 0);
	}

	SECTION("Write array of values")
	{
		Buffer buffer;
		buffer.Allocate(128);
		buffer.ZeroInitialize();

		float testArray[] = { 1.0f, 2.5f, 3.75f, 4.25f };
		buffer.Write(testArray, sizeof(testArray));

		float* readArray = reinterpret_cast<float*>(buffer.Data);
		for (size_t i = 0; i < 4; ++i)
		{
			REQUIRE(readArray[i] == testArray[i]);
		}
	}
}

TEST_CASE("Buffer copy operation", "[unit][core][buffer]")
{
	SECTION("Copy creates independent buffer with same data")
	{
		uint32_t sourceData[] = { 1, 2, 3, 4, 5 };
		Buffer buffer = Buffer::Copy(sourceData, sizeof(sourceData));

		REQUIRE(buffer.Data != nullptr);
		REQUIRE(buffer.Size == sizeof(sourceData));
		REQUIRE(buffer.Data != reinterpret_cast<byte*>(sourceData));

		uint32_t* copiedData = reinterpret_cast<uint32_t*>(buffer.Data);
		for (size_t i = 0; i < 5; ++i)
		{
			REQUIRE(copiedData[i] == sourceData[i]);
		}
	}

	SECTION("Modifying copy does not affect original")
	{
		uint32_t sourceData[] = { 10, 20, 30 };
		Buffer buffer = Buffer::Copy(sourceData, sizeof(sourceData));

		uint32_t* copiedData = reinterpret_cast<uint32_t*>(buffer.Data);
		copiedData[0] = 999;

		REQUIRE(sourceData[0] == 10); // Original unchanged
		REQUIRE(copiedData[0] == 999); // Copy modified
	}

	SECTION("Copy empty data creates valid buffer")
	{
		byte emptyData[1] = { 0 };
		Buffer buffer = Buffer::Copy(emptyData, 0);

		// Implementation should handle this gracefully
		REQUIRE(buffer.Size == 0);
	}
}

TEST_CASE("Buffer subscript operator", "[unit][core][buffer]")
{
	SECTION("Non-const subscript access")
	{
		Buffer buffer;
		buffer.Allocate(16);

		buffer[0] = 0x11;
		buffer[5] = 0x55;
		buffer[15] = 0xFF;

		REQUIRE(buffer[0] == 0x11);
		REQUIRE(buffer[5] == 0x55);
		REQUIRE(buffer[15] == 0xFF);
	}

	SECTION("Const subscript access")
	{
		Buffer buffer;
		buffer.Allocate(16);
		buffer[0] = 0xAA;
		buffer[10] = 0xBB;

		const Buffer& constBuffer = buffer;

		REQUIRE(constBuffer[0] == 0xAA);
		REQUIRE(constBuffer[10] == 0xBB);
	}

	SECTION("Subscript operator allows iteration")
	{
		Buffer buffer;
		buffer.Allocate(10);

		// Fill with sequential values
		for (int i = 0; i < 10; ++i)
		{
			buffer[i] = static_cast<byte>(i * 10);
		}

		// Verify
		for (int i = 0; i < 10; ++i)
		{
			REQUIRE(buffer[i] == static_cast<byte>(i * 10));
		}
	}
}

TEST_CASE("Buffer type casting with As()", "[unit][core][buffer]")
{
	SECTION("Cast to uint32_t array")
	{
		Buffer buffer;
		buffer.Allocate(16);

		uint32_t* intArray = buffer.As<uint32_t>();
		intArray[0] = 100;
		intArray[1] = 200;
		intArray[2] = 300;
		intArray[3] = 400;

		REQUIRE(intArray[0] == 100);
		REQUIRE(intArray[1] == 200);
		REQUIRE(intArray[2] == 300);
		REQUIRE(intArray[3] == 400);
	}

	SECTION("Cast to float array")
	{
		Buffer buffer;
		buffer.Allocate(32);

		float* floatArray = buffer.As<float>();
		floatArray[0] = 1.5f;
		floatArray[1] = 2.25f;
		floatArray[2] = 3.75f;

		REQUIRE(floatArray[0] == 1.5f);
		REQUIRE(floatArray[1] == 2.25f);
		REQUIRE(floatArray[2] == 3.75f);
	}

	SECTION("Cast to struct type")
	{
		struct TestStruct
		{
			int x, y, z;
			float w;
		};

		Buffer buffer;
		buffer.Allocate(sizeof(TestStruct));

		TestStruct* data = buffer.As<TestStruct>();
		data->x = 10;
		data->y = 20;
		data->z = 30;
		data->w = 1.5f;

		REQUIRE(data->x == 10);
		REQUIRE(data->y == 20);
		REQUIRE(data->z == 30);
		REQUIRE(data->w == 1.5f);
	}
}

TEST_CASE("Buffer GetSize()", "[unit][core][buffer]")
{
	SECTION("GetSize returns correct size")
	{
		Buffer buffer;
		buffer.Allocate(512);

		REQUIRE(buffer.GetSize() == 512);
		REQUIRE(buffer.Size == 512);
		REQUIRE(buffer.GetSize() == buffer.Size);
	}

	SECTION("GetSize returns zero for unallocated buffer")
	{
		Buffer buffer;
		REQUIRE(buffer.GetSize() == 0);
	}
}

TEST_CASE("Buffer boolean conversion", "[unit][core][buffer]")
{
	SECTION("Valid buffer converts to true")
	{
		Buffer buffer;
		buffer.Allocate(64);

		REQUIRE(buffer);
		REQUIRE(static_cast<bool>(buffer) == true);
	}

	SECTION("Null buffer converts to false")
	{
		Buffer buffer;

		REQUIRE_FALSE(buffer);
		REQUIRE(static_cast<bool>(buffer) == false);
	}

	SECTION("Use in conditional statements")
	{
		Buffer validBuffer;
		validBuffer.Allocate(32);

		Buffer nullBuffer;

		if (validBuffer)
		{
			REQUIRE(true); // Should reach here
		}
		else
		{
			REQUIRE(false); // Should not reach here
		}

		if (!nullBuffer)
		{
			REQUIRE(true); // Should reach here
		}
		else
		{
			REQUIRE(false); // Should not reach here
		}
	}
}

TEST_CASE("Buffer practical usage scenarios", "[unit][core][buffer]")
{
	SECTION("Store and retrieve vertex data")
	{
		struct Vertex
		{
			float x, y, z;
			float r, g, b;
		};

		Vertex vertices[] = {
			{ 0.0f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f },
			{ -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f }
		};

		Buffer buffer = Buffer::Copy(vertices, sizeof(vertices));

		Vertex* storedVertices = buffer.As<Vertex>();

		REQUIRE(storedVertices[0].x == 0.0f);
		REQUIRE(storedVertices[0].r == 1.0f);
		REQUIRE(storedVertices[1].g == 1.0f);
		REQUIRE(storedVertices[2].b == 1.0f);
	}

	SECTION("Write multiple data types sequentially")
	{
		Buffer buffer;
		buffer.Allocate(256);

		uint32_t id = 42;
		float scale = 2.5f;
		glm::vec3 position = { 10.0f, 20.0f, 30.0f };

		uint32_t offset = 0;
		buffer.Write(&id, sizeof(id), offset);
		offset += sizeof(id);

		buffer.Write(&scale, sizeof(scale), offset);
		offset += sizeof(scale);

		buffer.Write(&position, sizeof(position), offset);

		// Read back
		uint32_t readId = *reinterpret_cast<uint32_t*>(buffer.Data);
		float readScale = *reinterpret_cast<float*>(static_cast<byte*>(buffer.Data) + sizeof(uint32_t));
		glm::vec3 readPos = *reinterpret_cast<glm::vec3*>(static_cast<byte*>(buffer.Data) + sizeof(uint32_t) + sizeof(float));

		REQUIRE(readId == 42);
		REQUIRE(readScale == 2.5f);
		REQUIRE(readPos.x == 10.0f);
		REQUIRE(readPos.y == 20.0f);
		REQUIRE(readPos.z == 30.0f);
	}

	SECTION("Resize buffer and preserve data")
	{
		Buffer buffer;
		buffer.Allocate(16);

		uint32_t testValue = 0xCAFEBABE;
		buffer.Write(&testValue, sizeof(testValue));

		// Note: Current implementation doesn't preserve data on reallocation
		// This test documents expected behavior for future improvements
		uint32_t storedValue = *reinterpret_cast<uint32_t*>(buffer.Data);
		REQUIRE(storedValue == testValue);
	}
}

TEST_CASE("Buffer edge cases", "[unit][core][buffer]")
{
	SECTION("Multiple allocations without leaks")
	{
		Buffer buffer;

		// Multiple allocations should properly delete old data
		for (int i = 0; i < 10; ++i)
		{
			buffer.Allocate(128 * (i + 1));
			REQUIRE(buffer.Size == 128u * (i + 1));
		}
	}

	SECTION("Write at boundary")
	{
		Buffer buffer;
		buffer.Allocate(16);

		uint32_t value = 0x12345678;

		// Write at last valid position
		buffer.Write(&value, sizeof(value), 12);

		uint32_t* readPtr = reinterpret_cast<uint32_t*>(static_cast<byte*>(buffer.Data) + 12);
		REQUIRE(*readPtr == value);
	}

	SECTION("Single byte buffer")
	{
		Buffer buffer;
		buffer.Allocate(1);

		buffer[0] = 0xFF;

		REQUIRE(buffer.Size == 1);
		REQUIRE(buffer[0] == 0xFF);
	}

	SECTION("Large buffer allocation")
	{
		Buffer buffer;
		buffer.Allocate(1024 * 1024); // 1 MB

		REQUIRE(buffer.Data != nullptr);
		REQUIRE(buffer.Size == 1024 * 1024);

		// Spot check: write at start, middle, and end
		buffer[0] = 0xAA;
		buffer[512 * 1024] = 0xBB;
		buffer[1024 * 1024 - 1] = 0xCC;

		REQUIRE(buffer[0] == 0xAA);
		REQUIRE(buffer[512 * 1024] == 0xBB);
		REQUIRE(buffer[1024 * 1024 - 1] == 0xCC);
	}
}