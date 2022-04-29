#include "Camera.h"
#include "Keyboard.h"
#include "EnDX.h"
#include "enMath.h"
#include "Transform.h"
#include <algorithm>
#include <stdint.h>

using namespace DirectX;
namespace En3rN::DX
{
	Camera* Camera::m_activeCam = nullptr;

	Camera::Camera(float x, float y, float z, uint8_t speed, float nearPlane, float farPlane) : 
		m_nearPlane(nearPlane), 
		m_farPlane(farPlane),
		m_speed(speed),
		m_transform({x,y,z},{0.f,0.f,0.f},{1,1,1})
	{
		InitProjectionVariables();
		InitConstantBuffers();
	}

	Camera::Camera(const Transform& transform, uint8_t speed, float nearPlane, float farPlane) :
		m_transform(transform),
		m_speed(speed),
		m_nearPlane(nearPlane),
		m_farPlane(farPlane)
	{
		InitProjectionVariables();
		InitConstantBuffers();
	}
	void Camera::InitProjectionVariables()
	{
		auto r = Window::GetRect();
		m_aspect = Window::GetAspectRatio();
		m_fov = 2.0 * atan(tan(halfPI * 0.5) / m_aspect); //horizontal angle convertion
		m_width = r.width;
		m_height = r.height;
	}

	bool Camera::OnEvent(Event& e)
	{
		if (e.category == Event::Category::Keyboard && e.type == Event::Flag::KeyDown)
			if (e.wparam == keyMappings[(int)KeyMapping::SetRawMode])
			{
				auto& eh = Window::GetEventHandler();
				Logger::Debug("setting rawcapture");
				eh.AddEvent(Event(Event::Category::Window, Event::Flag::RawCapture, 0, 0));
				eh.AddEvent(Event(Event::Category::Mouse, Event::Flag::RawCapture, (WPARAM)this , 0));
				ToggleRawCaptureMode();
				return true;
			}
		if(e.type == Event::Flag::RawCapture){
			ToggleRawCaptureMode();
		}
		return false;
	}

