#pragma once
#include "Entity.h"
#include "Event.h"
#include "Controllable.h"
#include <DirectXMath.h>


namespace En3rN::DX
{
	
	class Camera : public Entity, public Controllable
	{
		friend class Skybox;
	public:
		Camera();
		Camera(float x, float y, float z, uint8_t speed, float viewDistance);
		bool OnEvent(Event& e) override;
		void OnRawCapture(float dt, Keyboard& kbd, Mouse& mouse) override;
		void UpdateViewMatrix() override;
		DirectX::XMMATRIX GetProjectionMatrix();
		static Camera& GetActiveCamera();
		static void SetActiveCamera(Camera& camera);
	private:
		static Camera* activeCam;
		float viewDistance;
	};
}

