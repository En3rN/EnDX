#include "Teqnique.h"
#include "Sampler.h"
#include "Rasterizer.h"
#include "Blend.h"
#include "Stencil.h"
#include "TransformConstantBufferScaling.h"
#include "Material.h"

namespace En3rN::DX 
{
	std::map<std::string, std::function<Teqnique()>> Teqnique::Teqniques = {

		{
			{"Phong"},
			{[](){return Teqnique( "Phong", true, Step( { "Phong" } ) ); }}
		},

		{
			{ "Unlit" },
			{[](){return Teqnique( "Unlit", true, Step( { "Unlit" } ) ); }}
		},
		{
			{ "Outline" },
			{[]() {
				Teqnique outline( "Outline", true ,
				Step( { "MaskBackBuffer" },
					BindableManager::Query<NullPixelShader>(),
					BindableManager::Query<DepthStencilState>( DepthStencilState::Depth::Disable, DepthStencilState::Stencil::Write, 1 )
				) ,
				Step( { "DrawOutline", "Outline" },
					BindableManager::Query<PixelShader>( "Unlit", "main" ),
					std::make_shared<Material::ConstantBuffer>( Material::Data( { 1,0,0,1 } ), 1, 1, "outlineMat" ), //default
					std::make_shared<TransformConstantBufferScaling>( 1.05f ) //default
				) ,
				Step( { "FullScreen","DrawOutline" },
						BindableManager::Query<NullPixelShader>(),	//HACK --> To avoid mesh.Addbindable to insert shader based on pass and material
						BindableManager::Query<DepthStencilState>( DepthStencilState::Depth::Disable, DepthStencilState::Stencil::Read, 1 ),
						BindableManager::Query<Blender>( Blender::State::Enabled )
					) );
					Vec3f color{ 1,0,0 };
					float scale{1.05};
					//outline.GetContext().add_element( color, "Color" );
					//outline.GetContext().add_element( scale, "Scale" );					
					return outline;
				}
			}
		}
	};
};
	

