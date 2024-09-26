#pragma once
#include "OpenGL.h"
#include "typeDefs.h"


struct Vec3 {
	float x = 0;
	float y = 0;
	float z = 0;
	float* r = nullptr;
	float* g = nullptr;
	float* b = nullptr;
	float* s = nullptr;
	float* t = nullptr;
	float* p = nullptr;

	Vec3() : x(0), y(0), z(0) {
		r = &x;
		g = &y;
		b = &z;
		s = &x;
		t = &y;
		p = &z;
	}
	Vec3(float val) : x(val), y(val), z(val) {
		r = &x;
		g = &y;
		b = &z;
		s = &x;
		t = &y;
		p = &z;
	}
	Vec3(float m, float n, float o) : x(m), y(n), z(o) {
		r = &x;
		g = &y;
		b = &z;
		s = &x;
		t = &y;
		p = &z;
	}

	glm::vec3 toGLM() {
		return glm::vec3(x, y, z);
	}

	Vec3& operator =(const Vec3& val) {
		this->x = val.x;
		this->y = val.y;
		this->z = val.z;
		return *this;
	}
};

struct iVec3 {
	int x = 0;
	int y = 0;
	int z = 0;
	int* r = nullptr;
	int* g = nullptr;
	int* b = nullptr;
	int* s = nullptr;
	int* t = nullptr;
	int* p = nullptr;

	iVec3() : x(0), y(0), z(0) {
		r = &x;
		g = &y;
		b = &z;
		s = &x;
		t = &y;
		p = &z;
	}
	iVec3(int val) : x(val), y(val), z(val) {
		r = &x;
		g = &y;
		b = &z;
		s = &x;
		t = &y;
		p = &z;
	}
	iVec3(int m, int n, int o) : x(m), y(n), z(o) {
		r = &x;
		g = &y;
		b = &z;
		s = &x;
		t = &y;
		p = &z;
	}
};

struct Vec2 {
	float x = 0;
	float y = 0;
	float* r = nullptr;
	float* g = nullptr;
	float* s = nullptr;
	float* t = nullptr;

	Vec2() : x(0), y(0) {
		r = &x;
		g = &y;
		s = &x;
		t = &y;
	}
	Vec2(float val) : x(val), y(val) {
		r = &x;
		g = &y;
		s = &x;
		t = &y;
	}
	Vec2(float m, float n) : x(m), y(n) {
		r = &x;
		g = &y;
		s = &x;
		t = &y;
	}

	glm::vec2 toGLM() {
		return glm::vec2(x, y);
	}

	Vec2& operator =(const Vec2& val) {
		if (this == &val)
			return *this;
		this->x = val.x;
		this->y = val.y;
		this->r = &this->x;
		this->g = &this->y;
		this->s = &this->x;
		this->t = &this->y;

		return *this;
	}
	Vec2& operator +=(const Vec2 val) {
		this->x += val.x;
		this->y += val.y;
		return *this;
	}
	Vec2& operator *=(const Vec2 val) {
		this->x *= val.x;
		this->y *= val.y;
		return *this;
	}
	Vec2 operator *(const Vec2 val) {
		Vec2 temp = *this;
		temp *= val;
		return temp;
	}
};

struct iVec2 {
	int x = 0;
	int y = 0;
	int* r = nullptr;
	int* g = nullptr;
	int* s = nullptr;
	int* t = nullptr;

	iVec2() : x(0), y(0) {
		r = &x;
		g = &y;
		s = &x;
		t = &y;
	}
	iVec2(int val) : x(val), y(val) {
		r = &x;
		g = &y;
		s = &x;
		t = &y;
	}
	iVec2(int m, int n) : x(m), y(n) {
		r = &x;
		g = &y;
		s = &x;
		t = &y;
	}
};

struct uiVec2 {
	uint x = 0;
	uint y = 0;
	uint* r = nullptr;
	uint* g = nullptr;
	uint* s = nullptr;
	uint* t = nullptr;

	uiVec2() : x(0), y(0) {
		r = &x;
		g = &y;
		s = &x;
		t = &y;
	}
	uiVec2(uint val) : x(val), y(val) {
		r = &x;
		g = &y;
		s = &x;
		t = &y;
	}
	uiVec2(uint m, uint n) : x(m), y(n) {
		r = &x;
		g = &y;
		s = &x;
		t = &y;
	}
};