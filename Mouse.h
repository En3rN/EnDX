#pragma once
#include "iHandle.h"
#include "vec.h"
#include "Event.h"
#include <deque>
#include <optional>
#include <bitset>
namespace En3rN::DX
{
	class Mouse: shPtr(Mouse), public EventListener
	{
	public:
		Mouse();
		const Vec2<uint16_t>& MousePos();
		bool IsPressed(uint8_t button);
		bool OnEvent(Event& e) override;
		void OnMove(uint16_t x, uint16_t y);
		void OnPress(uint8_t button);
		void OnRelease(uint8_t button);
		void OnRawDelta(int dx, int dy);
		Vec2i GetRawDelta();
		void SetRawCaptureMode();
		bool RawCapture();
		void TrimBuffer();

	private:
		uint8_t maxBufferSize = 5;
		Vec2<uint16_t> pos;
		std::bitset<10> buttonState;
		std::deque<Vec2i> rawDeltaBuffer;
		bool rawCapture = false;
	};
}

