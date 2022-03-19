#pragma once
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include <random>
#include <vector>
namespace En3rN::DX
{
    class Primitive
    {
		
	public:
		Model::handle operator()() // RandomGenerated Primitive
		{
			Vec3f pos(xdist(rng), ydist(rng), zdist(rng));
			Vec4f color(redDist(rng), greenDist(rng), blueDist(rng));
			switch (typedist(rng))
			{
			case 0:
				return LoadCube(
					std::move(pos),
					Material(Material::Data({color,Vec3f(),0,Vec3f(),0})
				));
			case 1:
				return LoadPlane(
					std::move(pos),
					Material(Material::Data({ color,Vec3f(),0,Vec3f(),0 })
				));
			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}
		Model::handle LoadCube(Vec3f&& pos, Material&& mat);
		Model::handle LoadPlane(Vec3f&& pos, Material&& mat);
		Model::handle LoadSphere(Vec3f&& pos, Material&& mat);
	private:
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> xdist{ -10.0f,10.0f };
		std::uniform_real_distribution<float> ydist{ -8.f,8.f };
		std::uniform_real_distribution<float> zdist{ 1.0f,20.0f };

		std::uniform_real_distribution<float> redDist{ .0f,1.f };
		std::uniform_real_distribution<float> greenDist{ .0f,1.f };
		std::uniform_real_distribution<float> blueDist{ .0f,1.f };

		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,0 };
    };

    
};


