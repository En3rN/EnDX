#include "enWin.h"
#include "Keyboard.h"
#include "logger.h"
#include "enString.h"
#include "Event.h"

namespace En3rN::DX
{

    void En3rN::DX::Keyboard::OnKeyPress(uint8_t keyCode)
    {        
        keystate.set(keyCode, true);
    }

    void En3rN::DX::Keyboard::OnKeyRelease(uint8_t keyCode)
    {        
        keystate.set(keyCode, false);
    }

    bool En3rN::DX::Keyboard::IsPressed(uint8_t keyCode)
    {
        return keystate[keyCode];
    }
}
