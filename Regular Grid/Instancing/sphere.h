#pragma once
#include "object.h"
#include "math.h"
#include "vector3D.h"
#include "point3D.h"

class sphere : public object {
public:
	sphere() {
		center = point3D(0.0, 0.0, 0.0);
		radius = 1.0f;
	}
	sphere(point3D cen, float r) : center(cen), radius(r){};

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record &rec) const;
	virtual bool hit_shadow(const ray& r, float t_min, float t_max) const;

	virtual bool bounding_box(aabb& box) const;

protected:
	point3D center;
	float radius;
};

bool sphere::bounding_box(aabb& box) const {
	box = aabb(center - vector3D(radius, radius, radius), center + vector3D(radius, radius, radius));
	return true;
}

bool sphere::hit(const ray& ray, float t_min, float t_max, hit_record& rec) const {
	vector3D oc = ray.origin() - center;
	float a = dot(ray.direction(), ray.direction());
	float b = dot(oc, ray.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = ray.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = ray.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false;
}

bool sphere::hit_shadow(const ray& ray, float t_min, float t_max) const {
	vector3D oc = ray.origin() - center;
	float a = dot(ray.direction(), ray.direction());
	float b = dot(oc, ray.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			return true;
		}
	}
	return false;
}

