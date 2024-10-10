#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>

SDL_Texture* loadTexture(const char* filePath, SDL_Renderer *renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath);
    if (texture == NULL) {
        printf("Failed to load texture: %s\n", IMG_GetError());
    }
    return texture;
}
