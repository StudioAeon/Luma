#pragma once

#include "../TestLayer.hpp"

#include <format>

namespace Luma {

	class RefCountTest : public Test
	{
	public:
		const char* GetName() const override { return "Reference Count Test"; }
		const char* GetCategory() const override { return "Memory"; }

		struct TestObject : public RefCounted
		{
			TestObject(int v) : value(v) {}
			int value;
		};

		void OnImGuiRender() override
		{
			ImGui::Text("Ref<T> reference counting test");
			ImGui::Text("Creates and copies Ref<TestObject> instances");
			ImGui::Separator();

			if (m_TestRef)
			{
				ImGui::Text("Test object value: %d", m_TestRef->value);
				ImGui::Text("Reference count: %u", m_TestRef->GetRefCount());
			}
		}

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			try
			{
				Ref<TestObject> ref1 = Ref<TestObject>::Create(42);

				if (ref1->GetRefCount() != 1)
				{
					result.Passed = false;
					result.Message = std::format("Expected ref count 1, got {}", ref1->GetRefCount());
					return result;
				}

				Ref<TestObject> ref2 = ref1;

				if (ref1->GetRefCount() != 2)
				{
					result.Passed = false;
					result.Message = std::format("Expected ref count 2 after copy, got {}", ref1->GetRefCount());
					return result;
				}

				ref2.Reset();

				if (ref1->GetRefCount() != 1)
				{
					result.Passed = false;
					result.Message = std::format("Expected ref count 1 after release, got {}", ref1->GetRefCount());
					return result;
				}

				m_TestRef = ref1;
				result.Message = "Reference counting working correctly";
			}
			catch (const std::exception& e)
			{
				result.Passed = false;
				result.Message = std::format("Exception: {}", e.what());
			}

			return result;
		}

	private:
		Ref<TestObject> m_TestRef;
	};

	class BufferTest : public Test
	{
	public:
		const char* GetName() const override { return "Buffer Test"; }
		const char* GetCategory() const override { return "Memory"; }

		void OnImGuiRender() override
		{
			if (!m_Buffer)
				return;

			ImGui::Text("Buffer size: %zu bytes", m_Buffer.Size);
			ImGui::Text("Buffer data pointer: %p", m_Buffer.Data);

			if (m_Buffer.Size > 0)
			{
				ImGui::Text("First 16 bytes:");
				ImGui::BeginChild("BufferData", ImVec2(0, 80), true);

				uint8_t* data = m_Buffer.As<uint8_t>();
				size_t displaySize = std::min(m_Buffer.Size, size_t(16));

				for (size_t i = 0; i < displaySize; ++i)
					ImGui::Text("%02zu: 0x%02X (%u)", i, data[i], data[i]);

				ImGui::EndChild();
			}
		}

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			try
			{
				constexpr size_t bufferSize = 1024;
				m_Buffer.Allocate(bufferSize);
				m_Buffer.ZeroInitialize();

				if (!m_Buffer.Data)
				{
					result.Passed = false;
					result.Message = "Buffer allocation failed";
					return result;
				}

				if (m_Buffer.Size != bufferSize)
				{
					result.Passed = false;
					result.Message = std::format("Expected size {}, got {}", bufferSize, m_Buffer.Size);
					return result;
				}

				uint8_t* data = m_Buffer.As<uint8_t>();
				for (size_t i = 0; i < bufferSize; ++i)
					data[i] = static_cast<uint8_t>(i % 256);

				for (size_t i = 0; i < bufferSize; ++i)
				{
					if (data[i] != static_cast<uint8_t>(i % 256))
					{
						result.Passed = false;
						result.Message = "Buffer data verification failed";
						return result;
					}
				}

				result.Message = std::format("Buffer allocated and verified ({} bytes)", bufferSize);
			}
			catch (const std::exception& e)
			{
				result.Passed = false;
				result.Message = std::format("Exception: {}", e.what());
			}

			return result;
		}

	private:
		Buffer m_Buffer;
	};

}