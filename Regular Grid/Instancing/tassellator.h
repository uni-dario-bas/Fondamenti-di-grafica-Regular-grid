#pragma once
#include <vector>
#include "instance.h"
#include "triangle.h"
#include "mesh.h"
#include "mesh_triangle.h"


class Tassellator {
	private:
		static void say_hello();

	public:
		static vector<instance*> create_triangles(mesh* m, matrix4D current_matrix, matrix4D inverse_matrix, texture* tex);
};

vector<instance*> Tassellator::create_triangles(mesh* m, matrix4D current_matrix, matrix4D inverse_matrix, texture* tex) {
	say_hello();
	
	vector<instance*> triangles = vector<instance*>();
	material* triangle_mat = new material();
	triangle_mat->texture = tex;

	for (int i = 0; i < m->num_faces; i++){

		int i0 = m->vertex_faces[0][3 * i + 0];
		int i1 = m->vertex_faces[0][3 * i + 1];
		int i2 = m->vertex_faces[0][3 * i + 2];

		point3D v0 = m->vertices[i0];
		point3D v1 = m->vertices[i1];
		point3D v2 = m->vertices[i2];

		//Strategia alternativa: applicazione della trasformazione diretta sui singoli vertici
		//point3D v0 = current_matrix * m->vertices[i0];
		//point3D v1 = current_matrix *  m->vertices[i1];
		//point3D v2 = current_matrix * m->vertices[i2];

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

		instance* triangle_instance = new instance(tri, triangle_mat, current_matrix, inverse_matrix);
		triangle_instance->alwaysComputeBB();

		//Strategia alternativa: applicando la trasformazione a priori, non � necessario passare all'istanza le matrici di trasformazione
		//instance* triangle_instance = new instance(tri, triangle_mat);

		triangles.push_back(triangle_instance);
	}
	return triangles;
}

void Tassellator::say_hello() {
	cout << endl;
	cout << "      _________ _______  _______  _______  _______  _        _        _______ _________ _______  _______          " << endl;
	cout << "      \\__   __/(  ___  )(  ____ \\(  ____ \\(  ____ \\( \\      ( \\      (  ___  )\\__   __/(  ___  )(  ____ )         " << endl;
	cout << "         ) (   | (   ) || (    \\/| (    \\/| (    \\/| (      | (      | (   ) |   ) (   | (   ) || (    )|         " << endl;
	cout << "         | |   | (___) || (_____ | (_____ | (__    | |      | |      | (___) |   | |   | |   | || (____)|         " << endl;
	cout << "   )     | |   |  ___  |(_____  )(_____  )|  __)   | |      | |      |  ___  |   | |   | |   | ||     __)    )    " << endl;
	cout << "  ) \\    | |   | (   ) |      ) |      ) || (      | |      | |      | (   ) |   | |   | |   | || (\\ (      ) \\  " << endl;
	cout << " / ) (   | |   | )   ( |/\\____) |/\\____) || (____/\\| (____/\\| (____/\\| )   ( |   | |   | (___) || ) \\ \\__  / ) (  " << endl;
	cout << " \\(_)/   )_(   |/     \\|\\_______)\\_______)(_______/(_______/(_______/|/     \\|   )_(   (_______)|/   \\__/  \\(_)/ " << endl;
	cout << endl;
}