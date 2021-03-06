#include "Mouse.h"
#include "logger.h"
#include "Entity.h"


#include <sstream>
namespace En3rN::DX
{
	Mouse::Mouse() : pos{}
	{
		//Register();
	}
	const Vec2<int16_t>& Mouse::MousePos()
	{		
		return pos;
	}

	bool Mouse::IsPressed(uint8_t button)
	{
		return false;
	}

	bool Mouse::OnEvent(Event& e)
	{
		if (e.type==Event::Flag::RawCapture)
		{			
			ToggleRawCaptureMode();
		}
		return false;
	}

	void Mouse::OnMove(int16_t& x, int16_t& y)
	{		
		//Logger::Debug("MousePosition [%d , %d]",x,y);
		pos.x = x; pos.y = y;
	}

	void Mouse::OnPress(uint8_t button)
	{
		Logger::Debug("MousePress [%d]", button);
		//buttonState[button] = true;
	}

	void Mouse::OnRelease(uint8_t button)
	{
		Logger::Debug("MouseRelease [%d]",button);
		buttonState[button] = false;
	}
	void Mouse::OnRawDelta(const long& dx, const long& dy)
	{	
		rawDeltaBuffer.emplace(Vec2i(dx,dy));
		TrimBuffer();
	}
	void Mouse::Flush()
	{
		while(!rawDeltaBuffer.empty())
			rawDeltaBuffer.pop();
		buttonState.reset();
	}
	std::optional<Vec2i> Mouse::GetRawDelta()
	{
		if (rawDeltaBuffer.empty())
		{
			return std::nullopt;
		}
		const Vec2i d = rawDeltaBuffer.front();
		rawDeltaBuffer.pop();
		return d;
		
	}
	void Mouse::ToggleRawCaptureMode()
	{
		rawCapture = rawCapture ? false : true;
	}
	bool Mouse::RawCapture()
	{
		return rawCapture;
	}
	void Mouse::TrimBuffer()
	{
		while (rawDeltaBuffer.size() > maxBufferSize)
		{
			rawDeltaBuffer.pop();
		}
	}
}