	void Camera::OnRawCapture(float dt, Keyboard& kbd, Mouse& mouse)
	{
		auto& pos = m_transform.Position();
		auto dir = m_transform.GetDirection();
		auto& angles = m_transform.Angles();
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
						pos += (dir * m_speed * dt);
						break;
					case En3rN::DX::KeyMapping::Backward:
						pos += (dir * -1 * m_speed * dt);
						break;
					case En3rN::DX::KeyMapping::Left:
						pos += (-m_transform.GetXAxis() * m_speed * dt);
						break;
					case En3rN::DX::KeyMapping::Right:
						pos += (m_transform.GetXAxis() * m_speed * dt);
						break;
					case En3rN::DX::KeyMapping::Up:
						pos.y += m_speed * dt;
						break;
					case En3rN::DX::KeyMapping::Down:
						pos.y -= m_speed * dt;
						break;
					case En3rN::DX::KeyMapping::TurnLeft:						
						angles.y = Wrap(angles.y - dt * sensitivity*3,0.f,twoPi);
						break;
					case En3rN::DX::KeyMapping::TurnRight:						
						angles.y = Wrap(angles.y + dt * sensitivity*3,0.f,twoPi);
						break;
					case En3rN::DX::KeyMapping::LookUp:
						angles.x = std::clamp(angles.x - dt * sensitivity*3, -0.95f * halfPI, 0.95f * halfPI);
						break;
					case En3rN::DX::KeyMapping::LookDown:
						angles.x = std::clamp(angles.x + dt * sensitivity*3, -0.95f * halfPI, 0.95f * halfPI);
						break;
					}
				}
				mapping++;
			};
		};
		if (rawCapture)
		{			
			while (const auto delta = mouse.GetRawDelta())
			{
				angles.y = Wrap(angles.y + delta->x * sensitivity * 0.005f,0.f,twoPi);
				angles.x = std::clamp(angles.x + delta->y * sensitivity * 0.005f, -0.95f * halfPI, 0.95f * halfPI);
			}
		}
		SetDirectionFromAngles();
	}

	void Camera::ToggleRawCaptureMode()
	{
		rawCapture = rawCapture ? false : true;
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
		XMVECTOR position = m_transform.GetPosition();
		XMVECTOR direction = m_transform.GetDirection();

		return XMMatrixTranspose(
			XMMatrixLookToLH(position, direction, Vec3f(0, 1, 0))
			);
	}

	DirectX::XMMATRIX Camera::GetProjectionMatrix()
	{
		if (m_useFovProjection) 
			return XMMatrixTranspose(XMMatrixPerspectiveFovLH(m_fov, m_aspect, m_nearPlane, m_farPlane));
		else
			return XMMatrixTranspose(XMMatrixPerspectiveLH(m_width, m_height, m_nearPlane, m_farPlane ));
	}

	Camera& Camera::GetActiveCamera()
	{
		return *m_activeCam;
	}
	
	float Camera::FovToHorizontal(float radAngle, float aspectRatio)
	{
		return 2.0 * atan(tan(radAngle * 0.5) / aspectRatio);
	}

	void Camera::SetActiveCamera(Camera& camera)
	{
		m_activeCam = &camera;
	}

	void Camera::SetConstantBuffer()
	{
		Transform::Matrix m[] = { GetViewMatrix(),DirectX::XMMatrixTranspose(m_transform.GetMatrix()), GetProjectionMatrix() };
		vsConstantBuffer->Update(*m);
	}
	void Camera::SetDirectionFromAngles() 
	{
		/*Vec3 base(.0f, .0f, 1.f);
		dir = base.Rotate(angles.y, angles.x, angles.z);*/
	}
	bool Camera::UIControls()
	{
		bool returnVal;
		if (ImGui::DragFloat3("Camera", m_transform.Position(), 0.05f, -1000.0f, 1000.0f)) {
			returnVal = true;
		}
		if (ImGui::DragFloat3("CameraAngles", m_transform.Angles(), 0.05f, 0, twoPi))
			returnVal = true;
		Vec3f dir = m_transform.GetDirection();
		ImGui::Text("CameraDirVec: %.2f, %.2f, %.2f", dir.x, dir.y, dir.z);
			
		if (ImGui::DragFloat4("Proj", &m_width, .1f, .1f, 2000.f)) {
			returnVal = true;
		};
		if (ImGui::Checkbox("FOV", &m_useFovProjection)) {
			returnVal = true;
		}
		ImGui::SameLine();
		if(ImGui::DragFloat2("FOV", &m_fov, .01f, .1f, 2.f))
			returnVal = true;
		return false;
	}

	void Camera::InitConstantBuffers()
	{
		vsConstantBuffer = BindableManager::Query<Transform::ConstantBuffer>(3, 0);
		vsConstantBuffer->Bind();
	}
	void Camera::Container::AddCamera(const Camera& camera)
	{
		UnregisterActiveCamera();
		m_cameras.push_back(camera);
		Camera::SetActiveCamera(m_cameras[m_activeIndex]);
		RegisterActiveCamera();
	}
	void Camera::Container::RemoveCamera(Index index)
	{
		assert(index < m_cameras.size());
		UnregisterActiveCamera();
		m_cameras.erase(m_cameras.begin() + index);
		assert(index < m_cameras.size());
		RegisterActiveCamera();
	}
	void Camera::Container::SetActiveCamera(Index index)
	{
		assert(index < m_cameras.size());
		UnregisterActiveCamera();
		m_activeIndex = index;
		Camera::SetActiveCamera(m_cameras[index]);
		RegisterActiveCamera();
	}
	Camera& Camera::Container::GetActiveCamera()
	{
		return m_cameras[m_activeIndex];
	}
	void Camera::Container::RegisterActiveCamera()
	{
		m_cameras[m_activeIndex].Register();
	}
	void Camera::Container::UnregisterActiveCamera()
	{
		if(!empty())
			m_cameras[m_activeIndex].Unregister();
	}
}
