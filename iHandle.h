#pragma once
#include <memory>
#define shPtr(x) public iHandle <std::shared_ptr<x>>
#define unPtr(x) public iHandle <std::unique_ptr<x>>

namespace En3rN
{
	template <typename smartPtrType>
	class iHandle
	{
	public:
		using handle = smartPtrType;
	};
}

