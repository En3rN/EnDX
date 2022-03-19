#pragma once
#include "Drawable.h"
#include "Primitive.h"
#include "Cube.h"
#include "Plane.h"
#include "enMath.h"
#include "vec.h"
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
				return std::make_unique<Cube>(Vec3f(xdist(rng), ydist(rng) , zdist(rng)));
			case 1:
				return std::make_unique<Plane>(Vec3f(xdist(rng), ydist(rng), zdist(rng)));
			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}
		//Pass -1 for random or pass preset
		Vec3f Angles(float x =-1, float y= -1, float z= -1)
		{
			std::uniform_real_distribution<float> angleDist{ -halfPI, halfPI };
			x == -1 ? x = angleDist(rng) : x = x;
			y == -1 ? y = angleDist(rng) : y = y;
			z == -1 ? z = angleDist(rng) : z = z;
			return { x,y,z };
		}
		size_t UID()
		{
			std::uniform_int_distribution<size_t> uid;
			return uid(rng);
		}
		
	private:
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> xdist{-10.0f,10.0f };
		std::uniform_real_distribution<float> ydist{-8.f,8.f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> zdist{ 1.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,1 };

	};


}
