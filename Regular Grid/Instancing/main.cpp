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
#include "object_set.h"
#include "grid.h"
#include "tassellator.h"
#include "scene_provider.h"


int const nx = 800;		
int const ny = 400;		
int const ns = 8;

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
	if ((initted & flags) != flags) {
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

void print_grid_stats(grid* grid_model, float time_grid) {
	cout << " ******** STATISTICHE GRIGLIA ********" << endl;
	cout << " Xmin: " << grid_model->min_coordinates().x << " Xmax: " << grid_model->max_coordinates().x << endl;
	cout << " Ymin: " << grid_model->min_coordinates().y << " Ymax: " << grid_model->max_coordinates().y << endl;
	cout << " Zmin: " << grid_model->min_coordinates().z << " Zmax: " << grid_model->max_coordinates().z << endl;
	cout << " CelleX: " << grid_model->nx << " CelleY: " << grid_model->ny << " CelleZ: " << grid_model->nz << endl;
	cout << " *************************************" << endl;
	cout << "\nTempo di Inizializzazione griglia: " << (clock() - time_grid) / 1000 << " seconds." << endl;
}

int main(int argc, char* argv[])
{

	if (init() == 1) {
		cout << "App Error! " << std::endl;
		system("pause");
		return 1;
	}
	srand(6);

	bool use_grid = true;
	grid* grid_model = new grid();
	scene_provider sp(ns, use_grid, nx, ny);


	// *********************** GRID SFERE ***********************
	// num_spheres = 1000000 side = 30 per costruire la griglia intorno alla camera
	int num_spheres = 100000;
	int side = 3;
	scene world = sp.get_sphere_scene(grid_model, num_spheres, side);
	//***********************************************************

	// *********************** GRID MESH  ***********************
	//scene world = sp.get_cat(grid_model, "../models/cat.obj", "../models/texturecat.jpg");
	//scene world = sp.get_person(grid_model, "../models/dennis.obj", "../models/dennis.jpg", 90);
	//scene world = sp.get_person(grid_model, "../models/fabienne.obj", "../models/fabienne.jpg", 180);
	// **********************************************************

	// *********************** GRID SFERE ***********************
	//bool always_compute_bb = true;
	//scene world = sp.get_cut_sphere_scene(grid_model, always_compute_bb);
	//***********************************************************

	if (use_grid && !grid_model->isEmpty()) {
		float time_grid = clock();

		instance* grid_ptr = new instance(grid_model, new material());
		grid_ptr->blockInstanceMaterialUse();
		world.addObject(grid_ptr);
		grid_model->setMultiplier(2.0);
		grid_model->computeCells();

		print_grid_stats(grid_model, time_grid);
	}

	float start = clock();

	world.parallel_render();

	float dif = (clock() - start) / 1000;
	cout << "\nTempo di Rendering: " << dif << " seconds." << endl;

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
