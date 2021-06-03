#pragma once
#include <DirectXMath.h>
#include <stdint.h>
#include <initializer_list>
namespace En3rN
{	
	template <typename T>
	struct Vec2
	{
	public:
		Vec2() = default;
		Vec2(T x,T y) : x(x), y(y) {};
		Vec2(const Vec2& other) = default;
		Vec2(Vec2&& rs) = default;
		Vec2& operator = (const Vec2& other) = default;
		Vec2& operator = (Vec2&& other) = default;
		Vec2 operator + (const Vec2& other) const
		{
			
			return Vec2(x + other.x, y + other.y);
		}
		Vec2& operator +=(const Vec2& other)
		{
			x += other.x; y += other.y;
			return *this;
		}
		Vec2 operator - (Vec2& other)
		{
			return Vec2(x - other.x, y - other.y);
		}
		bool operator == (Vec2& other)
		{
			if (x == other.x && y == other.y)
				return true;
			return false;
		}
		operator Vec2<T>() const 
		{ return { static_cast<T>(x), static_cast<T>(y) }; }

		template <typename T>
		Vec2 operator * (T multiplier)
		{			
			return Vec2(x * multiplier, y * multiplier);
		}
		T GetLength(Vec2& start,Vec2& end)
		{			
		}
		Vec2 Norm()
		{
			return *this;
		}
		~Vec2() = default;
		T x, y;
	};

	
	template <typename T>
	struct Vec3 
	{
		Vec3() = default;
		Vec3(T x, T y, T z) :x(x), y(y), z(z) {};
		//Vec3(T xAngle, T yAngle, T zAngle = 1, T lenght = 1) : x(cos(xAngle)), y(sin(yAngle)) {};
		Vec3(Vec3& other) = default;
		Vec3(Vec3&& other) = default;
		Vec3(std::initializer_list<T> list) : x(*(list.begin())), y(*(list.begin() + 1)), z(*(list.begin() + 2)) {};
		Vec3(const DirectX::XMVECTOR& other)
		{
			DirectX::XMFLOAT3 f3;
			DirectX::XMStoreFloat3(&f3,other);
			*this = f3;
		}
		Vec3(const DirectX::XMFLOAT3& other) : Vec3(other.x, other.y, other.z) {};
		Vec3& operator = (Vec3& other) = default;
		Vec3& operator = (const Vec3& other) = default;
		Vec3& operator = (Vec3 && other) = default;
		Vec3 operator + (const Vec3 & other) 
		{
			x += other.x; y += other.y; z += other.z;
			return *this;
		}
		Vec3 operator +=(const Vec3 & other)
		{
			x += other.x; y += other.y; z += other.z;
			return *this;
		}
		Vec3& operator -=(const Vec3& other)
		{
			x -= other.x; y -= other.y; z -= other.z;
			return *this;
		}
		Vec3 operator - (const Vec3 & other)
		{
			x -= other.x; y -= other.y; z -= other.z;
			return *this;
		}
		template <typename t>
		Vec3 operator * (const t multiplier)
		{
			return Vec3(x * multiplier, y * multiplier, z * multiplier);
		}
		template <typename t>
		Vec3 operator *= (t multiplier) {
			return Vec3(x * multiplier, y * multiplier,z * multiplier);
		}
		bool operator == (Vec3<T> & other)
		{
			if (x == other.x && y == other.y && z == other.z)
				return true;
			return false;
		}
		Vec3 Rotate(float pitch , float yaw, float roll) {
			
			return  DirectX::XMVector3Transform(*this, DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
		}
		Vec3 RotateX(float angle) {
			
			return DirectX::XMVector3Transform(*this, DirectX::XMMatrixRotationX(angle));
		}
		Vec3 RotateY(float angle) {
			return DirectX::XMVector3Transform(*this, DirectX::XMMatrixRotationY(angle));
		}
		void Normalize()
		{
			*this = DirectX::XMVector3Normalize(*this);
		}
		
		template <typename castT>
		Vec3 operator()(Vec3<castT>)
		{
			return { static_cast<castT>(x), static_cast<castT>(y), static_cast<castT>(z) };
		}
		operator DirectX::XMFLOAT3() {return DirectX::XMFLOAT3(x,y,z);}
		operator DirectX::XMVECTOR (){ 
			DirectX::XMFLOAT3 f3(x,y,z);
			return DirectX::XMLoadFloat3(&f3);
		}
		operator T* () { return {&x}; }
		T x, y, z;
	};
	template <typename T>
	struct Vec4 
	{
		T x,y,z,w;
	};
	using Vec2i =  Vec2<int>;
	using Vec2f =  Vec2<float>;
	using Vec3f =  Vec3<float>;
	using Vec4f =  Vec4<float>;
	using Vec4u8 = Vec4<uint8_t>;

	

}

