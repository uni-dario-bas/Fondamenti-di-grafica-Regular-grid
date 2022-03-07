#pragma once

#include <vector>
#include <cstdio>
#include <iostream>
#include "point3D.h"
#include "point2D.h"
#include "vector3D.h"
#include "aabb.h"
#include "ray.h"
#include "object.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "instance.h"
#include "triangle.h"

using namespace std;

// Q: Eliminare e spostare tutta la logica dei triangoli in instance / mesh_instance?
class read_only_mesh {
public:

	vector<point3D> 			vertices;				// mesh vertices 
	vector<vector3D> 			normals;				// average normal at each vertex;
	vector<point2D>				textures;
	vector<vector<int> > 		vertex_faces;			// the triangles shared by each vertex
	int num_vertices; 									// number of vertices
	int num_normals;
	int num_textures;
	int num_shapes;
	int num_faces;
	aabb aabb_mesh;

	bool areNormals = false;

	read_only_mesh(const char* filename, const char* basepath = NULL, bool triangulate = true) {
		load_mesh(filename, basepath, triangulate);
	};

	bool load_mesh(const char* filename, const char* basepath, bool triangulate);

	vector<instance*> create_triangles();
};

bool read_only_mesh::load_mesh(const char* filename, const char* basepath = NULL, bool triangulate = true)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	std::string war;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &war, &err, filename, basepath, triangulate);

	if (!err.empty()) {
		std::cerr << err << std::endl;
		return false;
	}

	if (!ret) {
		printf("Failed to load/parse .obj.\n");
		return false;
	}

	num_vertices = (attrib.vertices.size() / 3);
	num_normals = (attrib.normals.size() / 3);
	num_textures = (attrib.texcoords.size() / 2);
	num_shapes = shapes.size();

	aabb_mesh.pmax = point3D(FLT_MIN, FLT_MIN, FLT_MIN);
	aabb_mesh.pmin = point3D(FLT_MAX, FLT_MAX, FLT_MAX);

	for (int v = 0; v < attrib.vertices.size() / 3; v++) {
		point3D p = point3D(attrib.vertices[3 * v + 0], attrib.vertices[3 * v + 1], attrib.vertices[3 * v + 2]);

		aabb_mesh.pmin.x = ffmin(p.x, aabb_mesh.pmin.x);
		aabb_mesh.pmin.y = ffmin(p.y, aabb_mesh.pmin.y);
		aabb_mesh.pmin.z = ffmin(p.z, aabb_mesh.pmin.z);
		aabb_mesh.pmax.x = ffmax(p.x, aabb_mesh.pmax.x);
		aabb_mesh.pmax.y = ffmax(p.y, aabb_mesh.pmax.y);
		aabb_mesh.pmax.z = ffmax(p.z, aabb_mesh.pmax.z);

		vertices.push_back(p);
	}

	if (attrib.normals.size() > 0)
	{
		areNormals = true;
		for (int v = 0; v < attrib.normals.size() / 3; v++) {
			normals.push_back(vector3D(attrib.normals[3 * v + 0], attrib.normals[3 * v + 1], attrib.normals[3 * v + 2]));
		}
	}
	
	if (attrib.texcoords.size() > 0)
	{
		for (int v = 0; v < attrib.texcoords.size() / 2; v++) {
			textures.push_back(point2D(attrib.texcoords[2 * v + 0], attrib.texcoords[2 * v + 1]));
		}
	}

	vertex_faces.push_back(vector<int>());
	vertex_faces.push_back(vector<int>());
	vertex_faces.push_back(vector<int>());

	num_faces = 0;
	// For each shape
	for (int s = 0; s < shapes.size(); s++) {
		int index_offset = 0;

		// For each face
		for (int f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			num_faces++;

			// For each vertex in the face
			for (int v = 0; v < fv; v++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				vertex_faces[0].push_back(idx.vertex_index);
				vertex_faces[1].push_back(idx.normal_index);
				vertex_faces[2].push_back(idx.texcoord_index);
			}

			index_offset += fv;
		}
	}

	return true;
}

vector<instance*> read_only_mesh::create_triangles() {
	vector<instance*> triangles = vector<instance*>();
	
	for (int i = 0; i < num_faces; i++)
	{
		int i0 = vertex_faces[0][3 * i + 0];
		int i1 = vertex_faces[0][3 * i + 1];
		int i2 = vertex_faces[0][3 * i + 2];

		point3D v0 = vertices[i0];
		point3D v1 = vertices[i1];
		point3D v2 = vertices[i2];

		triangles.push_back(new instance(new triangle(v0, v1, v2), new material(lightgray, lightgray, lightgray, 1)));
	}

	return triangles;
}

