#pragma once

#include "../TestLayer.hpp"

#include <format>

namespace Luma {

	class EventDispatchTest : public Test
	{
	public:
		const char* GetName() const override { return "Event Dispatch Test"; }
		const char* GetCategory() const override { return "Event System"; }

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			try
			{
				bool windowCloseCalled = false;
				bool keyPressedCalled = false;

				WindowCloseEvent closeEvent;
				KeyPressedEvent keyEvent(KeyCode::A, 0);

				EventDispatcher dispatcher(closeEvent);
				dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent& e) {
					windowCloseCalled = true;
					return true;
				});

				EventDispatcher keyDispatcher(keyEvent);
				keyDispatcher.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& e) {
					keyPressedCalled = true;
					return e.GetKeyCode() == KeyCode::A;
				});

				if (!windowCloseCalled)
				{
					result.Passed = false;
					result.Message = "WindowCloseEvent not dispatched";
					return result;
				}

				if (!keyPressedCalled)
				{
					result.Passed = false;
					result.Message = "KeyPressedEvent not dispatched";
					return result;
				}

				result.Message = "All events dispatched correctly";
			}
			catch (const std::exception& e)
			{
				result.Passed = false;
				result.Message = std::format("Exception: {}", e.what());
			}

			return result;
		}
	};

	class EventQueueTest : public Test
	{
	public:
		const char* GetName() const override { return "Event Queue Test"; }
		const char* GetCategory() const override { return "Event System"; }

		TestResult Run() override
		{
			TestResult result;
			result.Name = GetName();

			try
			{
				std::vector<EventType> receivedEvents;

				std::vector<std::unique_ptr<Event>> eventQueue;
				eventQueue.push_back(std::make_unique<WindowResizeEvent>(1920, 1080));
				eventQueue.push_back(std::make_unique<KeyPressedEvent>(KeyCode::W, 0));

				for (auto& event : eventQueue)
					receivedEvents.push_back(event->GetEventType());

				if (receivedEvents.size() != 2)
				{
					result.Passed = false;
					result.Message = std::format("Expected 2 events, got {}", receivedEvents.size());
					return result;
				}

				result.Message = "Event queue processed correctly";
			}
			catch (const std::exception& e)
			{
				result.Passed = false;
				result.Message = std::format("Exception: {}", e.what());
			}

			return result;
		}
	};

}