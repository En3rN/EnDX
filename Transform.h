#pragma once
#include "Buffer.h"
#include "enMath.h"
#include "vec.h"
#include <assimp\matrix4x4.h>
namespace En3rN::DX
{
    class Transform 
    {
	public:
		
		using Matrix = DirectX::XMMATRIX;
		using ConstantBuffer = VSConstantBuffer<Matrix>;
		Transform() : m_position({ 0,0,0 }), m_angles({ 0,0,0 }), m_scale({ 1,1,1 }) {};
		Transform(const Matrix& transform);
		void toEuler(double x, double y, double z, double angle);
		Transform(Vec3f position, Vec3f angles = {0,0,0}, Vec3f scale = {1,1,1});

		~Transform() = default;

		const Matrix    GetMatrix() const;
		Vec3f&			Position();
		Vec3f&			Angles();
		Vec3f&			Scale();
		const Vec3f&	GetPosition() const;
		Vec3f			GetDirection() const;
		Vec3f			GetXAxis() const;
		Vec3f			GetUpAxis() const;

		static void		Listener (void(int i, char c));
		static Matrix   GetMatrix(Vec3f& pos, Vec3f& angles, Vec3f& scale);
		static Vec3f	GetDirection(Vec3f& angles);

		
		bool			UIControls();

		// component
		/*virtual void	OnAttach()	override;
		virtual void	OnUpdate()	override;
		virtual void	OnDetach()	override;*/
	private:
		Vec3f m_position{};
		Vec3f m_angles{};
		Vec3f m_scale{};
    };
}

