#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>

struct Texture
{
    SDL_Texture *sdl_texture;
    int width, height;
    SDL_Rect src_rect;
};

Texture* texture_new(SDL_Texture* sdl_texture, int width, int height);
void texture_free(Texture *texture);
Texture* texture_load_from_bmp(SDL_Renderer* renderer, const char* path);
Texture* texture_load_from_png(SDL_Renderer* renderer, const char* path);
void texture_render(Texture *texture, SDL_Renderer* renderer, int x, int y);

#endif
