#include "Mouse.h"
#include "logger.h"

#include <sstream>
namespace En3rN::DX
{
	Mouse::Mouse() : pos{}
	{
		//Register();
	}
	const Vec2<uint16_t>& Mouse::MousePos()
	{		
		return pos;
	}

	bool Mouse::IsPressed(uint8_t button)
	{
		return false;
	}

	bool Mouse::OnEvent(Event& e)
	{
		if (e.category == Event::Category::Mouse && e.type==Event::Type::RawCapture)
		{
			Logger::Debug("MouseRespondingToEvent");
			SetRawCaptureMode();
			return true;
			
		}
		return false;
	}

	void Mouse::OnMove(uint16_t x, uint16_t y)
	{
		/*std::stringstream s;
		s << "MouseMove:[" << x <<',' << y << ']' << std::endl;*/
		Logger::Debug("MousePosition [%d , %d]\n",x,y);
		pos.x = x; pos.y = y;
	}

	void Mouse::OnPress(uint8_t button)
	{
		Logger::Debug("MousePress [%d]\n", button);
		buttonState[button] = true;
	}

	void Mouse::OnRelease(uint8_t button)
	{
		Logger::Debug("MouseRelease [%d]\n",button);
		buttonState[button] = false;
	}
	void Mouse::OnRawDelta(int dx, int dy)
	{
		rawDeltaBuffer.push_back(Vec2(dx,dy));
		TrimBuffer();
	}
	Vec2i Mouse::GetRawDelta()
	{
		if (rawDeltaBuffer.empty())
		{
			return Vec2i(0,0);
		}
		const Vec2i d = rawDeltaBuffer.front();
		rawDeltaBuffer.pop_front();
		return d;
		
	}
	void Mouse::SetRawCaptureMode()
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
			rawDeltaBuffer.pop_front();
		}
	}
}
