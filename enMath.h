#pragma once
#include <math.h>

namespace En3rN
{
	constexpr auto PI_D = 3.14159265358979323846;
	constexpr float twoPi = (float)PI_D * 2;
	constexpr float PI = (float)PI_D;
	constexpr float halfPI = (float)PI_D / 2;
	
	template<typename T>
	T WrapAngle(T theta) noexcept
	{
		constexpr T twoPi = (T)PI_D * 2;
		const T mod = (T)fmod(theta, twoPi);
		if (mod > (T)PI_D)
		{
			return mod - twoPi;
		}
		else if (mod < -(T)PI_D)
		{
			return mod + twoPi;
		}
		return mod;
	}
	template <typename T>
	T Wrap(T value,T min, T max)
	{
		if (value > max)
		{
			return min + value-max;
		}
		else if (value < min)
		{
			return max + value;
		}
		return value;
	}
}
