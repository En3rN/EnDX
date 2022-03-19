#pragma once
#include "iBindable.h"
namespace En3rN::DX
{
    class Stencil : public Bindable
    {
    public:
        using handle = std::shared_ptr<Stencil>;
        enum class State{ DepthOnly, StencilEnabled, DepthOnlyFuncLessEqualNoWrite };
        Stencil(State state);
        static std::string GetKey(State state)
        {
            return typeid(Stencil).name() + std::to_string((int)state);
        }
        /*bool IsBound() override
        {
            ID3D11DepthStencilState* current;
            pContext->OMGetDepthStencilState(&current,NULL);
            if (pStencilState.Get() == current)
                return true;
            return false;
        }*/
        void Bind();
    private:
        State state;
        ComPtr<ID3D11DepthStencilState> pStencilState;
    };
}

