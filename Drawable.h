#pragma once
#include "iHandle.h"
#include "iBindable.h"
#include "Entity.h"
#include "Model.h"
#include <functional>


namespace En3rN::DX
{
	class Drawable :  public GfxResources, unPtr(Drawable)
	{
		friend class Behaviors;
	public:
		Drawable() = default;
		Drawable(Drawable&& other) noexcept = default;
		virtual ~Drawable() = default;

		void AddBindable(Bindable::Base::handle bindable);
		Drawable& operator << (Bindable::Base::handle bindable)
		{
			AddBindable(std::move(bindable));
			return *this;
		}

		std::vector<Bindable::Base::handle>& GetBindables() { return bindables; }
		//std::vector<int>& GetDynamicBindableIndexes() { return DynamicBindableIndexes; }

		//virtual void UpdateConstantBuffer();
		virtual void Update(float dt) { return; }
		virtual void Draw() = 0;

		//operator Entity& () { return static_cast<Entity&>(*this); }
	protected:
		std::vector<Bindable::Base::handle> bindables;
		UINT indexCount{};
	};
}
