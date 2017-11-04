#include "vec3.h"

vec3& vec3::operator=(const vec3& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}
vec3& vec3::operator+=(const vec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
vec3& vec3::operator-=(const vec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
vec3& vec3::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}
vec3& vec3::operator/=(float scalar) {
	if (scalar != 0) {
		scalar = 1.0f / scalar;
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}
	return *this;
}

void vec3::clean() {
	if (fabs(x) < TOLERANCE) x = 0;
	if (fabs(y) < TOLERANCE) y = 0;
	if (fabs(z) < TOLERANCE) z = 0;
}

void vec3::makeZero() {
	Set(0.0f, 0.0f, 0.0f);
}
bool vec3::isZero() const {
	return (fabs(x) < TOLERANCE
		&& fabs(y) < TOLERANCE
		&& fabs(z) < TOLERANCE);
}

float vec3::sqrMagnitude() const {
	return x*x + y*y + z*z;
}
float vec3::magnitude() const  {
	return sqrt(sqrMagnitude());
}

void vec3::normalize() {
	float m = magnitude();
	if (m != 0) {
		m = 1.0f / m;

		x *= m;
		y *= m;
		z *= m;
	}
}
vec3 vec3::normalized() const {
	float m = magnitude();
	if (m != 0) {
		m = 1.0f / m;
		return vec3(x * m, y * m, z * m);
	}
	else return *this;
}

const float* vec3::toPointer() const {
	return &x;
}