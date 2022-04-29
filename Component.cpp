#include "Component.h"
#include "Camera.h"
#include "Window.h"
#include "Model.h"
#include "Mesh.h"


namespace En3rN::DX
{
	struct CameraComponent
	{
		float FovHorizontal = Camera::FovToHorizontal(halfPI, Window::GetAspectRatio());
		float ClipPlaneNear = .5f;
		float ClipPlaneFar = 1000.f;
		float AspectRatio = Window::GetAspectRatio();
	};
}