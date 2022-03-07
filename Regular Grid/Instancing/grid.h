#pragma once

#include "object_set.h"

class grid : public object_set {
public:
	grid::grid() {}

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	virtual bool hit_shadow(const ray& r, float t_min, float t_max) const;

	void computeCells();

	// Q: Potrebbe essere privato (o eliminato siccome è una singola istruzione)?
	void addObjectInCell(instance* new_object, int index) {
		cells[index]->addObject(new_object);	
	}

	void setMultiplier(int multiplier) {
		m = multiplier;
	}

	int nx, ny, nz;

// Q: Potrebbe essere private?
protected:
	vector<object_set*> cells;
	aabb grid_bb;
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
	float s = pow(wx * wy * wz / n_objects, 1.0 / 3.0);
	nx = (m * wx / s) + 1;
	ny = (m * wy / s) + 1;
	nz = (m * wz / s) + 1;

	int n_cells = nx * ny * nz;
	cout << "* Grid objects size: " << objects.size() << " Grid n_cells: " << n_cells << endl;

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
			//cout << "K: " << k << " Size: " << countObj << endl;
			count++;
		}
	}

	grid_bb = aabb(p0, p1);
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


bool grid::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {
	//TODO cambiare double in float ?

	double ox = r.o.x;
	double oy = r.o.y;
	double oz = r.o.z;
	double dx = r.d.x;
	double dy = r.d.y;
	double dz = r.d.z;

	double x0 = grid_bb.aabb_min().x;
	double y0 = grid_bb.aabb_min().y;
	double z0 = grid_bb.aabb_min().z;
	double x1 = grid_bb.aabb_max().x;
	double y1 = grid_bb.aabb_max().y;
	double z1 = grid_bb.aabb_max().z;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	tx_min = (x0 - ox) / dx;
	tx_max = (x1 - ox) / dx;
	if (tx_min > tx_max) 
		swap(tx_min, tx_max);

	ty_min = (y0 - oy) / dy;
	ty_max = (y1 - oy) / dy;
	if (ty_min > ty_max) 
		swap(ty_min, ty_max);

	tz_min = (z0 - oz) / dz;
	tz_max = (z1 - oz) / dz;
	if (tz_min > tz_max) 
		swap(tz_min, tz_max);

	double t0, t1;

	t0 = min(tx_min, ty_min, tz_min);
	t1 = min(tx_max, ty_max, tz_max);

	int ix, iy, iz;

	// TROVA LA PRIMA CELLA CHE IL RAGGIO ATTRAVERSA
	if (grid_bb.contains(r.o)) {
		ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
	} else {
		point3D p = r.o + t0 * r.d;
		ix = clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1);
		iy = clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1);
		iz = clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1);
	}

	double dtx = (tx_max - tx_min) / nx;
	double dty = (ty_max - ty_min) / ny;
	double dtz = (tz_max - tz_min) / nz;

	double 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;

	// Q: Eliminare il codice duplicato?
	tx_next = tx_min + (ix + 1) * dtx;
	ix_step = +1;
	ix_stop = nx;
	// DIREZIONE DEL RAGGIO NEGATIVA LUNGO X
	if (dx <= 0) {
		ix_step = -1;
		ix_stop = -1;
		tx_next = (dx == 0 ? FLT_MAX: tx_min + (nx - ix) * dtx);
	}

	tx_next = tx_min + (ix + 1) * dtx;
	ix_step = +1;
	ix_stop = nx;
	// DIREZIONE DEL RAGGIO NEGATIVA LUNGO X
	if (dx <= 0) {
		ix_step = -1;
		ix_stop = -1;
		tx_next = (dx == 0 ? FLT_MAX : tx_min + (nx - ix) * dtx);
	}

	ty_next = ty_min + (iy + 1) * dty;
	iy_step = +1;
	iy_stop = ny;
	// DIREZIONE DEL RAGGIO NEGATIVA LUNGO Y
	if (dy <= 0) {
		iy_step = -1;
		iy_stop = -1;
		ty_next = (dy == 0 ? FLT_MAX : ty_min + (ny - iy) * dty);
	}

	tz_next = tz_min + (iz + 1) * dtz;
	iz_step = +1;
	iz_stop = nz;
	// DIREZIONE DEL RAGGIO NEGATIVA LUNGO Z
	if (dz <= 0) {
		iz_step = -1;
		iz_stop = -1;
		tz_next = (dz == 0 ? FLT_MAX : tz_min + (nz - iz) * dtz);
	}

	int cont = 0;
	while (true) {
		cont++;
		object_set* objects_in_cells = cells[ix + nx * iy + nx * ny * iz];

		// Q: Refactor / metodo? Per ogni asse potrebbe essere utilizzata la programmazione non strutturata (if + continue)
		if (tx_next < ty_next && tx_next < tz_next) {
			if (objects_in_cells->hit(r, tmin, tmax, rec) && tmin < tx_next) {
				return true;
			}

			tx_next += dtx;
			ix += ix_step;

			if (ix == ix_stop) {
				return false;
			}

		}
		else {
			if (ty_next < tz_next) {
				if (objects_in_cells->hit(r, tmin, tmax, rec) && tmin < ty_next) {
					return true;
				}

				ty_next += dty;
				iy += iy_step;

				if (iy == iy_stop) {
					return false;
				}

			}
			else {
				if (objects_in_cells->hit(r, tmin, tmax, rec) && tmin < tz_next) {
					return true;
				}

				tz_next += dtz;
				iz += iz_step;

				if (iz == iz_stop) {
					return false;
				}
			}
		}
	}
	return false;
}

bool grid::hit_shadow(const ray& r, float t_min, float t_max) const {
	hit_record tmp;
	// Q: Potremmo introdurre il metodo traverse, a cui passiamo un booleano (useHitShadow) in modo da utilizzare un unico metodo?
	return hit(r, t_min, t_max, tmp);
	//return false;
}