#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <direct.h>

#include "texture.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;
const int SCREEN_SCALE = 2;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *surface;
SDL_Texture *render_target;

Texture *hello_world;
Texture *arrows;
Texture *frog;

void init();
void load_media();
void close();

void init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Failed to SDL init: %s\n", SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE, SDL_WINDOW_SHOWN);
	if (!window)
	{
		printf("Failed to create window: %s\n", SDL_GetError());
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	render_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!render_target)
	{
		printf("Failed to create render target texture: %s\n", SDL_GetError());
		exit(1);
	}

	int img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(img_flags) & img_flags))
	{
		printf("Failed to SDL_image init: %s\n", SDL_GetError());
		exit(1);
	}

	surface = SDL_GetWindowSurface(window);
}

enum LoadSurfaceType {
	LoadSurfaceType_BMP,
	LoadSurfaceType_PNG,
};

void load_media()
{
	hello_world = texture_load_from_bmp(renderer, "hello_world.bmp");
	if (!hello_world)
	{
		printf("Failed to load hello_world image\n");
		exit(1);
	}
	hello_world->height = SCREEN_HEIGHT / 2;

	arrows = texture_load_from_bmp(renderer, "arrows.bmp");
	if (!arrows)
	{
		printf("Failed to load arrows image\n");
		exit(1);
	}
	arrows->src_rect = { 0, 0, 100, 100 };
	arrows->width = 100;
	arrows->height = 100;

	frog = texture_load_from_png(renderer, "frog.png");
	if (!frog)
	{
		printf("Failed to load frog image\n");
		exit(1);
	}
}

void close()
{
	texture_free(hello_world);
	texture_free(arrows);
	texture_free(frog);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char **argv)
{
	char *base_path = SDL_GetBasePath();
	if (!base_path)
	{
		printf("Failed to get the base path: %s\n", SDL_GetError());
		return 1;
	}
	if(_chdir(base_path))
	{
		printf("Failed to change directory\n");
		return 1;
	}

	init();
	load_media();

	SDL_Rect top_left = { 0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	SDL_Rect top_right = { SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	SDL_Rect bottom = { 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 };

	SDL_Point arrow_pos = { 0, 0 };
	SDL_Point frog_pos = { 0, 0 };

	SDL_Rect green_rect = { 412, 20, 200, 150 };

	bool mouse_down;

	SDL_Event e;
	bool quit = false;

	while (quit == false)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
			{
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					mouse_down = e.button.state == SDL_PRESSED;
				}
			}
			else if(e.type == SDL_MOUSEMOTION)
			{
				frog_pos.x = e.motion.x / SCREEN_SCALE - frog->width / 2;
				frog_pos.y = e.motion.y / SCREEN_SCALE - frog->height / 2;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_LEFT:
					arrows->src_rect.x = 0;
					arrow_pos.x -= 50;
					break;
				case SDLK_RIGHT:
					arrows->src_rect.x = 100;
					arrow_pos.x += 50;
					break;
				case SDLK_UP:
					arrows->src_rect.x = 200;
					arrow_pos.y -= 50;
					break;
				case SDLK_DOWN:
					arrows->src_rect.x = 300;
					arrow_pos.y += 50;
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				}
			}

		}

		if (SDL_SetRenderTarget(renderer, render_target) < 0)
		{
			printf("Failed to set render target to texture\n", SDL_GetError());
			return 1;
		}

		hello_world->width = SCREEN_WIDTH / 2;

		// Top Left
		if (SDL_RenderSetViewport(renderer, &top_left))
		{
			printf("Failed to set top-left viewport!\n", SDL_GetError());
			return 1;
		}
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		texture_render(hello_world, renderer, 0, 0);
		texture_render(frog, renderer, frog_pos.x, frog_pos.y);
		
		// Top Right
		if (SDL_RenderSetViewport(renderer, &top_right))
		{
			printf("Failed to set top-right viewport!\n", SDL_GetError());
			return 1;
		}
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		texture_render(hello_world, renderer, 0, 0);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);

		SDL_Rect outline_rect = { frog_pos.x, frog_pos.y, frog->width, frog->height };
		SDL_RenderDrawRect(renderer, &outline_rect);

		// Bottom
		hello_world->width = SCREEN_WIDTH;
		if (SDL_RenderSetViewport(renderer, &bottom))
		{
			printf("Failed to set bottom viewport!\n", SDL_GetError());
			return 1;
		}
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		texture_render(hello_world, renderer, 0, 0);
		texture_render(arrows, renderer, arrow_pos.x, arrow_pos.y);
		
		SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0xFF);
		SDL_RenderFillRect(renderer, &green_rect);

		if (mouse_down)
		{
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			for (int x = 0; x < SCREEN_WIDTH; x += 50)
			{
				SDL_RenderDrawLine(renderer, x, 0, x, SCREEN_HEIGHT);
			}
			for (int y = 0; y < SCREEN_HEIGHT; y += 50)
			{
				SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
			}
		}

		if (SDL_SetRenderTarget(renderer, NULL) < 0)
		{
			printf("Failed to set render target to texture\n", SDL_GetError());
			return 1;
		}
		SDL_RenderCopy(renderer, render_target, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	close();

	return 0;
}
