#include "vec4.h"

vec4& vec4::operator=(const vec4& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}
vec4& vec4::operator+=(const vec4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}
vec4& vec4::operator-=(const vec4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}
vec4& vec4::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}
vec4& vec4::operator/=(float scalar) {
	if (scalar != 0) {
		scalar = 1.0f / scalar;
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
	}
	return *this;
}

void vec4::clean() {
	if (fabs(x) < TOLERANCE) x = 0;
	if (fabs(y) < TOLERANCE) y = 0;
	if (fabs(z) < TOLERANCE) z = 0;
	if (fabs(w) < TOLERANCE) w = 0;
}

void vec4::makeZero() {
	Set(0.0f, 0.0f, 0.0f, 0.0f);
}
bool vec4::isZero() {
	return (fabs(x) < TOLERANCE
		&& fabs(y) < TOLERANCE
		&& fabs(z) < TOLERANCE
		&& fabs(w) < TOLERANCE);
}

float vec4::sqrMagnitude() {
	return x*x + y*y + z*z + w*w;
}
float vec4::magnitude() {
	return sqrt(sqrMagnitude());
}

void vec4::normalize() {
	float m = magnitude();
	if (m != 0) {
		m = 1.0f / m;

		x *= m;
		y *= m;
		z *= m;
		w *= m;
	}
}
vec4 vec4::normalized() {
	float m = magnitude();
	if (m != 0) {
		m = 1.0f / m;
		return vec4(x * m, y * m, z * m, w * m);
	}
	else return *this;
}

const float* vec4::toPointer() const {
	return &x;
}