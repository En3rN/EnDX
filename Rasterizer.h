#pragma once
#include "iBindable.h"
namespace En3rN::DX
{
    
    class Rasterizer :
        public Bindable
    {
    public:
        using handle = std::shared_ptr<Rasterizer>;
        enum class State { None = 1, Front, Back };
        Rasterizer(State state);
        static std::string GetKey(State state)
        {
            return typeid(Rasterizer).name() + std::to_string((int)state);
        }
        void Bind() override;
    private:
        State state;
        ComPtr<ID3D11RasterizerState> pRasterizerState;
    };
    
}

