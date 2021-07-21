#pragma once
#include "iHandle.h"
#include "iBindable.h"
#include "Entity.h"
#include "Model.h"
#include <functional>


namespace En3rN::DX
{
	class Drawable :public Entity, public GfxResources, unPtr(Drawable)
	{
	public:
		Drawable() = default;
		Drawable(Drawable&& other) noexcept = default;
		virtual ~Drawable() = default;
		std::vector<Bindable::handle>& GetBindables() { return bindables; }
		virtual void Draw() = 0;
		void AddBindable(Bindable::handle bindable);
		Drawable& operator << (Bindable::handle bindable)
		{
			AddBindable(std::move(bindable));
			return *this;
		}
		operator Entity& () { return dynamic_cast<Entity&>(*this); }
	protected:
		std::vector<Bindable::handle> bindables;
		std::vector<int> DynamicBindableIndexes;
		UINT indexCount;
	};

	class DynamicDrawable : public Drawable
	{
		friend class Behaviors;
	public:
		DynamicDrawable() = default;
		DynamicDrawable(DynamicDrawable&& other) noexcept = default;
		virtual ~DynamicDrawable() = default;
		virtual void Update(float dt) = 0;
	protected:
	};
}
