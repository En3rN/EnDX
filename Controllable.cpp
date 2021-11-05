#include "Controllable.h"

namespace En3rN::DX
{
	Controllable::Controllable() : rawCapture(false)
	{
		keyMappings = { 'W','S','A','D',VK_SPACE,VK_CONTROL,'Q','E','R','F', VK_TAB };
		Register();
	}
	Controllable::Controllable( uint8_t forward, uint8_t backward,
								uint8_t left, uint8_t right,
								uint8_t up, uint8_t down,
								uint8_t turnleft, uint8_t turnright,
								uint8_t lookup, uint8_t lookdown,
								uint8_t rawMode)
	{
		keyMappings = { forward,backward,left,right,up,down,turnleft,turnright,lookup,lookdown,rawMode };
	}
	
}