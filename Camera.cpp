#include "Camera.h"
#include "Keyboard.h"
#include "EnDX.h"
#include "enMath.h"
#include "Transform.h"
#include <algorithm>

using namespace DirectX;
namespace En3rN::DX
{
	Camera* Camera::activeCam;

	Camera::Camera() : nearPlane(0.5f), farPlane(100)
	{
		auto r = Window::GetRect();
		w = r.width;
		h = r.height;
		SetDirection(0.0f, 0.0f, 1.0f);
		speed = 10;
		InitConstantBuffers();
		activeCam = this;
	}

	Camera::Camera(float x, float y, float z, uint8_t speed, float nearPlane, float farPlane) : nearPlane(nearPlane), farPlane(farPlane)
	{
		auto r = Window::GetRect();
		aspect = Window::GetAspectRatio();
		fov =  2.0 * atan(tan(halfPI * 0.5) / aspect); //horizontal angle convertion
		w = r.width;
		h = r.height;
		pos = { x,y,z };
		dir = { 0.0f, 0.0f, 1.0f };
		this->speed = speed;
		InitConstantBuffers();
		activeCam = this;
	}

	bool Camera::OnEvent(Event& e)
	{
		if (e.category == Event::Category::Keyboard && e.type == Event::Type::KeyDown)
			if (e.wparam == keyMappings[(int)KeyMapping::SetRawMode])
			{
				auto& eh = Window::GetEventHandler();
				Logger::Debug("setting rawcapture");
				eh.AddEvent(Event::Create(Event::Category::Window, Event::Type::RawCapture, 0, 0));
				eh.AddEvent(Event::Create(Event::Category::Mouse, Event::Type::RawCapture, 0, (LPARAM)this));
				rawCapture ? rawCapture = false : rawCapture = true;
				return true;
			}
		return false;
	}

	void Camera::OnRawCapture(float dt, Keyboard& kbd, Mouse& mouse)
	{
		Timer t("camRaw");
		uint8_t mapping = 0;
		for (auto i = 0; i < kbd.PressCount(); ++i)
		{
			for (auto& key : keyMappings)
			{
				if (kbd.IsPressed(key)) {
					KeyMapping keyMapping = KeyMapping(mapping);
					switch (keyMapping)
					{
					case En3rN::DX::KeyMapping::Forward:
						pos += (dir * speed * dt);
						break;
					case En3rN::DX::KeyMapping::Backward:
						pos += (dir * -1 * speed * dt);
						break;
					case En3rN::DX::KeyMapping::Left:
						pos += (dir.Rotate(0, -halfPI, 0) * speed * dt);
						break;
					case En3rN::DX::KeyMapping::Right:
						pos += (dir.Rotate(0, halfPI, 0) * speed * dt);
						break;
					case En3rN::DX::KeyMapping::Up:
						pos.y += speed * dt;
						break;
					case En3rN::DX::KeyMapping::Down:
						pos.y -= speed * dt;
						break;
					case En3rN::DX::KeyMapping::TurnLeft:						
						angles.x = WrapAngle(angles.x - dt * sensitivity*3);
						break;
					case En3rN::DX::KeyMapping::TurnRight:						
						angles.x = WrapAngle(angles.x + dt * sensitivity*3);
						break;
					case En3rN::DX::KeyMapping::LookUp:
						angles.y = std::clamp(angles.y - dt * sensitivity*3, -0.95f * halfPI, 0.95f * halfPI);
						break;
					case En3rN::DX::KeyMapping::LookDown:
						angles.y = std::clamp(angles.y + dt * sensitivity*3, -0.95f * halfPI, 0.95f * halfPI);
						break;
					}
				}
				mapping++;
			};
		};
		if (rawCapture)
		{
			Timer timer("mouseONLY");
			while (const auto delta = mouse.GetRawDelta())
			{
				angles.x = WrapAngle (angles.x + delta->x * sensitivity * 0.005f);
				angles.y = std::clamp(angles.y + delta->y * sensitivity * 0.005f, -0.95f * halfPI, 0.95f * halfPI);
			}
		}
		SetDirectionFromAngles();
	}

	/*void Camera::OnRawMouse(const long dx, const long dy) 
	{
		Timer d("mouseONLY");
		angles.x = std::clamp(
			angles.x +
			dy * sensitivity * 0.005f,
			-0.95f * halfPI,
			0.95f * halfPI);
		angles.y = WrapAngle(angles.y +dx * sensitivity * 0.005f);
		SetDirectionFromAngles();
	}*/
	
	DirectX::XMMATRIX Camera::GetViewMatrix() const
	{
		return XMMatrixTranspose(
			XMMatrixLookToLH(pos, dir, Vec3f(0, 1, 0))
			);
	}
	DirectX::XMMATRIX Camera::GetProjectionMatrix()
	{
		if (bfov) 
			return XMMatrixTranspose(XMMatrixPerspectiveFovLH(fov, aspect, nearPlane, farPlane));
		else
			return XMMatrixTranspose(XMMatrixPerspectiveLH(w, h, nearPlane,farPlane ));
	}		
	Camera& Camera::GetActiveCamera()
	{
		return *activeCam;
	}
	void Camera::SetActiveCamera(Camera& camera)
	{
		activeCam = &camera;
	}
	void Camera::SetConstantBuffer()
	{
		DirectX::XMMATRIX mvp[] = { GetViewMatrix() ,GetProjectionMatrix() };
		vsConstantBuffer->Update(*mvp);
	}	
	void Camera::InitConstantBuffers()
	{
		vsConstantBuffer = BindableManager::Query<Transform::ConstantBuffer>(2,0);
		vsConstantBuffer->Bind();
	}
}
