#pragma once
#include "Component.h"
#include "Buffer.h"
#include <assimp\matrix4x4.h>
namespace En3rN::DX
{
    class Transform : Component
    {
	public:
		using Matrix = DirectX::XMMATRIX;
		using ConstantBuffer = VSConstantBuffer<Matrix>;
		Transform();
		Transform(aiMatrix4x4 mat);
		Transform(Vec3f position, Vec3f rotationAngles, Vec3f scale);

		~Transform() = default;
		

		const Matrix&   GetMatrix() const;
		void			SetTransform(Vec3f position, Vec3f rotationAngles, Vec3f scale);
		void			ApplyTransform(const Matrix& transform);
		void			Rotate(float roll, float pitch, float yaw);
		void            Translate(float x, float y, float z);
		void			Scale(float x, float y, float z);

		// component
		virtual void	OnAttach()	override {}
		virtual void	Bind()		override;
		virtual void	OnUpdate()	override;
		virtual void	OnDetach()	override {}
	private:
		Matrix							m_matrix;
		std::shared_ptr<ConstantBuffer> m_constantbuffer;
    };
}

