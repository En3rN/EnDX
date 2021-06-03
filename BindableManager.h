#pragma once
#include "iBindable.h"
#include <unordered_map>
#include <map>
namespace En3rN::DX
{
	template <typename Bkey>
	class BindableManager : std::unordered_map<Bkey, Bindable::handle>
	{
	};
}

