#pragma once
#include "iBindable.h"
namespace En3rN::DX
{
    class Stencil : public Bindable
    {
    public:
        enum class State{ DepthStencilEnabled, DepthOnly, DepthOnlyFuncLessEqualNoWrite };
        Stencil(State state);
        void Bind();
    private:
        ComPtr<ID3D11DepthStencilState> pStencilState;
    };
}

