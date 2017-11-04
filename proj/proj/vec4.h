#pragma once

#include "Math.h"
#include <iostream>
#include "vec3.h"

#define TOLERANCE 1.0e-6

struct vec4 {
	float x;
	float y;
	float z;
	float w;

	vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	vec4(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W) {}
	vec4(const vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	vec4(const vec3& v, float f) : x(v.x), y(v.y), z(v.z), w(f) {}

	void Set(float X, float Y, float Z, float W) {
		x = X;
		y = Y;
		z = Z;
		w = W;
	}

	friend std::ostream&  vec4::operator<<(std::ostream& os, vec4& vec) {
		vec.clean();
		os << '(' << vec.x << ',' << ' ' << vec.y << ',' << ' ' << vec.z << ',' << ' ' << vec.w << ')';
		return os;
	}
	friend std::istream&  vec4::operator>>(std::istream& is, vec4& vec) {
		is >> vec.x >> vec.y >> vec.z >> vec.w;
		return is;
	}

	friend bool operator==(const vec4& v1, const vec4& v2) {
		return (fabs(v1.x - v2.x) < TOLERANCE
			&&  fabs(v1.y - v2.y) < TOLERANCE
			&&  fabs(v1.z - v2.z) < TOLERANCE
			&&  fabs(v1.w - v2.w) < TOLERANCE);
	}
	friend bool operator!=(const vec4& v1, const vec4& v2) {
		return !(v1 == v2);
	}

	friend vec4 operator+(const vec4& v1, const vec4& v2) {
		return vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
	}
	friend vec4 operator-(const vec4& v1, const vec4& v2) {
		return vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
	}
	friend vec4 operator*(const vec4& v, float scalar) {
		return vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
	}
	friend vec4 operator*(float scalar, const vec4& v) {
		return vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
	}
	friend vec4 operator/(const vec4& v, float scalar) {
		scalar = 1.0f / scalar;
		return vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
	}

	vec4& operator=(const vec4& v);
	vec4& operator+=(const vec4& v);
	vec4& operator-=(const vec4& v);
	vec4& operator*=(float scalar);
	vec4& operator/=(float scalar);

	void clean();
	void makeZero();
	bool isZero();

	float sqrMagnitude();
	float magnitude();
	void normalize();
	vec4 normalized();

	const float* toPointer() const;
};
