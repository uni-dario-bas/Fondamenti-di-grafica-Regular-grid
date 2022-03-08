#pragma once
#include <vector>
#include "instance.h"
#include "triangle.h"
#include "mesh.h"


vector<instance*> create_triangles(mesh* m, matrix4D current_matrix, matrix4D inverse_matrix) {
	vector<instance*> triangles = vector<instance*>();
	cout << "entrato nel create" << endl;
	for (int i = 0; i < m->num_faces; i++){

		if (i % 10000 == 0) {
			cout << "i: " << i << endl;
		}

		int i0 = m->vertex_faces[0][3 * i + 0];
		int i1 = m->vertex_faces[0][3 * i + 1];
		int i2 = m->vertex_faces[0][3 * i + 2];

		point3D v0 = current_matrix * m->vertices[i0];
		point3D v1 = current_matrix *  m->vertices[i1];
		point3D v2 = current_matrix * m->vertices[i2];

		//material* triangle_mat = new material();
		instance* triangle_instance = new instance(new triangle(v0, v1, v2), new material());
		triangles.push_back(triangle_instance);
	}
	return triangles;
}