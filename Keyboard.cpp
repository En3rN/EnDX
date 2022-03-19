#include "enWin.h"
#include "Keyboard.h"
#include "logger.h"
#include "enString.h"
#include "Event.h"

namespace En3rN::DX
{
    bool Keyboard::OnEvent(Event& e)
    {
        if(e.category == Event::Category::Keyboard)
        {
            switch(e.type)
            {
            case Event::Type::KeyUp:
                OnKeyRelease((uint8_t)e.lparam);
            case Event::Type::KeyDown:
                OnKeyPress((uint8_t)e.lparam);
            default:
                break;
            }
        }
        return false;
    }
    void En3rN::DX::Keyboard::OnKeyPress(uint8_t keyCode)
    {        
        keystate.set(keyCode);
    }

    void En3rN::DX::Keyboard::OnKeyRelease(uint8_t keyCode)
    {        
        keystate.reset(keyCode);
    }

    bool En3rN::DX::Keyboard::IsPressed(uint8_t keyCode)
    {
        return keystate[keyCode];
    }
    int Keyboard::PressCount()
    {
        return keystate.count();
    }
    void Keyboard::Clear()
    {
        keystate.reset();
    }
}
