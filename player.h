#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
#include <stdbool.h>

#define MAX_ITEM_NAME_LENGTH 16


typedef enum {
    TOOL,
    SEED
} ItemType;

typedef struct {
    ItemType type;
    char name[MAX_ITEM_NAME_LENGTH];
    int srcX;
    int srcY;
    int srcW;
    int srcH;
    int qty;
    int invX;
    int invY;

} Item;

typedef struct {
    SDL_Rect slot;
    Item* item;

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
    int currentQuickInventorySelection;
    Inventory inv;
} Player;

void initInventory(Inventory *inv);
void initPlayer(Player *player);
void updatePlayer(Player *player);
void renderPlayer(SDL_Renderer *renderer, Player *Player, SDL_Rect *camera);
void freeInventory(Player *player);

#endif
