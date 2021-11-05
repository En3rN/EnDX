#include "Drawable.h"

void En3rN::DX::Drawable::AddBindable(Bindable::Base::handle bindable)
{
	/*DynamicBindable* dynamic = dynamic_cast<DynamicBindable*>(bindable.get());
	if (dynamic) DynamicBindableIndexes.push_back((int)bindables.size());*/
	bindables.emplace_back(std::move(bindable));
}