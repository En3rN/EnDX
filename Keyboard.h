#pragma once
#include "Event.h"
#include "iHandle.h"
#include <bitset>

namespace En3rN::DX
{
	class Keyboard: shPtr(Keyboard) , public Event::Listener
	{
	public:
		bool OnEvent(Event& e) override;
		void OnKeyPress(uint8_t keyCode);
		void OnKeyRelease(uint8_t keyCode);
		bool IsPressed(uint8_t keyCode);
		int  PressCount();
		void Clear();
	private:
		std::bitset<256u> keystate{ 0 };
	};
}


