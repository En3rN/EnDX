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
		Vec2(DirectX::XMVECTOR xmvec)
		{
			DirectX::XMFLOAT2 tmp;
			DirectX::XMStoreFloat2(&tmp, xmvec);
			*this = { tmp.x, tmp.y };
		}
		Vec2& operator = (const Vec2& other) = default;
		Vec2& operator = (Vec2&& other) noexcept = default;
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
		Vec2 Normalized()
		{
			DirectX::XMFLOAT2 tmp = { x,y };
			return DirectX::XMVector2Normalize(DirectX::XMLoadFloat2(&tmp));
		}
		T Dot(Vec2 other) 
		{
			Vec2 tmp = DirectX::XMVector2Dot(*this, other);
			return tmp.x;
		}
		operator DirectX::XMFLOAT2() { return reinterpret_cast<DirectX::XMFLOAT2>(*this); }
		operator DirectX::XMFLOAT2* () { return reinterpret_cast<DirectX::XMFLOAT2*>(this); }
		operator DirectX::XMVECTOR () { return DirectX::XMLoadFloat2(*this); }
		operator const DirectX::XMFLOAT2 () const { return DirectX::XMFLOAT2{ x,y }; }
		operator const DirectX::XMVECTOR () const { return DirectX::XMLoadFloat2(*this); }

		~Vec2() = default;
		T x, y;
	};

	
	template <typename T>
	struct Vec3 
	{
		Vec3() = default;
		Vec3(T x, T y, T z) :x(x), y(y), z(z) {};
		Vec3(const Vec3& other) = default;
		Vec3(Vec3&& other) = default;
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
		Vec3 operator + (const Vec3 & other) const
		{
			return Vec3(x+other.x,y+other.y,z+other.z);
		}
		Vec3& operator +=(const Vec3& other)
		{
			x += other.x; y += other.y; z += other.z;
			return *this;
		}
		Vec3 operator - (const Vec3& other) const
		{
			return Vec3(x - other.x, y - other.y, z - other.z);
		}
		Vec3& operator -=(const Vec3& other)
		{
			x -= other.x; y -= other.y; z -= other.z;
			return *this;
		}
		template <typename Scalar>
		Vec3 operator * (const Scalar& multiplier) const
		{
			return Vec3(x * multiplier, y * multiplier, z * multiplier);
		}
		template <typename Scalar>
		Vec3& operator *= (const Scalar& multiplier) {
			return Vec3(x * multiplier, y * multiplier,z * multiplier);
		}
		bool operator == (const Vec3 & other) const
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
		T Length() const
		{
			return Vec3(DirectX::XMVector3Length(*this)).x;
		}
		T Dot(const Vec3& other)
		{
			Vec3 tmp = DirectX::XMVector3Dot(*this, other);

			return tmp.x;
		}
		bool operator = (const Vec3& other) const
		{
			bool result = true;
			x != other.x ? result = false : result = result;
			y != other.y ? result = false : result = result;
			z != other.z ? result = false : result = result;
			return result;
		}
		bool operator != (const Vec3& other) const
		{
			bool result = true;
			x = other.x ? result = false : result = result;
			y = other.y ? result = false : result = result;
			z = other.z ? result = false : result = result;
			return result;
		}
		
		template <typename castT>
		operator Vec3<castT>()							{ return { static_cast<castT>(x), static_cast<castT>(y), static_cast<castT>(z) }; }
		operator DirectX::XMFLOAT3()					{ return reinterpret_cast<DirectX::XMFLOAT3>(*this); }
		operator DirectX::XMFLOAT3* ()					{ return reinterpret_cast<DirectX::XMFLOAT3*>(this); }
		operator DirectX::XMVECTOR ()					{ return DirectX::XMLoadFloat3(*this);}
		operator DirectX::XMFLOAT3* ()			const	{ return (DirectX::XMFLOAT3*)(this); }
		operator const DirectX::XMFLOAT3()		const	{ return reinterpret_cast<DirectX::XMFLOAT3>(*this); }
		//operator const DirectX::XMFLOAT3* ()	const	{ return reinterpret_cast<DirectX::XMFLOAT3*>(this); }
		operator const DirectX::XMVECTOR() 		const	{ return DirectX::XMLoadFloat3(*this); }
		operator T* () { return {&x}; }
		T x, y, z;
	};
	template <typename T>
	struct Vec4 
	{
		T x,y,z,w;
	};
	using Vec2i =  Vec2<int>;
	using Vec2l = Vec2<long>;
	using Vec2f =  Vec2<float>;
	using Vec3f =  Vec3<float>;
	using Vec4f =  Vec4<float>;
	using Vec4u8 = Vec4<uint8_t>;

	

}

