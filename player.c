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
    /*ItemType type;
    char[MAX_ITEM_NAME_LENGTH] name;
    int srcX;
    int srcY;
    int srcW;
    int srcH;
    int qty;
    int invX;
    int invY;*/

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 8; j++) {
            int y = i*(ORIGINAL_TILE_SIZE*4) + 16;
            int x = j*(ORIGINAL_TILE_SIZE*4) + 16;
            inv->inventorySlots[i][j].slot = (SDL_Rect){x + SCALE, y + SCALE, ORIGINAL_TILE_SIZE*4 - SCALE*2, ORIGINAL_TILE_SIZE*4 - SCALE*2};
            inv->inventorySlots[i][j].item = NULL;
        }
    }


    for(int i = 0; i < 8; i++) {
        SDL_Rect rect;
        rect.w = ORIGINAL_TILE_SIZE * 4;
        rect.h = ORIGINAL_TILE_SIZE * 4;
        rect.y = (SCREEN_HEIGHT-(ORIGINAL_TILE_SIZE*4) - ORIGINAL_TILE_SIZE);
        rect.x = (SCREEN_WIDTH/2 - rect.w/2) - (rect.w/2) - 3 * (ORIGINAL_TILE_SIZE * 4) + i * (ORIGINAL_TILE_SIZE * 4);
        inv->quickInventorySlots[i].slot = rect;
        inv->quickInventorySlots[i].item = NULL;
    }

    //DEFAULT ITEMEK ADDOLÁSA
    inv->inventorySlots[3][0].item = (Item *)malloc(sizeof(Item));
    if (inv->inventorySlots[3][0].item != NULL) {
        inv->inventorySlots[3][0].item->type = TOOL;
        strcpy(inv->inventorySlots[3][0].item->name, "Hoe");
        inv->inventorySlots[3][0].item->srcX = 0;
        inv->inventorySlots[3][0].item->srcY = 0;
        inv->inventorySlots[3][0].item->srcW = ORIGINAL_TILE_SIZE;
        inv->inventorySlots[3][0].item->srcH = ORIGINAL_TILE_SIZE;
        inv->inventorySlots[3][0].item->qty = 1;
        inv->inventorySlots[3][0].item->invX = 3;
        inv->inventorySlots[3][0].item->invY = 0;
    }

    inv->inventorySlots[3][4].item = (Item *)malloc(sizeof(Item));
    if (inv->inventorySlots[3][4].item != NULL) {
        inv->inventorySlots[3][4].item->type = TOOL;
        strcpy(inv->inventorySlots[3][4].item->name, "Hoe");
        inv->inventorySlots[3][4].item->srcX = 0;
        inv->inventorySlots[3][4].item->srcY = 0;
        inv->inventorySlots[3][4].item->srcW = ORIGINAL_TILE_SIZE;
        inv->inventorySlots[3][4].item->srcH = ORIGINAL_TILE_SIZE;
        inv->inventorySlots[3][4].item->qty = 1;
        inv->inventorySlots[3][4].item->invX = 3;
        inv->inventorySlots[3][4].item->invY = 0;
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

void freeInventory(Player *player) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            if (player->inv.inventorySlots[i][j].item != NULL) {
                free(player->inv.inventorySlots[i][j].item);
                player->inv.inventorySlots[i][j].item = NULL;
            }
        }
    }
}
