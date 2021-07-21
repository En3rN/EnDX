#pragma once
#include "Event.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <stdint.h>
#include <vector>
namespace En3rN::DX
{
	enum class KeyMapping{Forward, Backward, Left, Right, Up, Down, TurnLeft, TurnRight, LookUp, LookDown, SetRawMode};

	class Controllable : public EventListener
	{
	protected:
		Controllable();
		Controllable(uint8_t forward,	uint8_t backward,
					 uint8_t left,		uint8_t right,
					 uint8_t up,		uint8_t down,		
					 uint8_t turnleft,	uint8_t turnright,		
					 uint8_t lookup,	uint8_t lookdown,		
					 uint8_t rawMode);
		virtual void OnRawCapture(float dt, Keyboard& kbd, Mouse& mouse) = 0;
		std::vector<uint8_t> keyMappings;
		float sensitivity = 0.5;
		bool rawCapture;
	};
}
