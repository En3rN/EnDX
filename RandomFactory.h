#pragma once
#include "Drawable.h"
#include "Primitive.h"
#include "Cube.h"
#include "Plane.h"
#include "enMath.h"
#include <random>
#include <memory>

namespace En3rN::DX
{	
	class RandomFactory
	{
	public:		
		std::unique_ptr<Drawable> operator()()
		{
			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<Cube>(Vec3f(adist(rng), adist(rng) , rdist(rng)));
			case 1:
				return std::make_unique<Plane>(Vec3f(adist(rng), adist(rng), rdist(rng)));
			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}
	private:
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{-10.0f,10.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 1.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,1 };
	};


}
