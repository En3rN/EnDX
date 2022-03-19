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
	class Buffer;
	class Graphics;
	class Keyboard;
	class Mouse;
	class DynamicBindable;
	class Camera;
	class BindableManager;	
	class Job;
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
		virtual bool IsBound() { return false; }
		virtual void Bind() = 0;
		virtual void Update(const Job& job) { return;}
		virtual ~Bindable() = default;
	protected:
	};
}
