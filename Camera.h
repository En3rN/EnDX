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
	
	class Camera : public Entity, public Controllable/*, public Mouse::RawListener*/
	{
		friend class Skybox;
	public:
		Camera();
		Camera(float x, float y, float z, uint8_t speed,float nearPlane ,float farPlane);
		bool OnEvent(Event& e) override;
		void OnRawCapture(float dt, Keyboard& kbd, Mouse& mouse) override;
		//void OnRawMouse(const long dx, const long dy) override;
		DirectX::XMMATRIX GetViewMatrix() const override;
		DirectX::XMMATRIX GetProjectionMatrix();		
		static Camera& GetActiveCamera();
		static void SetActiveCamera(Camera& camera);
		void SetConstantBuffer();

		static Camera* activeCam;		
		float w, h, nearPlane, farPlane;
		float fov , aspect;
		bool bfov = true;
	private:
		void InitConstantBuffers();
		std::shared_ptr<Transform::ConstantBuffer> vsConstantBuffer;
	};
}

