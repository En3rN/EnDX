#pragma once
#include "iBindable.h"
#include <string>
#include <typeinfo>

namespace En3rN::DX
{
    
    
	class Blend : public Bindable
    {
    public:
        enum class State { Default, Enabled, Custom };
        Blend(State state);
        std::string GetKey();
        void Bind() override;
    private:
        State state;
        ComPtr<ID3D11BlendState> pBlenderState;
    };
}
