#include "Camera.h"
#include "Keyboard.h"
#include "EnDX.h"
#include "enMath.h"
#include <algorithm>


using namespace DirectX;
namespace En3rN::DX
{
	Camera* Camera::activeCam;

	Camera::Camera()
	{
		viewDistance = 100.0;
		SetDirection(0.0f, 0.0f, 1.0f);
		speed = 10;
		UpdateViewMatrix();
		activeCam = this;
	}

	Camera::Camera(float x, float y, float z,uint8_t speed ,float viewDistance) :  viewDistance(viewDistance)
	{
		pos = { x,y,z };
		dir = { 0.0f, 0.0f, 1.0f };
		this->speed = speed;
		UpdateViewMatrix();
		activeCam = this;
	}

	bool Camera::OnEvent(Event& e)
	{
		if (e.category == Event::Category::Keyboard && e.type == Event::Type::KeyDown)
			if (e.wparam == keyMappings[(int)KeyMapping::SetRawMode])
			{
				Logger::Debug("setting rawcapture");
				Event(Event::Category::Window, Event::Type::RawCapture, 0, 0);
				Event(Event::Category::Mouse, Event::Type::RawCapture, 0, 0);
				rawCapture ? rawCapture = false : rawCapture = true;
				return true;
			}
		return false;
	}

	void Camera::OnRawCapture(float dt, Keyboard& kbd, Mouse& mouse)
	{
		Timer t("camRaw");
		uint8_t mapping = 0;
		for (auto key : keyMappings)
		{
			KeyMapping keyMapping = KeyMapping(mapping);
			if (kbd.IsPressed(key)) {
				switch (keyMapping)
				{
				case En3rN::DX::KeyMapping::Forward:
					pos += (dir * speed* dt);
						break;
				case En3rN::DX::KeyMapping::Backward:
					pos += (dir * -1 * speed * dt);
						break;
				case En3rN::DX::KeyMapping::Left:
					pos += (dir.Rotate(0, -PI / 2, 0) * speed * dt);
						break;
				case En3rN::DX::KeyMapping::Right:
					pos += (dir.Rotate(0, PI / 2, 0) * speed * dt);
					break;
				case En3rN::DX::KeyMapping::Up:
					pos.y += speed * dt;
					break;
				case En3rN::DX::KeyMapping::Down:
					pos.y -= speed * dt;
					break;
				case En3rN::DX::KeyMapping::TurnLeft:
					angles.y -= dt;
					/*dir = dir.Rotate(0, -dt, 0);*/
					break;
				case En3rN::DX::KeyMapping::TurnRight:
					angles.y += dt;
					//dir = dir.Rotate(0, dt, 0);
					break;
				case En3rN::DX::KeyMapping::LookUp:
				{
					angles.x = std::clamp(angles.x + dt,-0.95f*PI/2, 0.95f * PI / 2);

					/*dir.y = std::clamp(dir.y + dt, -.95f, .95f);
					SetDirection(dir);*/
					break;
				}
				case En3rN::DX::KeyMapping::LookDown:
				{
					angles.x=std::clamp(angles.x - dt, -0.95f * PI / 2, 0.95f * PI / 2);
					/*dir.y = std::clamp(dir.y - dt, -.95f, .95f);
					SetDirection(dir);*/
					break;
				}
				}
			}
			mapping++;
		};
		if (rawCapture)
		{
			auto mousedelta = mouse.GetRawDelta();
			while (mousedelta != Vec2i(0, 0)) 
			{
				angles.x = std::clamp(angles.x +
					(float)mousedelta.y * sensitivity * 0.005f,
					-0.95f * PI / 2,
					0.95f * PI / 2);

				angles.y = WrapAngle(angles.y + (float)mousedelta.x * sensitivity * 0.005f);
				mousedelta = mouse.GetRawDelta();
			}
		}
		SetDirectionFromAngles();
	}
	
	void Camera::UpdateViewMatrix()
	{
		viewMatrix = XMMatrixLookToLH(pos, dir, Vec3f(0,1,0));
	}
	DirectX::XMMATRIX Camera::GetProjectionMatrix()
	{
		auto w = Window::GetRect();
		return DirectX::XMMatrixPerspectiveFovLH(PI/3.f,Window::GetAspectRatio() , .1f, viewDistance);
	}
	Camera& Camera::GetActiveCamera()
	{
		return *activeCam;
	}
	void Camera::SetActiveCamera(Camera& camera)
	{
		activeCam = &camera;
	}
}
