#include "Camera.h"
#include "Keyboard.h"
#include "EnDX.h"
#include <algorithm>


using namespace DirectX;
namespace En3rN::DX
{
	Camera* Camera::activeCam;

	Camera::Camera()
	{
		viewDistance = 100.0;
		SetDirection(0.0f, 0.0f, 1.0f);
		speed = 3;
		UpdateViewMatrix();
		activeCam = this;
	}

	Camera::Camera(float x, float y, float z, float viewDistance) : viewDistance(viewDistance)
	{
		pos = { x,y,z };
		dir = { 0.0f, 0.0f, 1.0f };
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
			}
			
		return false;
	}

	void Camera::OnRawCapture(float dt, Keyboard& kbd, Mouse& mouse)
	{
		if (!rawCapture) return;
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
					dir = dir.Rotate(0, -dt, 0);
					break;
				case En3rN::DX::KeyMapping::TurnRight:
					dir = dir.Rotate(0, dt, 0);
					break;
				case En3rN::DX::KeyMapping::LookUp:
				{
					dir.y = std::clamp(dir.y + dt, -.95f, .95f);
					SetDirection(dir);
					break;
				}
				case En3rN::DX::KeyMapping::LookDown:
				{
					dir.y = std::clamp(dir.y - dt, -.95f, .95f);
					SetDirection(dir);
					break;
				}
				}
			}
			mapping++;
		};
		auto mousedelta = mouse.GetRawDelta();
		while (mousedelta.has_value()){
			dir.y = std::clamp(dir.y - (float)mousedelta.value().y* sensitivity * .01f, -.95f, .95f);
			dir = dir.RotateY((float)mousedelta.value().x* sensitivity* .01f);
			dir.Normalize();
			mousedelta =mouse.GetRawDelta();
		} 
		

	}
	
	void Camera::UpdateViewMatrix()
	{
		viewMatrix = XMMatrixLookToLH(pos, dir, Vec3f(0,1,0));
	}
	DirectX::XMMATRIX Camera::GetProjectionMatrix()
	{
		auto w = Window::GetRect();
		return DirectX::XMMatrixPerspectiveFovLH(PI/2,Window::GetAspectRatio() , 0.5f, viewDistance);
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
