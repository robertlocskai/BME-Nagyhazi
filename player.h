#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
#include <stdbool.h>

typedef struct {
    SDL_Rect slot;
    int item;

} InventorySlot;

typedef struct {
    InventorySlot inventorySlots[4][8];

} Inventory;

typedef struct {
    int xVelocity;
    int yVelocity;
    int speed;
    SDL_Rect rect;
    bool editMode;
    int currentEditCursorSize;
    Inventory inv;
} Player;

void initInventory(Inventory *inv);
void initPlayer(Player *player);
void updatePlayer(Player *player);
void renderPlayer(SDL_Renderer *renderer, Player *Player, SDL_Rect *camera);

#endif
