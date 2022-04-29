#pragma once
#include "iHandle.h"
#include "GfxResources.h"
#include "logger.h"
#include "enString.h"
#include <mutex>
#include <map>
#include <vector>
#include <string>


namespace En3rN::DX
{	
	class Bindable : public GfxResources
	{
	public:
		struct Base
		{ 
			using handle = std::shared_ptr<Bindable>;
			using Container = std::vector<handle>;
		};
		
		Bindable() = default;
		Bindable(const Bindable& other) = default;
		Bindable(Bindable && other) noexcept = default;
		Bindable& operator = (const Bindable& other) = default;
		Bindable& operator = (Bindable&& other) noexcept = default;

		//virtual Bindable::Base::handle Clone() = 0;
		virtual void Bind() = 0;
		virtual ~Bindable() = default;
	protected:
	};
}
