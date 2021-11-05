#pragma once

#include <memory>
#include <vector>
#include <map>
#include <string>

namespace En3rN::DX
{
	class Component
	{
	public:
		struct Base
		{
			using handle = std::shared_ptr<Component>;
			using Container = std::map<std::string, handle>;
		};
		virtual void OnAttach() = 0;
		virtual void Bind() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDetach() = 0;
		virtual ~Component() = default;
	};
}