#include "Behavior.h"
#include "enMath.h"
namespace En3rN::DX
{
	Behaviors::handle Behaviors::instance = std::make_unique<Behaviors>();
	Behaviors::Behaviors()
	{
		behaviors.emplace_back([](Drawable& e,float dt) {
			e.angles.x = WrapAngle(e.angles.x + dt);
			e.angles.y = WrapAngle(e.angles.y + dt / 2);
			e.pos = e.pos.Rotate(0, 0, dt / e.pos.Length());
			});
		behaviors.emplace_back([](Drawable& e, float dt) {
			e.angles.x = WrapAngle(e.angles.x + dt);
			e.angles.z = WrapAngle(e.angles.z + dt / 2);
			e.pos = e.pos.Rotate(0, dt / e.pos.Length(), 0);
			});
		behaviors.emplace_back([](Drawable& e, float dt) {
			e.angles.y = WrapAngle(e.angles.y + dt);
			e.angles.x = WrapAngle(e.angles.x + dt / 2);
			e.pos = e.pos.Rotate(dt / e.pos.Length(), 0, 0);
			});
	}
	Behaviors::Behavior& Behaviors::Get(uint8_t id)
	{
		auto& i = *instance;
		return i.behaviors[id % i.behaviors.size()];
	}
}