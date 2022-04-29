#pragma once
#include "iBindable.h"
#include "DepthStencil.h"
namespace En3rN::DX
{
    class DepthStencilState : public Bindable
    {
    public:        
        using handle = std::shared_ptr<DepthStencilState>;
        enum class Depth{ Enable, Disable, FuncLessEqualNoWrite };
        enum class Stencil { Disable, Read , Write};
        DepthStencilState(Depth depth= Depth::Enable, Stencil stencil = Stencil::Disable, ::UINT stencilRef = 0);
        void Bind();
    private:
        Depth m_depth;
        Stencil m_stencil;
        UINT  m_ref;
        ComPtr<ID3D11DepthStencilState> pStencilState;
    };
}

