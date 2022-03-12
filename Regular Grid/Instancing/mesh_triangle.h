#pragma once
#include "object.h"
#include "math.h"
#include "vector3D.h"
#include "point3D.h"
#include "triangle.h"
#include "point2D.h"

class mesh_triangle : public triangle {
public:

	point2D uv0_texture;
	point2D uv1_texture;
	point2D uv2_texture;

	mesh_triangle(point3D _v0, point3D _v1, point3D _v2): triangle(_v0, _v1, _v2) {};

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

};


bool mesh_triangle::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	const float EPSILON = 0.0000001f;
	vector3D edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = v1 - v0;
	edge2 = v2 - v0;
	h = cross(r.d, edge2);
	a = dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
		return false;
	f = 1 / a;
	s = r.o - v0;
	u = f * (dot(s, h));
	if (u < 0.0f || u > 1.0f)
		return false;
	q = cross(s, edge1);
	v = f * dot(r.d, q);
	if (v < 0.0f || u + v > 1.0f)
		return false;
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * dot(edge2, q);
	if (t > tmin && t < tmax) {
		if (t > EPSILON) // ray intersection
		{
			point3D intersPoint = r.o + (normalize(r.d) * (t * magnitude(r.d)));
			rec.normal = norm;
			rec.t = dot((v0 - r.o), norm) / dot(r.direction(), norm);
			rec.p = intersPoint;
			vector3D bary(1.0f - u - v, u, v);
			point2D uv = uv0_texture * bary.x + uv1_texture * bary.y + uv2_texture * bary.z;
			rec.u = uv.x;
			rec.v = uv.y;
			return true;
		}
	}
	// This means that there is a line intersection but not a ray intersection.
	return false;
}

