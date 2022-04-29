#include "Skybox.h"
#include "Shader.h"
#include "Sampler.h"
#include "Texture.h"
#include "Rasterizer.h"
#include "Camera.h"
#include "Blend.h"
#include "Stencil.h"
#include "Buffer.h"
#include "InputLayout.h"
#include "enBuffer.h"
#include "Transform.h"
namespace En3rN::DX
{
	Skybox::Skybox(std::string file)
	{ 
        auto& cam = Camera::GetActiveCamera();
        float x = 1.0f / 4.0f;
        float y = 1.0f / 3.0f;
        float p = 1.0f;
        float c = 0.01f;
        VertexElement::Pos vertecies[]
        {
            {-p,+p,+p,}, //up    // 0.0f,1.0f
            {+p,+p,+p,},         // 1.0f,1.0f
            {+p,+p,-p,},         // 1.0f,0.0f
            {-p,+p,-p,}, 		 // 0.0f,0.0f
            {-p,+p,+p,}, //left  // 0.0f,1.0f         
            {-p,+p,-p,},		 // 1.0f,1.0f
            {-p,-p,-p,},		 // 1.0f,0.0f
            {-p,-p,+p,},		 // 0.0f,0.0f
            {-p,+p,-p,}, //front // 0.0f,1.0f
            {+p,+p,-p,},		 // 1.0f,1.0f
            {+p,-p,-p,},		 // 1.0f,0.0f
            {-p,-p,-p,},		 // 0.0f,0.0f
            {+p,+p,-p,}, //right // 0.0f,1.0f
            {+p,+p,+p,},		 // 1.0f,1.0f
            {+p,-p,+p,},		 // 1.0f,0.0f
            {+p,-p,-p,},		 // 0.0f,0.0f
            {-p,-p,+p,}, //back  // 0.0f,1.0f
            {+p,-p,+p,},		 // 1.0f,1.0f
            {+p,+p,+p,},		 // 1.0f,0.0f
            {-p,+p,+p,},		 // 0.0f,0.0f
            {-p,-p,-p,}, //down  // 0.0f,1.0f
            {+p,-p,-p,},		 // 1.0f,1.0f
            {+p,-p,+p,},		 // 1.0f,0.0f
            {-p,-p,+p,},		 // 0.0f,0.0f
        };
        enBuffer buf;
        buf.add_element(vertecies, std::size(vertecies));
        buf.create_buffer();
        std::vector<uint16_t> indecies{ 0, 1, 2, 2, 3, 0,   //
                                        4, 5, 6, 6, 7, 4,   //
                                        8, 9,10,10,11, 8,   //
                                       12,13,14,14,15,12,   //
                                       16,17,18,18,19,16,   //
                                       20,21,22,22,23,20};  //
        indexCount = (UINT)std::size(indecies);
        auto vs = BindableManager::Query <VertexShader>("VSSkybox.cso");        
        auto ps = BindableManager::Query <PixelShader>("PSSkybox.cso");
        AddBindable(BindableManager::Query<VertexBuffer>(buf,"cube"));
        AddBindable(BindableManager::Query<IndexBuffer>(indecies, "cube"));
        AddBindable(BindableManager::Query<InputLayout>(vs->GetSignatures(), vs.get()->GetBlob(),"cube"));
        AddBindable(BindableManager::Query<Texture>(file, 0, Texture::Flag::CubeMap));
        AddBindable(BindableManager::Query<Sampler>(Sampler::State::Wrap));
        AddBindable(BindableManager::Query<Rasterizer>(Rasterizer::State::Front));
        AddBindable(BindableManager::Query<DepthStencilState>(DepthStencilState::Depth::FuncLessEqualNoWrite));
        AddBindable(BindableManager::Query<Transform::ConstantBuffer>(2,1));
        AddBindable(std::move(vs));
        AddBindable(std::move(ps));
	}
	void Skybox::Draw()
    {
        GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        GetContext()->DrawIndexed(indexCount, 0, 0);
	}
    void Skybox::Update(float dt)
    {
        auto& cam = Camera::GetActiveCamera();
    }
}