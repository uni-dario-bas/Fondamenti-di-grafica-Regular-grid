#pragma once
#include "object.h"
#include "math.h"
#include "vector3D.h"
#include "point3D.h"
#include "instance.h"
#include <vector>


class object_set : public object {
public:
	object_set::object_set() {

	}

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	virtual bool hit_shadow(const ray& r, float t_min, float t_max) const;

	virtual bool bounding_box(aabb& box) const;

    point3D min_coordinates() const;
    point3D max_coordinates() const;

	void addObject(instance* new_object) {
		objects.push_back(new_object);
	}

    int countObjects() {
        return objects.size();
    }
         

protected:
	vector<instance*> objects;
    float EPSILON = 0.000001;
};

bool object_set::hit(const ray& ray, float t_min, float t_max, hit_record& rec) const {
	if (objects.size() == 0) {
		return false;
	}
	hit_record temp_rec;
	bool hit_anything = false;
	float closest_so_far = t_max;

	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->hit(ray, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
            temp_rec.m = objects[i]->getMaterial();
            rec = temp_rec;
		}
	}
	return hit_anything;
}

bool object_set::hit_shadow(const ray& ray, float t_min, float t_max) const {
	for (int i = 0; i < objects.size(); i++)
		if (objects[i]->hit_shadow(ray, 0.01f, FLT_MAX))
			return true;
	return false;
}

bool object_set::bounding_box(aabb& box) const {
	box = aabb(min_coordinates(), max_coordinates());
	return true;
}

point3D object_set::min_coordinates() const{
    aabb bbox_;
    point3D p0(FLT_MAX, FLT_MAX, FLT_MAX);
    int num_objects = objects.size();
    for (int i = 0; i < num_objects; i++) {
        objects[i]->bounding_box(bbox_);
        if (bbox_.aabb_min().x < p0.x) {
            p0.x = bbox_.aabb_min().x;
        }
        if (bbox_.aabb_min().y < p0.y) {
            p0.y = bbox_.aabb_min().y;
        }
        if (bbox_.aabb_min().z < p0.z) {
            p0.z = bbox_.aabb_min().z;
        }
    }
    p0.x -= EPSILON;
    p0.y -= EPSILON;
    p0.z -= EPSILON;
    return p0;
}

point3D object_set::max_coordinates() const{
    aabb bbox_;
    point3D p1(FLT_MIN, FLT_MIN, FLT_MIN);
    int num_objects = objects.size();
    for (int i = 0; i < num_objects; i++) {
        objects[i]->bounding_box(bbox_);
        if (bbox_.aabb_max().x > p1.x) {
            p1.x = bbox_.aabb_max().x;
        }
        if (bbox_.aabb_max().y > p1.y) {
            p1.y = bbox_.aabb_max().y;
        }
        if (bbox_.aabb_max().z > p1.z) {
            p1.z = bbox_.aabb_max().z;
        }
    }
    p1.x += EPSILON;
    p1.y += EPSILON;
    p1.z += EPSILON;
    return p1;
}
