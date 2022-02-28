#include <iostream>
#include <time.h>
#include <stdio.h>
#include "SDL.h" 
#include "SDL_image.h"
#include "float.h"
#include "vector3D.h"
#include "point3D.h"
#include "color.h"
#include "ray.h"
#include "geometry.h"
#include "cylinder.h"
#include "sphere.h"
#include "instance.h"
#include "mesh.h"
#include "raster.h"
#include "object.h"
#include "camera.h"
#include "color.h"
#include "scene.h"
#include "texture.h"

int const nx = 800;		//x
int const ny = 400;		//y
int const ns = 1;		//number of samples

const unsigned int MAX_RAY_DEPTH = 5;

using namespace std;

int init() {
	/* // Initialize SDL2. */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	/* Create the window where we will draw. */
	window = SDL_CreateWindow("Regular Grid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, nx, ny, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	//Initialize PNG,JPG,TIF loading
	int flags = IMG_INIT_JPG;
	int initted = IMG_Init(flags);
	if ((initted&flags) != flags) {
		cout << "SDL_image could not initialize! SDL_image Error:" << IMG_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	/* We must call SDL_CreateRenderer in order for draw calls to affect this window. */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		SDL_DestroyWindow(window);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		IMG_Quit();
		SDL_Quit();
		return 1;
	}
	return 0;
}

void close() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (init() == 1) {
		cout << "App Error! " << std::endl;
		return 1;
	}

	scene world;

	/*
	// Texture Mapping preliminare
	point3D light_position(0.0f, 10.0f, 0.0f);
	point_light *light = new point_light(light_position, black, lightgray, black);
	world.addLight(light);

	point3D lookfrom(0, 2, 10);
	point3D lookat(0, 0, -1);
	vector3D up(0, 1, 0);
	world.setCamera(lookfrom, lookat, up, 20, nx, ny, ns);

	object* model3d = new mesh("../models/cat.obj", "../models/");
	checker_texture* checker_tex = new checker_texture(new constant_texture(color(0.2f, 0.3f, 0.1f)), new constant_texture(color(0.9f, 0.9f, 0.9f)));
    //image_texture("texturecat.jpg");
	material* m = new material();
	m->texture = checker_tex;
	instance* mesh_ptr = new instance(model3d, m);
	
	mesh_ptr->scale(1.0, 1.0, 1.0);
	mesh_ptr->rotate_y(90.0f);
	mesh_ptr->rotate_z(90.0f);
	mesh_ptr->translate(0.0f, -40.0f, -120.0f);
	world.addObject(mesh_ptr);
	*/
	
	point3D light_position(6.0f, 6.0f, 0.0f);
	point_light *light = new point_light(light_position, lightgray, lightgray, lightgray);
	world.addLight(light);

	point3D lookfrom(13, 5, 3);
	point3D lookat(0, 0, 0);
	vector3D up(0, 1, 0);
	world.setCamera(lookfrom, lookat, up, 20, nx, ny, ns);

	srand(5);

	object* sphere_model = new sphere();
	instance* sphere_ptr = new instance(sphere_model, new material());
	//sphere_ptr->getMaterial()->reflective = 0.8;
	sphere_ptr->scale(1000.0, 1000.0, 1000.0);
	sphere_ptr->translate(0, -1000, 0);
	world.addObject(sphere_ptr);
	
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = randZeroToOne();
			point3D center(a + 0.9f*randZeroToOne(), 0.2f, b + 0.9f*randZeroToOne());

			sphere_ptr = new instance(sphere_model, new material());
			//sphere_ptr->getMaterial()->reflective = 1.0;
			sphere_ptr->scale(0.2f, 0.2f, 0.2f);
			sphere_ptr->translate(center.x, center.y, center.z);
			world.addObject(sphere_ptr);
		}
	}
	
	sphere_ptr = new instance(sphere_model, new material());
	//sphere_ptr->getMaterial()->reflective = 0.2;
	sphere_ptr->translate(0, 1, 0);
	world.addObject(sphere_ptr);

	sphere_ptr = new instance(sphere_model, new material());
	//sphere_ptr->getMaterial()->reflective = 0.5;
	sphere_ptr->translate(-4, 1, 0);
	world.addObject(sphere_ptr);

	sphere_ptr = new instance(sphere_model, new material());
	sphere_ptr->getMaterial()->reflective = 1.0;
	sphere_ptr->translate(4, 1, 0);
	world.addObject(sphere_ptr);
	
	time_t start, end;
	time(&start);

	//world.parallel_render();
	world.render();

	time(&end);
	double dif = difftime(end, start);
	cout << "Rendering time: " << dif << " seconds." << endl;

	SDL_Event event;
	bool quit = false;

	/* Poll for events */
	while (SDL_PollEvent(&event) || (!quit)) {

		switch (event.type) {

		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				quit = true;
				break;
				// cases for other keypresses

			case SDLK_s:
				saveScreenshotBMP("screenshot.bmp");
				cout << "Screenshot saved!" << endl;
				break;
			}
		}
	}

	close();
	return 0;
}
