#pragma once
#include "iHandle.h"
#include "Drawable.h"
#include <stdint.h>
#include <vector>
#include <functional>
namespace En3rN::DX
{
	class Behaviors : unPtr(Behaviors)
	{
	public:
		using Behavior = std::function<void(Drawable&, float)>;
		using Container = std::vector<Behavior>;
		Behaviors();
		static Behavior& Get(uint8_t id);

	private:
		static handle instance;
		Container behaviors;
	};
}

