#pragma once

#include "../TestLayer.hpp"
#include "Luma/Serialization/Serialization.hpp"
#include "Luma/Serialization/MemoryStream.hpp"

#include <yaml-cpp/yaml.h>

#include <format>

namespace Luma {

	class YAMLSerializationTest : public Test
	{
	public:
		const char* GetName() const override { return "YAML Serialization"; }
		const char* GetCategory() const override { return "Serialization"; }

		struct TestData
		{
			std::string Name;
			int Value;
			float FloatValue;
			bool BoolValue;
		};

		void OnImGuiRender() override
		{
			ImGui::Text("Test Data:");
			ImGui::Separator();
			ImGui::Text("Name: %s", m_TestData.Name.c_str());
			ImGui::Text("Value: %d", m_TestData.Value);
			ImGui::Text("Float: %.2f", m_TestData.FloatValue);
			ImGui::Text("Bool: %s", m_TestData.BoolValue ? "true" : "false");

			if (!m_SerializedYAML.empty())
			{
				ImGui::Separator();
				ImGui::Text("Serialized YAML:");
				ImGui::BeginChild("YAML", ImVec2(0, 150), true);
				ImGui::TextWrapped("%s", m_SerializedYAML.c_str());
				ImGui::EndChild();
			}
		}

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			try
			{
				m_TestData.Name = "TestObject";
				m_TestData.Value = 42;
				m_TestData.FloatValue = 3.14159f;
				m_TestData.BoolValue = true;

				YAML::Emitter out;
				out << YAML::BeginMap;
				out << YAML::Key << "Name" << YAML::Value << m_TestData.Name;
				out << YAML::Key << "Value" << YAML::Value << m_TestData.Value;
				out << YAML::Key << "FloatValue" << YAML::Value << m_TestData.FloatValue;
				out << YAML::Key << "BoolValue" << YAML::Value << m_TestData.BoolValue;
				out << YAML::EndMap;

				m_SerializedYAML = out.c_str();

				YAML::Node node = YAML::Load(m_SerializedYAML);
				TestData deserializedData;
				deserializedData.Name = node["Name"].as<std::string>();
				deserializedData.Value = node["Value"].as<int>();
				deserializedData.FloatValue = node["FloatValue"].as<float>();
				deserializedData.BoolValue = node["BoolValue"].as<bool>();

				if (deserializedData.Name != m_TestData.Name ||
					deserializedData.Value != m_TestData.Value ||
					std::abs(deserializedData.FloatValue - m_TestData.FloatValue) > 0.0001f ||
					deserializedData.BoolValue != m_TestData.BoolValue)
				{
					result.Passed = false;
					result.Message = "Deserialized data does not match original";
					return result;
				}

				result.Message = "YAML serialization/deserialization successful";
			}
			catch (const std::exception& e)
			{
				result.Passed = false;
				result.Message = std::format("Exception: {}", e.what());
			}

			return result;
		}

	private:
		TestData m_TestData;
		std::string m_SerializedYAML;
	};

	class BinarySerializationTest : public Test
	{
	public:
		const char* GetName() const override { return "Binary Serialization"; }
		const char* GetCategory() const override { return "Serialization"; }

		struct BinaryTestData
		{
			int32_t IntValue;
			float FloatValue;
			double DoubleValue;
			std::string StringValue;
		};

		void OnImGuiRender() override
		{
			ImGui::Text("Binary Test Data:");
			ImGui::Separator();
			ImGui::Text("Int: %d", m_TestData.IntValue);
			ImGui::Text("Float: %.2f", m_TestData.FloatValue);
			ImGui::Text("Double: %.6f", m_TestData.DoubleValue);
			ImGui::Text("String: %s", m_TestData.StringValue.c_str());

			if (m_SerializedBuffer)
			{
				ImGui::Separator();
				ImGui::Text("Serialized size: %zu bytes", m_SerializedBuffer.Size);
				ImGui::Text("Binary data (hex):");
				ImGui::BeginChild("BinaryData", ImVec2(0, 150), true);

				uint8_t* data = m_SerializedBuffer.As<uint8_t>();
				size_t displaySize = std::min(m_SerializedBuffer.Size, size_t(64));

				for (size_t i = 0; i < displaySize; ++i)
				{
					if (i % 16 == 0 && i > 0)
						ImGui::Text("");
					ImGui::SameLine();
					ImGui::Text("%02X ", data[i]);
				}

				ImGui::EndChild();
			}
		}

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			try
			{
				m_TestData.IntValue = 12345;
				m_TestData.FloatValue = 67.89f;
				m_TestData.DoubleValue = 3.141592653589793;
				m_TestData.StringValue = "BinaryTest";

				constexpr size_t estimatedSize = 1024;
				m_SerializedBuffer.Allocate(estimatedSize);
				m_SerializedBuffer.ZeroInitialize();

				MemoryStreamWriter writer(m_SerializedBuffer, estimatedSize);
				writer.WriteRaw(m_TestData.IntValue);
				writer.WriteRaw(m_TestData.FloatValue);
				writer.WriteRaw(m_TestData.DoubleValue);
				writer.WriteString(m_TestData.StringValue);

				MemoryStreamReader reader(m_SerializedBuffer);
				BinaryTestData deserializedData;
				reader.ReadRaw(deserializedData.IntValue);
				reader.ReadRaw(deserializedData.FloatValue);
				reader.ReadRaw(deserializedData.DoubleValue);
				reader.ReadString(deserializedData.StringValue);

				if (deserializedData.IntValue != m_TestData.IntValue ||
					std::abs(deserializedData.FloatValue - m_TestData.FloatValue) > 0.0001f ||
					std::abs(deserializedData.DoubleValue - m_TestData.DoubleValue) > 0.000001 ||
					deserializedData.StringValue != m_TestData.StringValue)
				{
					result.Passed = false;
					result.Message = "Deserialized data does not match original";
					return result;
				}

				result.Message = std::format("Binary serialization successful ({} bytes)",
					m_SerializedBuffer.Size);
			}
			catch (const std::exception& e)
			{
				result.Passed = false;
				result.Message = std::format("Exception: {}", e.what());
			}

			return result;
		}

	private:
		BinaryTestData m_TestData;
		Buffer m_SerializedBuffer;
	};

}