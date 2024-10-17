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

void setDimensionsOfHotbar(Inventory *inv) {

    for(int i = 0; i < 8; i++) {
        /*SDL_Rect rect;
        rect.w = ORIGINAL_TILE_SIZE * 4;
        rect.h = ORIGINAL_TILE_SIZE * 4;
        rect.y = (SCREEN_HEIGHT-(ORIGINAL_TILE_SIZE*4) - ORIGINAL_TILE_SIZE);
        rect.x = (SCREEN_WIDTH/2 - rect.w/2) - (rect.w/2) - 3 * (ORIGINAL_TILE_SIZE * 4) + i * (ORIGINAL_TILE_SIZE * 4);*/
        //inv->quickInventorySlots[i].slot = rect;
        inv->quickInventorySlots[i].slot.x = (SCREEN_WIDTH/2 - inv->quickInventorySlots[i].slot.w/2) - (inv->quickInventorySlots[i].slot.w/2) - 3 * (ORIGINAL_TILE_SIZE * 4) + i * (ORIGINAL_TILE_SIZE * 4);
        inv->quickInventorySlots[i].slot.y = (SCREEN_HEIGHT-(ORIGINAL_TILE_SIZE*4) - ORIGINAL_TILE_SIZE);
    }

}

Item* initItem(ItemName name) {

    Item* item = (Item*)malloc(sizeof(Item));
    if (item == NULL) {
        return NULL;
    }

    if (name == HOE) {
        item->type = TOOL;
        item->name = HOE;
        strcpy(item->displayName, "Hoe");
        item->srcX = 0;
        item->srcY = 0;
        item->srcW = ORIGINAL_TILE_SIZE;
        item->srcH = ORIGINAL_TILE_SIZE;
        item->qty = 1;
     }

    printf("Created item at address: %p\n", (void*)item);
    return item;

}

bool addItemToInventory(Inventory* inv, Item* item, int row, int col) {
    if (inv == NULL || item == NULL || row < 0 || row >= 4 || col < 0 || col >= 8) {
        return false;
    }

    if (inv->inventorySlots[row][col].item != NULL) {
        return false;
    }

    inv->inventorySlots[row][col].item = item;
    item->invX = col;
    item->invY = row;
    printf("Added to X: %d | Y: %d\n", row, col);
    printf("%s\n", inv->inventorySlots[row][col].item->displayName);

    return true;
}

void removeItemFromInventory(Inventory* inv, int row, int col) {
    if (inv != NULL && row >= 0 && row < 4 && col >= 0 && col < 8) {
        if (inv->inventorySlots[row][col].item != NULL) {
            free(inv->inventorySlots[row][col].item);
            inv->inventorySlots[row][col].item = NULL;
        }
    }
}

void initInventoryWithDefaultItems(Inventory* inv) {
    Item* hoe1 = initItem(HOE);
    if (hoe1 != NULL) {
        if (!addItemToInventory(inv, hoe1, 3, 0)) {
            free(hoe1);
        }
    }

    Item* hoe2 = initItem(HOE);
    if (hoe2 != NULL) {
        if (!addItemToInventory(inv, hoe2, 3, 4)) {
            free(hoe2);
        }
    }
}

void initInventory(Inventory *inv) {
    memset(inv, 0, sizeof(Inventory));

    printf("Inventory state memory clear utan:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            printf("[%d]", inv->inventorySlots[i][j].item == NULL ? 0 : 1);
        }
        printf("\n");
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            int y = i * (ORIGINAL_TILE_SIZE * 4) + 16;
            int x = j * (ORIGINAL_TILE_SIZE * 4) + 16;
            inv->inventorySlots[i][j].slot = (SDL_Rect){x + SCALE, y + SCALE, ORIGINAL_TILE_SIZE * 4 - SCALE * 2, ORIGINAL_TILE_SIZE * 4 - SCALE * 2};
            inv->inventorySlots[i][j].item = NULL;
        }
    }

    for (int i = 0; i < 8; i++) {
        SDL_Rect rect;
        rect.w = ORIGINAL_TILE_SIZE * 4;
        rect.h = ORIGINAL_TILE_SIZE * 4;
        rect.y = (SCREEN_HEIGHT - (ORIGINAL_TILE_SIZE * 4) - ORIGINAL_TILE_SIZE);
        rect.x = (SCREEN_WIDTH / 2 - rect.w / 2) - (rect.w / 2) - 3 * (ORIGINAL_TILE_SIZE * 4) + i * (ORIGINAL_TILE_SIZE * 4);
        inv->quickInventorySlots[i].slot = rect;
        inv->quickInventorySlots[i].item = NULL;
    }

    initInventoryWithDefaultItems(inv);
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
