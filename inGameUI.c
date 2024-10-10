#include "inGameUI.h"
#include "dimensions.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Player.h"

void renderTileHighlight(SDL_Renderer *renderer, int tileX, int tileY, SDL_Texture *tileset, SDL_Rect *camera, Player *player) {
    SDL_Rect src;

    SDL_Rect highlightRect;
    switch(player->currentEditCursorSize) {
        case 1:
            highlightRect.x = tileX * TILE_SIZE - camera->x;
            highlightRect.y = tileY * TILE_SIZE - camera->y;
            src.x = 0;
            src.y = 3*ORIGINAL_TILE_SIZE;
        break;
        case 2:
            highlightRect.x = tileX * TILE_SIZE - camera->x;
            highlightRect.y = tileY * TILE_SIZE - camera->y;
            src.x = 3*ORIGINAL_TILE_SIZE;
            src.y = 0;
        break;
        case 3:
            highlightRect.x = tileX * TILE_SIZE - TILE_SIZE - camera->x;
            highlightRect.y = tileY * TILE_SIZE - TILE_SIZE - camera->y;
            src.x = 0;
            src.y = 0;
        break;
        default:
            highlightRect.x = tileX * TILE_SIZE - camera->x;
            highlightRect.y = tileY * TILE_SIZE - camera->y;
    }

    highlightRect.w = TILE_SIZE*player->currentEditCursorSize;
    highlightRect.h = TILE_SIZE*player->currentEditCursorSize;
    src.w = ORIGINAL_TILE_SIZE * player->currentEditCursorSize;
    src.h = ORIGINAL_TILE_SIZE * player->currentEditCursorSize;

    SDL_RenderCopy(renderer, tileset, &src, &highlightRect);
}
