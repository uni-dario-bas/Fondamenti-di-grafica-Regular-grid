#pragma once

#include "object_set.h"

class grid : public object_set {
public:
	grid::grid() {}

	//virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	//virtual bool hit_shadow(const ray& r, float t_min, float t_max) const;

	void computeCells();

	void addObjectInCell(instance* new_object, int index) {
		cells[index]->addObject(new_object);	
	}

	void setMultiplier(int multiplier) {
		m = multiplier;
	}

	int nx, ny, nz;

protected:
	vector<object_set*> cells;
	float m = 2.0;
	void populateCells(int obj_index, float ixmin, float iymin, float izmin, float ixmax, float iymax, float izmax);
};

void grid::computeCells() {

	point3D p0 = min_coordinates();
	point3D p1 = max_coordinates();

	int n_objects = objects.size();
	float wx = p1.x - p0.x;
	float wy = p1.y - p0.y;
	float wz = p1.z - p0.z;
	float s = pow(wx * wy * wz / n_objects, 0.33333333);
	nx = (m * wx / s) + 1;
	ny = (m * wy / s) + 1;
	nz = (m * wz / s) + 1;

	int n_cells = nx * ny * nz;

	for (int j = 0; j < n_cells; j++) {
		cells.push_back(new object_set());
	}

	aabb obj_bb;

	for (int i = 0; i < n_objects; i++) {
		//OCCHIO QUI ALLA BB
		objects[i]->bounding_box(obj_bb);

		int ixmin = clamp((obj_bb.aabb_min().x - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
		int iymin = clamp((obj_bb.aabb_min().y - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
		int izmin = clamp((obj_bb.aabb_min().z - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);
		int ixmax = clamp((obj_bb.aabb_max().x - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
		int iymax = clamp((obj_bb.aabb_max().y - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
		int izmax = clamp((obj_bb.aabb_max().z - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);

		populateCells(i, ixmin, iymin, izmin, ixmax, iymax, izmax);
	}

	int count = 0;
	for (int k = 0; k < cells.size(); k++) {
		int countObj = cells[k]->countObjects();
		if (countObj > 0) {
			cout << "K: " << k << " Size: " << countObj << endl;
			count++;
		}
	}
	cout << "********** STATISTICHE CELLE ************" << endl;
	cout << "Celle non vuote: " << count << " Celle vuote: " << cells.size() - count << endl;
	cout << "*****************************************" << endl << endl;
}

void grid::populateCells(int obj_index, float ixmin, float iymin, float izmin, float ixmax, float iymax, float izmax) {
	int cell_index;
	for (int x = ixmin; x <= ixmax; x++) {
		for (int y = iymin; y <= iymax; y++) {
			for (int z = izmin; z <= izmax; z++) {	
				cell_index = x + (nx * y) + (nx * ny * z);
				addObjectInCell(objects[obj_index], cell_index);
			}
		}
	}
}

//bool grid::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
//	return true;
//}
//bool grid::hit_shadow(const ray& r, float t_min, float t_max) const {
//	return true;
//}