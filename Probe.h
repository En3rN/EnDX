#pragma once
#include "enBuffer.h"
#include <entt\entt.hpp>
namespace En3rN::DX {

	class Entity;

	struct Probe
	{
	public:
		virtual bool Visit( Entity& entity ) = 0;
	private:
	};

	struct EntityTreeProbe : public Probe
	{
		bool Visit( Entity& entity ) override;
		entt::entity selected = entt::null;
		Entity* pEntity;
	};

}

