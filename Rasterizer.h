#pragma once
#include "iBindable.h"
namespace En3rN::DX
{
    enum class CullMode{None=1,Front,Back};
    class Rasterizer :
        public Bindable
    {
    public:
        Rasterizer(CullMode cull);
        void Bind() override;
    private:
        ComPtr<ID3D11RasterizerState> pRasterizerState;
    };
}

