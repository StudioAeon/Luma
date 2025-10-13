#pragma once

#include "../TestLayer.hpp"

#include <format>

namespace Luma {

	class InputStateTest : public Test
	{
	public:
		const char* GetName() const override { return "Input State Test"; }
		const char* GetCategory() const override { return "Input System"; }

		void OnUpdate(Timestep ts) override
		{
			m_WKeyPressed = Input::IsKeyPressed(KeyCode::W);
			m_AKeyPressed = Input::IsKeyPressed(KeyCode::A);
			m_SKeyPressed = Input::IsKeyPressed(KeyCode::S);
			m_DKeyPressed = Input::IsKeyPressed(KeyCode::D);
			m_SpacePressed = Input::IsKeyPressed(KeyCode::Space);
		}

		void OnImGuiRender() override
		{
			ImGui::Text("Keyboard Input:");
			ImGui::Text("  W: %s", m_WKeyPressed ? "Pressed" : "Released");
			ImGui::Text("  A: %s", m_AKeyPressed ? "Pressed" : "Released");
			ImGui::Text("  S: %s", m_SKeyPressed ? "Pressed" : "Released");
			ImGui::Text("  D: %s", m_DKeyPressed ? "Pressed" : "Released");
			ImGui::Text("  Space: %s", m_SpacePressed ? "Pressed" : "Released");
			ImGui::Separator();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f),
				"Press keys to test input system");
		}

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();
			result.Message = "Input system polling functional (manual verification required)";
			return result;
		}

	private:
		bool m_WKeyPressed = false;
		bool m_AKeyPressed = false;
		bool m_SKeyPressed = false;
		bool m_DKeyPressed = false;
		bool m_SpacePressed = false;
	};

	class InputPollingTest : public Test
	{
	public:
		const char* GetName() const override { return "Input Polling Performance"; }
		const char* GetCategory() const override { return "Input System"; }

		void OnUpdate(Timestep ts) override
		{
			auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < 100; ++i)
			{
				Input::IsKeyPressed(KeyCode::W);
				Input::IsKeyPressed(KeyCode::A);
				Input::IsKeyPressed(KeyCode::S);
				Input::IsKeyPressed(KeyCode::D);
			}

			auto end = std::chrono::high_resolution_clock::now();
			m_LastPollTime = std::chrono::duration<float, std::micro>(end - start).count();
		}

		void OnImGuiRender() override
		{
			ImGui::Text("400 input polls per frame");
			ImGui::Text("Last poll time: %.2f µs", m_LastPollTime);
			ImGui::Text("Average: %.4f µs per poll", m_LastPollTime / 400.0f);
		}

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			constexpr int iterations = 10000;
			auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < iterations; ++i)
				Input::IsKeyPressed(KeyCode::W);

			auto end = std::chrono::high_resolution_clock::now();
			float totalTime = std::chrono::duration<float, std::milli>(end - start).count();
			float avgTime = (totalTime * 1000.0f) / static_cast<float>(iterations);

			result.Message = std::format("Average poll time: {:.4f} µs ({} iterations)",
				avgTime, iterations);

			return result;
		}

	private:
		float m_LastPollTime = 0.0f;
	};

}