#pragma once
#include "iHandle.h"
#include "vec.h"
#include "Event.h"
#include <functional>
#include <queue>
#include <optional>
#include <bitset>
namespace En3rN::DX
{
	class Mouse: shPtr(Mouse), public Event::Listener
	{
	public:
		struct RawListener
		{
			virtual void OnRawMouse(const long dx, const long dy) = 0;
		};
		Mouse();
		const Vec2<int16_t>& MousePos();
		bool IsPressed(uint8_t button);
		bool OnEvent(Event& e) override;
		void OnMove(int16_t& x, int16_t& y);
		void OnPress(uint8_t button);
		void OnRelease(uint8_t button);
		void OnRawDelta(const long& dx, const long& dy);
		void Flush();
		std::optional<Vec2i> GetRawDelta();
		void ToggleRawCaptureMode();
		bool RawCapture();
		void TrimBuffer();

	private:
		uint8_t maxBufferSize = 5;
		Vec2<int16_t> pos{};
		std::bitset<10> buttonState{};
		std::queue<Vec2i> rawDeltaBuffer{};
		bool rawCapture = false;
	};
}

