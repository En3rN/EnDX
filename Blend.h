#pragma once
#include "iBindable.h"
#include <sstream>
#include <string>
#include <typeinfo>

namespace En3rN::DX
{
    
    
	class Blender : public Bindable
    {
    public:
        enum class State { Default, Enabled, Custom };
        Blender(State state);
        static std::string GetKey(State state)
        {
            return typeid(Blender).name() + std::to_string((int)state);
        }
        void Bind() override;
    private:
        State state;
        ComPtr<ID3D11BlendState> pBlenderState;
    };

   /* std::ostream& operator << (std::ostream& os, Blend::State state)
    {        
        os << (int)state;
    }*/
}
