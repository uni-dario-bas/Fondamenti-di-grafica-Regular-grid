#pragma once
#include <iostream>
#include "scene.h"
#include "grid.h"
#include "sphere.h"
using namespace std;

class scene_provider {
public:

	scene_provider(int ns, bool grid, int nx_scene, int ny_scene) {
		camera_ns = ns;
		use_grid = grid;
		nx = nx_scene;
		ny = ny_scene;
	}

	scene get_mesh_scene(grid* grid_model, char* mesh_path, char* texture_path) {
		scene world;
		add_light(world);
		add_camera(world);
		mesh* mesh_ptr = new mesh(mesh_path, "./");
		texture* texture_ptr = new image_texture(texture_path);
		material* mesh_material = new material();
		mesh_material->texture = texture_ptr;
		instance* mesh_instance = new instance(mesh_ptr, mesh_material);
		mesh_instance->rotate_y(90);
		mesh_instance->rotate_z(90);
		mesh_instance->rotate_y(45);
		mesh_instance->scale(0.05, 0.05, 0.05);
		mesh_instance->translate(0, -1, 0);
		if (use_grid) {
			vector<instance*> triangles = create_triangles(mesh_ptr, mesh_instance->getCurrentMatrix(),
				mesh_instance->getInverseMatrix(), texture_ptr);
			for (instance* t : triangles) {
				grid_model->addObject(t);
			}
		}
		else {
			world.addObject(mesh_instance);
		}
		return world;
	}

	scene get_dennis(grid* grid_model, char* mesh_path, char* texture_path) {
		scene world;
		add_light(world);
		add_camera(world);
		mesh* mesh_ptr = new mesh(mesh_path, "./");
		texture* texture_ptr = new image_texture(texture_path);
		material* mesh_material = new material();
		mesh_material->texture = texture_ptr;
		instance* mesh_instance = new instance(mesh_ptr, mesh_material);
		mesh_instance->scale(0.015, 0.015, 0.015);
		mesh_instance->rotate_y(90);
		mesh_instance->translate(0, -1.5, 0);
		if (use_grid) {
			vector<instance*> triangles = create_triangles(mesh_ptr, mesh_instance->getCurrentMatrix(),
				mesh_instance->getInverseMatrix(), texture_ptr);
			for (instance* t : triangles) {
				grid_model->addObject(t);
			}
		}
		else {
			world.addObject(mesh_instance);
		}
		return world;
	}

	scene get_sphere_scene(grid* grid_model, float num_spheres, float side) {
		scene world;
		add_light(world);
		add_camera(world);
		object* sphere_model = new sphere();
		instance* sphere_ptr = new instance(sphere_model, new material());
		float sphere_volume = 1.0 / num_spheres;
		float sphere_radius = pow(((3.0 / 4) * sphere_volume) / 3.14, 1.0 / 3);
		for (int k = 0; k < num_spheres; k++) {
			sphere_ptr = new instance(sphere_model, new material());
			sphere_ptr->rotate_x(45);
			sphere_ptr->scale(sphere_radius, sphere_radius, sphere_radius);
			sphere_ptr->translate(randMToN(-side, side), randMToN(-side, side), randMToN(-side, side));
			sphere_ptr->scale(0.4, 1.2, 0.4);
			sphere_ptr->alwaysComputeBB();
			sphere_ptr->rotate_z(45);
			use_grid ? grid_model->addObject(sphere_ptr) : world.addObject(sphere_ptr);
		}
		return world;
	}

	scene get_test(grid* grid_model) {
		scene world;

		point3D light_position(0.0f, 7.0f, 0.0f);
		point_light* light = new point_light(light_position, lightgray, lightgray, lightgray);
		world.addLight(light);

		point3D lookfrom(0.0, 6.0, -5.0);
		point3D lookat(0, 0, 0);
		vector3D up(0, 1, 0);
		world.setCamera(lookfrom, lookat, up, 20, nx, ny, camera_ns);

		//object* sphere_model = new sphere();
		//instance* sphere_ptr = new instance(sphere_model, new material());
		//sphere_ptr->translate(0, 2, 0);
		//use_grid ? grid_model->addObject(sphere_ptr) : world.addObject(sphere_ptr);
		//sphere_ptr = new instance(sphere_model, new material());
		//sphere_ptr->scale(1000.0, 1000.0, 1000.0);
		//sphere_ptr->translate(0, -1000, 0);
		//world.addObject(sphere_ptr);
		return world;
	}

private:
	int camera_ns;
	bool use_grid;
	int nx;
	int ny;

	void add_light(scene& world) {
		point3D light_position(0.0f, 0.0f, -10.0f);
		point_light* light = new point_light(light_position, lightgray, lightgray, lightgray);
		world.addLight(light);
	}

	void add_camera(scene& world) {
		point3D lookfrom(0.0, 0.0, -10.0);
		point3D lookat(0, 0, 0);
		vector3D up(0, 1, 0);
		world.setCamera(lookfrom, lookat, up, 20, nx, ny, camera_ns);
	}
};