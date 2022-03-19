#pragma once
#include "Entity.h"
#include "Event.h"
#include "Controllable.h"
#include "enMath.h"
#include "Buffer.h"
#include "BindableManager.h"
#include "Transform.h"
#include <DirectXMath.h>


namespace En3rN::DX
{
	
	class Camera : public Controllable/*, public Mouse::RawListener*/
	{
		friend class Skybox;
	public:
		using handle = std::shared_ptr<Camera>;
		class Container;
		Camera() = default;
		Camera(float x, float y, float z, uint8_t speed,float nearPlane ,float farPlane);
		Camera(const Transform& transform, uint8_t speed = 10, float nearPlane= 0.5f, float farPlane=1000);

		void InitProjectionVariables();

		bool OnEvent(Event& e) override;
		void OnRawCapture(float dt, Keyboard& kbd, Mouse& mouse) override;
		void ToggleRawCaptureMode();
		
		DirectX::XMMATRIX GetViewMatrix() const;
		DirectX::XMMATRIX GetProjectionMatrix();
		const Vec3f& GetPosition() const { return m_transform.GetPosition(); }
		static Camera& GetActiveCamera();

		static float FovToHorizontal(float radAngle, float aspectRatio);
		static void SetActiveCamera(Camera& camera);
		void SetConstantBuffer();
		void SetDirectionFromAngles();

		bool UIControls();

	private:
		void InitConstantBuffers();
		float m_width, m_height, m_nearPlane, m_farPlane, m_fov , m_aspect;
		bool m_useFovProjection = true;
		
		float m_speed;
		Transform m_transform;
		std::shared_ptr<Transform::ConstantBuffer> vsConstantBuffer;
		static Camera* m_activeCam;		
	};

	class Camera::Container
	{
	public:
		using Index = uint32_t;
		void AddCamera(const Camera& camera);
		void RemoveCamera(Index index);
		void SetActiveCamera(Index index);
		Camera& GetActiveCamera();
		
		auto size()		{ return m_cameras.size(); }
		auto empty()	{ return m_cameras.empty(); }
		auto begin()	{ return m_cameras.begin(); }
		auto end()		{ return m_cameras.end(); }

		auto& operator [] (Index index) { return m_cameras[index]; }

	private:
		void RegisterActiveCamera();
		void UnregisterActiveCamera();
		std::vector<Camera> m_cameras;
		Index				m_activeIndex{};
	};
}

