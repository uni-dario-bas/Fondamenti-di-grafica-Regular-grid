#pragma once
#include <vector>
#include "instance.h"
#include "triangle.h"
#include "mesh.h"
#include "mesh_triangle.h"


vector<instance*> create_triangles(mesh* m, matrix4D current_matrix, matrix4D inverse_matrix, texture* tex) {
	vector<instance*> triangles = vector<instance*>();
	cout << "entrato nel create" << endl;
	material* triangle_mat = new material();
	triangle_mat->texture = tex;
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

		// ASSEGNO I PUNTI DI TEXTURE
		i0 = m->vertex_faces[2][3 * i + 0];
		i1 = m->vertex_faces[2][3 * i + 1];
		i2 = m->vertex_faces[2][3 * i + 2];

		point2D uv0 = m->textures[i0];
		point2D uv1 = m->textures[i1];
		point2D uv2 = m->textures[i2];

		mesh_triangle* tri = new mesh_triangle(v0, v1, v2);
		tri->uv0_texture = uv0;
		tri->uv1_texture = uv1;
		tri->uv2_texture = uv2;
		// ************************


		instance* triangle_instance = new instance(tri, triangle_mat);
		triangles.push_back(triangle_instance);
	}
	return triangles;
}