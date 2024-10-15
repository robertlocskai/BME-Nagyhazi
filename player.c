#include <SDL.h>
#include "dimensions.h"
#include "player.h"
#include <stdbool.h>

void initPlayer(Player *player) {
    player->speed = 5;
    player->xVelocity = 0;
    player->yVelocity = 0;
    player->rect.x = TILE_SIZE * 50 / 2;
    player->rect.y =TILE_SIZE * 50 / 2;
    player->rect.w = TILE_SIZE;
    player->rect.h = TILE_SIZE;
    player->editMode = false;
    player->currentEditCursorSize = 1;
    player->currentQuickInventorySelection = 0;
    initInventory(&player->inv);
}

void initInventory(Inventory *inv) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 8; j++) {
            int y = i*(ORIGINAL_TILE_SIZE*4) + 16;
            int x = j*(ORIGINAL_TILE_SIZE*4) + 16;
            inv->inventorySlots[i][j].slot = (SDL_Rect){x + SCALE, y + SCALE, ORIGINAL_TILE_SIZE*4 - SCALE*2, ORIGINAL_TILE_SIZE*4 - SCALE*2};
            inv->inventorySlots[i][j].item = 0;
        }
    }
}

void updatePlayer(Player *player) {
    player->rect.x += player->xVelocity;
    player->rect.y += player->yVelocity;
}

void renderPlayer(SDL_Renderer *renderer, Player *player, SDL_Rect *camera) {

    SDL_Rect renderRect = player->rect;
    renderRect.x -= camera->x;
    renderRect.y -= camera->y;


    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &renderRect);

}
