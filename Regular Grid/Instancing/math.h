#pragma once
#include <stdlib.h>
#include "color.h"
#include "vector3D.h"
#include <vector>

const float Deg2Rad = 3.1415926f / 180.0f;

inline float lerp(float a, float b, float t) {
	return (1.0f - t) * a + t * b;
}

inline vector3D lerp(vector3D &a, vector3D &b, float t) {
	return (1.0f - t) * a + t * b;
}

inline color lerp(color &a, color &b, float t) {
	return (1.0f - t) * a + t * b;
}

float randZeroToOne(){
	return float(rand() / (RAND_MAX + 1.));
}

double randMToN(double M, double N){
	return M + (rand() / (RAND_MAX / (N - M)));
}

double randMinusOneToOne() {
	return randMToN(-1.0, 1.0);
}

float max(const float a, const float b)
{
	return (a < b) ? b : a;
}

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

inline int clamp(int num, int min, int max) {
	if (num < min) {
		return min;
	}
	if (num > max) {
		return max;
	}
	return num;
}

inline void swapNumber(float& a, float& b) {
	float tmp = a;
	a = b;
	b = tmp;
}

 point3D findMaxInPoints(std::vector<point3D> points) {
	float x1 = -FLT_MAX;
	float y1 = -FLT_MAX;
	float z1 = -FLT_MAX;
	for (point3D p : points) {
		if (p.x > x1)
			x1 = p.x;
	}
	for (point3D p : points) {
		if (p.y > y1)
			y1 = p.y;
	}
	for (point3D p : points) {
		if (p.z > z1)
			z1 = p.z;
	}
	return point3D(x1, y1, z1);
}

 point3D findMinInPoints(std::vector<point3D>& points) {
	float x1 = FLT_MAX;
	float y1 = FLT_MAX;
	float z1 = FLT_MAX;
	for (point3D p : points) {
		if (p.x < x1)
			x1 = p.x;
	}
	for (point3D p : points) {
		if (p.y < y1)
			y1 = p.y;
	}
	for (point3D p : points) {
		if (p.z < z1)
			z1 = p.z;
	}
	return point3D(x1, y1, z1);
}
