#pragma once

#include "../TestLayer.hpp"

#include <format>
#include <set>

namespace Luma {

	class InputStateTest : public Test
	{
	public:
		const char* GetName() const override { return "Input State Test"; }
		const char* GetCategory() const override { return "Input System"; }

		void OnUpdate(Timestep ts) override
		{
			// Test Pressed (single frame)
			m_WKeyPressed = Input::IsKeyPressed(KeyCode::W);
			m_AKeyPressed = Input::IsKeyPressed(KeyCode::A);
			m_SKeyPressed = Input::IsKeyPressed(KeyCode::S);
			m_DKeyPressed = Input::IsKeyPressed(KeyCode::D);
			m_SpacePressed = Input::IsKeyPressed(KeyCode::Space);

			// Test Held (continuous)
			m_WKeyHeld = Input::IsKeyHeld(KeyCode::W);
			m_SpaceHeld = Input::IsKeyHeld(KeyCode::Space);

			// Test Released (single frame)
			m_WKeyReleased = Input::IsKeyReleased(KeyCode::W);

			// Test Down (immediate SDL state)
			m_ShiftDown = Input::IsKeyDown(KeyCode::LeftShift);
			m_CtrlDown = Input::IsKeyDown(KeyCode::LeftControl);

			// Test toggle keys using KeyMods
			m_CapsLockOn = Input::IsKeyToggledOn(KeyCode::CapsLock);
			m_NumLockOn = Input::IsKeyToggledOn(KeyCode::NumLock);
			m_ScrollLockOn = Input::IsKeyToggledOn(KeyCode::ScrollLock);

			// Mouse button states
			m_LeftMousePressed = Input::IsMouseButtonPressed(MouseButton::Left);
			m_RightMousePressed = Input::IsMouseButtonPressed(MouseButton::Right);
			m_LeftMouseHeld = Input::IsMouseButtonHeld(MouseButton::Left);

			// Mouse position
			auto [x, y] = Input::GetMousePosition();
			m_MouseX = x;
			m_MouseY = y;
		}

		void OnImGuiRender() override
		{
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Keyboard States:");

			ImGui::Text("  W: %s | Held: %s | Released: %s",
				m_WKeyPressed ? "PRESSED" : "       ",
				m_WKeyHeld ? "YES" : "NO ",
				m_WKeyReleased ? "YES" : "NO ");

			ImGui::Text("  A: %s", m_AKeyPressed ? "PRESSED" : "       ");
			ImGui::Text("  S: %s", m_SKeyPressed ? "PRESSED" : "       ");
			ImGui::Text("  D: %s", m_DKeyPressed ? "PRESSED" : "       ");
			ImGui::Text("  Space: %s | Held: %s",
				m_SpacePressed ? "PRESSED" : "       ",
				m_SpaceHeld ? "YES" : "NO ");

			ImGui::Separator();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Modifier Keys (IsKeyDown):");
			ImGui::Text("  Shift: %s", m_ShiftDown ? "DOWN" : "UP  ");
			ImGui::Text("  Ctrl:  %s", m_CtrlDown ? "DOWN" : "UP  ");

			ImGui::Separator();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Toggle Keys (IsKeyToggledOn):");
			ImGui::Text("  CapsLock:   %s", m_CapsLockOn ? "ON" : "OFF");
			ImGui::Text("  NumLock:    %s", m_NumLockOn ? "ON" : "OFF");
			ImGui::Text("  ScrollLock: %s", m_ScrollLockOn ? "ON" : "OFF");

			ImGui::Separator();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Mouse:");
			ImGui::Text("  Left:  %s | Held: %s",
				m_LeftMousePressed ? "PRESSED" : "       ",
				m_LeftMouseHeld ? "YES" : "NO ");
			ImGui::Text("  Right: %s", m_RightMousePressed ? "PRESSED" : "       ");
			ImGui::Text("  Position: (%.1f, %.1f)", m_MouseX, m_MouseY);

			ImGui::Separator();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f),
				"Pressed = single frame, Held = continuous, Down = immediate SDL query");
		}

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();
			result.Message = "Input system state transitions functional (manual verification required)";
			return result;
		}

	private:
		bool m_WKeyPressed = false, m_WKeyHeld = false, m_WKeyReleased = false;
		bool m_AKeyPressed = false;
		bool m_SKeyPressed = false;
		bool m_DKeyPressed = false;
		bool m_SpacePressed = false, m_SpaceHeld = false;
		bool m_ShiftDown = false, m_CtrlDown = false;
		bool m_CapsLockOn = false, m_NumLockOn = false, m_ScrollLockOn = false;
		bool m_LeftMousePressed = false, m_LeftMouseHeld = false;
		bool m_RightMousePressed = false;
		float m_MouseX = 0.0f, m_MouseY = 0.0f;
	};

	class InputPollingTest : public Test
	{
	public:
		const char* GetName() const override { return "Input Polling Performance"; }
		const char* GetCategory() const override { return "Input System"; }

		void OnUpdate(Timestep ts) override
		{
			auto start = std::chrono::high_resolution_clock::now();

			// Test keyboard polling
			for (int i = 0; i < 100; ++i)
			{
				Input::IsKeyPressed(KeyCode::W);
				Input::IsKeyHeld(KeyCode::A);
				Input::IsKeyDown(KeyCode::S);
				Input::IsKeyReleased(KeyCode::D);
			}

			// Test mouse polling
			for (int i = 0; i < 50; ++i)
			{
				Input::IsMouseButtonPressed(MouseButton::Left);
				Input::GetMousePosition();
			}

			auto end = std::chrono::high_resolution_clock::now();
			m_LastPollTime = std::chrono::duration<float, std::micro>(end - start).count();
		}

		void OnImGuiRender() override
		{
			ImGui::Text("Per frame: 400 keyboard + 100 mouse polls");
			ImGui::Text("Last poll time: %.2f µs", m_LastPollTime);
			ImGui::Text("Average: %.4f µs per poll", m_LastPollTime / 500.0f);

			if (m_LastPollTime > 100.0f)
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f),
					"WARNING: High polling latency detected");
		}

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			constexpr int iterations = 10000;
			auto start = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < iterations; ++i)
			{
				Input::IsKeyPressed(KeyCode::W);
				Input::IsKeyHeld(KeyCode::W);
			}

			auto end = std::chrono::high_resolution_clock::now();
			float totalTime = std::chrono::duration<float, std::milli>(end - start).count();
			float avgTime = (totalTime * 1000.0f) / static_cast<float>(iterations);

			result.Message = std::format("Average poll time: {:.4f} µs ({} iterations, {} ops)",
				avgTime, iterations, iterations * 2);

			return result;
		}

	private:
		float m_LastPollTime = 0.0f;
	};

}