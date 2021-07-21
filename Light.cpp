#include "Light.h"

En3rN::DX::DirecionalLight::DirecionalLight()
{
	SetDirection(.5f, -.5f, 0.0f);
}

En3rN::DX::DirecionalLight::DirecionalLight(Vec3f dir)
{
	SetDirection(dir);
}
