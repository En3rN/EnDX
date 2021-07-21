#pragma once
#include "iBindable.h"
namespace En3rN::DX
{
    enum class State{None=1,Front,Back};
    class Rasterizer :
        public Bindable
    {
    public:
        Rasterizer(State state);
        std::string GetKey(State state);
        void Bind() override;
    private:
        State state;
        ComPtr<ID3D11RasterizerState> pRasterizerState;
    };
}

