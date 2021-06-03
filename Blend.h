#pragma once
#include "iBindable.h"

namespace En3rN::DX
{
    
    
	class Blend : public Bindable
    {
    public:
        enum class State { Default, Enabled, Custom };
        Blend(State state);
        void Bind() override;
    private:
        ComPtr<ID3D11BlendState> pBlenderState;
    };
}
