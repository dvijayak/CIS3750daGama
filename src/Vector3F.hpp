#ifndef Vector3F_hpp
#define Vector3F_hpp

#include "global.hpp"

class Vector3F
{
public:
	Vector3F (float x=0.0, float y=0.0, float z=0.0) : m_x(x), m_y(y), m_z(z) { Update(); }
	Vector3F (const Vector3F& other) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z) { Update(); }
	Vector3F& operator= (const Vector3F& other);
	~Vector3F () {}

	// Standard arithmetic ops
	const Vector3F operator+ (const Vector3F& other) const;
	const Vector3F& operator+= (const Vector3F& other);
	const Vector3F operator- (const Vector3F& other) const;
	const Vector3F& operator-= (const Vector3F& other);
	const Vector3F operator* (const float scalar) const;
	const Vector3F& operator*= (const float scalar);
	// "Multiplication" with another vector is defined in this case as the dot product with the vector
	float operator* (const Vector3F& other) const;
	const Vector3F operator/ (const float scalar) const;
	const Vector3F& operator/= (const float scalar);

	// Computes the inner angle (in radians) between the two vectors
	float Angle (const Vector3F& other) const;

	// Dot product
	float Dot (const Vector3F& other) const;
	// Cross product
	const Vector3F Cross (const Vector3F& other) const;

	// Normalize the vector (can be used as an lvalue)
	Vector3F& Normalize ();
	// Normalize the vector but copy the result to the provided vector (overwriting it's existing data)
	void Normalize (Vector3F& target) const;

	// Accesors/Mutators
	inline void SetX (float x) { m_x = x; Update(); }
	inline void SetY (float y) { m_y = y; Update(); }
	inline void SetZ (float z) { m_z = z; Update(); }
	inline float GetX ()	const { return m_x; }
	inline float GetY ()	const { return m_y; }
	inline float GetZ ()	const { return m_z; }
	inline float GetNorm ()	const { return m_norm; }
	inline float GetMagnitude () const { return m_norm; } // A convenience	

	friend std::ostream& operator<< (std::ostream& os, const Vector3F& v);

private:
	float m_x, m_y, m_z; // coordinates
	float m_norm; // magnitude

	// Called whenever x, y or z are changed
	void Update ();

	// Calculate the magnitude of the vector
	void ComputeNorm ();
};

Vector3F& Vector3F::operator= (const Vector3F& other)
{
	m_x = other.m_x;
	m_y = other.m_y;
	m_z = other.m_z;

	Update();

	return *this;
}

void Vector3F::Update ()
{
	ComputeNorm();
}

inline void Vector3F::ComputeNorm ()
{
	m_norm = sqrt( Dot(*this) );
}

inline const Vector3F Vector3F::operator+ (const Vector3F& other) const
{
	return Vector3F(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

inline const Vector3F& Vector3F::operator+= (const Vector3F& other)
{
	return *this = *this + other;
}

inline const Vector3F Vector3F::operator- (const Vector3F& other) const
{
	return Vector3F(m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
}

inline const Vector3F& Vector3F::operator-= (const Vector3F& other)
{
	return *this = *this - other;
}

inline const Vector3F Vector3F::operator* (const float scalar) const
{
	return Vector3F(m_x * scalar, m_y * scalar, m_z * scalar);
}

inline const Vector3F& Vector3F::operator*= (const float scalar)
{
	return *this = *this * scalar;
}

inline float Vector3F::operator* (const Vector3F& other) const
{
	return Dot(other);
}

inline const Vector3F Vector3F::operator/ (const float scalar) const
{
	return *this * (1.0/scalar);
}

inline const Vector3F& Vector3F::operator/= (const float scalar)
{
	return *this = *this / scalar;
}


float Vector3F::Angle (const Vector3F& other) const
{
	// this . other = ||this|| * ||other|| * cos(theta)

	float dot = Dot(other);
	float normProduct = m_norm * other.m_norm;
	return acos(dot/normProduct);
}

inline float Vector3F::Dot (const Vector3F& other) const
{
	return m_x*other.m_x + m_y*other.m_y + m_z*other.m_z;
}

inline const Vector3F Vector3F::Cross (const Vector3F& other) const
{	
	return Vector3F(
			m_y*other.m_z - m_z*other.m_y,
			m_z*other.m_x - m_x*other.m_z,
			m_x*other.m_y - m_y*other.m_x
		);
}

inline Vector3F& Vector3F::Normalize ()
{
	*this /= m_norm;
	return *this;
}

inline void Vector3F::Normalize (Vector3F& target) const
{
	target = *this;
	target.Normalize();
}


std::ostream& operator<< (std::ostream& os, const Vector3F& v)
{
	os << "(" << v.m_x << ", " << v.m_y << ", " << v.m_z << " | " << v.m_norm << ")";

	return os;
}

#endif
