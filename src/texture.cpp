#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "texture.h"

Texture* texture_new(SDL_Texture* sdl_texture, int width, int height)
{
    Texture *texture = (Texture*)malloc(sizeof(Texture));
    if (!texture)
    {
        printf("Failed to malloc a texture");
        return NULL;
    }

    texture->sdl_texture = sdl_texture;
    texture->width = width;
    texture-> height = height;
    texture->src_rect = { 0, 0, width, height };

    return texture;
}

void texture_free(Texture *texture)
{
    SDL_DestroyTexture(texture->sdl_texture);
    free(texture);
}

Texture* texture_load_from_bmp(SDL_Renderer* renderer, const char* path)
{
    SDL_Surface *surface = SDL_LoadBMP(path);

    if (!surface)
    {
        printf("Failed to load image %s: %s\n", path, SDL_GetError());
        return NULL;
    }

    SDL_Texture *sdl_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!sdl_texture)
    {
        printf("Failed to create texture for %s: %s\n", path, SDL_GetError());
        return NULL;
    }

    Texture *texture = texture_new(sdl_texture, surface->w, surface->h);
    SDL_FreeSurface(surface);

    return texture;
}

Texture* texture_load_from_png(SDL_Renderer* renderer, const char* path)
{
    SDL_Surface *surface = IMG_Load(path);

    if (!surface)
    {
        printf("Failed to load image %s: %s\n", path, IMG_GetError());
        return NULL;
    }

    SDL_Texture *sdl_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!sdl_texture)
    {
        printf("Failed to create texture for %s: %s\n", path, SDL_GetError());
        return NULL;
    }

    Texture *texture = texture_new(sdl_texture, surface->w, surface->h);
    SDL_FreeSurface(surface);
    
    return texture;
}

void texture_render(Texture *texture, SDL_Renderer* renderer, int x, int y)
{
    SDL_Rect dest_rect = { x, y, texture->width, texture->height};
    SDL_RenderCopy(renderer, texture->sdl_texture, &texture->src_rect, &dest_rect);
}
