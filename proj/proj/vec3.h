#pragma once

#include "Math.h"
#include <iostream>

#define TOLERANCE 1.0e-6

struct vec3 {
		float x;
		float y;
		float z;

		vec3() : x(0.0f), y(0.0f), z(0.0f) {}
		vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
		vec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}

		void Set(float X, float Y, float Z) {
			x = X;
			y = Y;
			z = Z;
		}

		friend std::ostream&  vec3::operator<<(std::ostream& os, vec3& vec) {
			vec.clean();
			os << '(' << vec.x << ',' << ' ' << vec.y << ',' << ' ' << vec.z << ')';
			return os;
		}
		friend std::istream&  vec3::operator>>(std::istream& is, vec3& vec) {
			is >> vec.x >> vec.y >> vec.z;
			return is;
		}

		friend bool operator==(const vec3& v1, const vec3& v2) {
			return (fabs(v1.x - v2.x) < TOLERANCE
				&&  fabs(v1.y - v2.y) < TOLERANCE
				&&  fabs(v1.z - v2.z) < TOLERANCE);
		}
		friend bool operator!=(const vec3& v1, const vec3& v2) {
			return !(v1 == v2);
		}

		friend vec3 operator+(const vec3& v1, const vec3& v2) {
			return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
		}
		friend vec3 operator-(const vec3& v) {
			return vec3(-v.x, -v.y, -v.z);
		}
		friend vec3 operator-(const vec3& v1, const vec3& v2) {
			return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
		}
		
		friend vec3 operator*(const vec3& v, float scalar) {
			return vec3(v.x*scalar, v.y*scalar, v.z*scalar);
		}
		friend vec3 operator*(float scalar, const vec3& v) {
			return vec3(v.x*scalar, v.y*scalar, v.z*scalar);
		}
		friend vec3 operator/(const vec3& v, float scalar) {
			scalar = 1.0f / scalar;
			return vec3(v.x*scalar, v.y*scalar, v.z*scalar);
		}

		vec3& operator=(const vec3& v);
		vec3& operator+=(const vec3& v);
		vec3& operator-=(const vec3& v);
		vec3& operator*=(float scalar);
		vec3& operator/=(float scalar);


		friend float dotProduct(const vec3& v1, const vec3& v2) {
			return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		}
		friend vec3 crossProduct(const vec3& v1, const vec3& v2) {
			return vec3(v1.y*v2.z - v1.z*v2.y,
				v1.z*v2.x - v1.x*v2.z,
				v1.x*v2.y - v1.y*v2.x);
		}
		friend vec3 lerp(const vec3& v1, const vec3& v2, float k) {
			return (1 - k) * v1 + k * v2;
		}
		friend float angleBetween(const vec3& a, const vec3& b)
		{
			float angle = dotProduct(a, b);
			angle /= (a.magnitude() * b.magnitude());
			return angle = acosf(angle);
		}

		void clean();
		void makeZero();
		bool isZero() const;

		float sqrMagnitude() const;
		float magnitude() const;
		void normalize();
		vec3 normalized() const;

		const float* toPointer() const;
	};