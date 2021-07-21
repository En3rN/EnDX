#pragma once
#include "iBindable.h"
namespace En3rN::DX
{
    class Stencil : public Bindable
    {
    public:
        enum class State{ DepthStencilEnabled, DepthOnly, DepthOnlyFuncLessEqualNoWrite };
        Stencil(State state);
        std::string GetKey(State state)
        {
            return typeid(Stencil).name() + '#' + std::to_string((int)state);
        }
        void Bind();
    private:
        State state;
        ComPtr<ID3D11DepthStencilState> pStencilState;
    };
}

