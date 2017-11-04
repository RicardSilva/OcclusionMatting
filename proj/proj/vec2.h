#pragma once
struct vec2 {

	float x;
	float y;

	vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}
	vec2(float X_, float Y_)
	{
		x = X_;
		y = Y_;
	}
	friend bool operator==(const vec2& v1, const vec2& v2) {
		return (v1.x == v2.x && v1.y == v2.y);
	}
	friend bool operator!=(const vec2& v1, const vec2& v2) {
		return !(v1 == v2);
	}
	friend vec2 operator+(const vec2& v1, const vec2& v2) {
		return vec2(v1.x + v2.x, v1.y + v2.y);
	}
	friend vec2 operator-(const vec2& v1, const vec2& v2) {
		return vec2(v1.x - v2.x, v1.y - v1.y);
	}
	friend vec2 operator*(const vec2& v, float scalar) {
		return vec2(v.x*scalar, v.y*scalar);
	}
	friend vec2 operator*(float scalar, const vec2& v) {
		return vec2(v.x*scalar, v.y*scalar);
	}

	
};