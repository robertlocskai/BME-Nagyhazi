#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
#include <stdbool.h>
#include "items.h"


#pragma pack(1)
typedef struct {
    SDL_Rect slot;
    Item* item;

} InventorySlot;
#pragma pack()

typedef struct {
    InventorySlot inventorySlots[4][8];
    InventorySlot quickInventorySlots[8];

} Inventory;


typedef struct {
    int xVelocity;
    int yVelocity;
    int speed;
    SDL_Rect rect;
    bool editMode;
    int currentEditCursorSize;
    int currentQuickInventorySelection;
    Inventory inv;
    Item* cursorHeldItem;
} Player;

Item* initItem(ItemName name);
void initInventory(Inventory *inv);
bool addItemToInventory(Inventory* inv, Item* item, int row, int col);
void removeItemFromInventory(Inventory* inv, int row, int col);
void initInventoryWithDefaultItems(Inventory* inv);
void setDimensionsOfHotbar(Inventory *inv);
void initPlayer(Player *player);
void updatePlayer(Player *player, Map* map);
void renderPlayer(SDL_Renderer *renderer, Player *Player, SDL_Rect *camera);
void freeInventory(Player *player);

#endif
