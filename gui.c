#include "gui.h"
#include "dimensions.h"

void initGUI(Gui *gui, GUI_Type givenType) {
    switch(givenType) {
        case INVENTORY:
            gui->scale = 4;
            gui->srcH = 4 * ORIGINAL_TILE_SIZE;
            gui->srcW = 8 * ORIGINAL_TILE_SIZE;
            gui->srcX = 0;
            gui->srcY = 0;
            gui->x = 16;
            gui->y = 16;
            gui->type = givenType;
            gui->visible = false;
        break;
        case QUICK_INVENTORY:
            gui->scale = 4;
            gui->srcH = ORIGINAL_TILE_SIZE;
            gui->srcW = 8 * ORIGINAL_TILE_SIZE;
            gui->srcX = 0;
            gui->srcY = 4 * ORIGINAL_TILE_SIZE;
            gui->x = SCREEN_WIDTH/2 - gui->srcW*gui->scale/2;
            gui->y = SCREEN_HEIGHT - gui->srcH*gui->scale - 16;
            gui->type = givenType;
            gui->visible = true;
        break;
        case QUICK_INVENTORY_CURSOR:
            gui->scale = 4;
            gui->srcH = ORIGINAL_TILE_SIZE+2;
            gui->srcW = ORIGINAL_TILE_SIZE+2;
            gui->srcX = 9 * ORIGINAL_TILE_SIZE - 1;
            gui->srcY = ORIGINAL_TILE_SIZE - 1;
            gui->x = SCREEN_WIDTH/2 - gui->srcW*gui->scale/2 - (ORIGINAL_TILE_SIZE*gui->scale)*3 - (ORIGINAL_TILE_SIZE*gui->scale)/2;
            gui->y = SCREEN_HEIGHT - gui->srcH*gui->scale - (16 - SCALE);
            gui->type = givenType;
            gui->visible = true;
        break;
        default:

    }
}

void updateGUIPositions(GUIManager *guiManager) {
    guiManager->guis[Index_QUICK_INVENTORY].x = SCREEN_WIDTH / 2 - guiManager->guis[Index_QUICK_INVENTORY].srcW * guiManager->guis[Index_QUICK_INVENTORY].scale / 2;
    guiManager->guis[Index_QUICK_INVENTORY].y = SCREEN_HEIGHT - guiManager->guis[Index_QUICK_INVENTORY].srcH * guiManager->guis[Index_QUICK_INVENTORY].scale - 16;
    guiManager->guis[Index_QUICK_INVENTORY_CURSOR].x = SCREEN_WIDTH/2 - guiManager->guis[Index_QUICK_INVENTORY_CURSOR].srcW*guiManager->guis[Index_QUICK_INVENTORY_CURSOR].scale/2 - (ORIGINAL_TILE_SIZE*guiManager->guis[Index_QUICK_INVENTORY_CURSOR].scale)*3 - (ORIGINAL_TILE_SIZE*guiManager->guis[Index_QUICK_INVENTORY_CURSOR].scale)/2;
    guiManager->guis[Index_QUICK_INVENTORY_CURSOR].y = SCREEN_HEIGHT - guiManager->guis[Index_QUICK_INVENTORY_CURSOR].srcH*guiManager->guis[Index_QUICK_INVENTORY_CURSOR].scale - (16 - SCALE);
}

void initGUIManager(GUIManager *guiManager) {
    Gui inventory;
    initGUI(&inventory, INVENTORY);
    Gui quickInventory;
    initGUI(&quickInventory, QUICK_INVENTORY);
    Gui quickInventoryCursor;
    initGUI(&quickInventoryCursor, QUICK_INVENTORY_CURSOR);

    guiManager->guis[Index_INVENTORY]=inventory;
    guiManager->guis[Index_QUICK_INVENTORY]=quickInventory;
    guiManager->guis[Index_QUICK_INVENTORY_CURSOR]=quickInventoryCursor;

}
void updateGUI(GUIManager *guiManager, int mouseX, int mouseY) {
        //if(guiManager->guis[INVENTORY].visible) {
        //}
}
void drawGUI(SDL_Renderer *renderer, GUIManager *guiManager, SDL_Texture *spriteSheet, SDL_Texture *itemSpriteSheet, Player *player, int mouseX, int mouseY) {
    for(int i = 0; i < sizeof(guiManager->guis)/sizeof(guiManager->guis[0]); i++) {
        if(guiManager->guis[i].visible) {
            SDL_Rect src, dest;

            src.h = guiManager->guis[i].srcH;
            src.w = guiManager->guis[i].srcW;
            src.x = guiManager->guis[i].srcX;
            src.y = guiManager->guis[i].srcY;

            dest.w = guiManager->guis[i].srcW * guiManager->guis[i].scale;
            dest.h = guiManager->guis[i].srcH * guiManager->guis[i].scale;
            dest.x = guiManager->guis[i].x;
            dest.y = guiManager->guis[i].y;


            if(guiManager->guis[i].type == INVENTORY) {
                SDL_RenderCopy(renderer, spriteSheet, &src, &dest);
                for(int i = 0; i < 4; i++) {
                    for(int j = 0; j < 8; j++) {
                        if(mouseX>=player->inv.inventorySlots[i][j].slot.x &&
                           mouseX<=player->inv.inventorySlots[i][j].slot.x + player->inv.inventorySlots[i][j].slot.w &&
                           mouseY>=player->inv.inventorySlots[i][j].slot.y &&
                           mouseY<=player->inv.inventorySlots[i][j].slot.y + player->inv.inventorySlots[i][j].slot.h) {
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
                            SDL_RenderFillRect(renderer, &player->inv.inventorySlots[i][j].slot);
                        }

                        if(player->inv.inventorySlots[i][j].item != NULL) {
                            SDL_Rect itemSrc;
                            itemSrc.x = player->inv.inventorySlots[i][j].item->srcX;
                            itemSrc.y = player->inv.inventorySlots[i][j].item->srcY;
                            itemSrc.w = player->inv.inventorySlots[i][j].item->srcW;
                            itemSrc.h = player->inv.inventorySlots[i][j].item->srcH;

                            SDL_RenderCopy(renderer, itemSpriteSheet, &itemSrc, &player->inv.inventorySlots[i][j].slot);

                        }
                    }
                }
            }
            else if(guiManager->guis[i].type == QUICK_INVENTORY) {
            SDL_RenderCopy(renderer, spriteSheet, &src, &dest);
                for(int i = 0; i < 8; i++) {
                    if(player->inv.inventorySlots[3][i].item != NULL) {
                        SDL_Rect itemSrc;
                        itemSrc.x = player->inv.inventorySlots[3][i].item->srcX;
                        itemSrc.y = player->inv.inventorySlots[3][i].item->srcY;
                        itemSrc.w = player->inv.inventorySlots[3][i].item->srcW;
                        itemSrc.h = player->inv.inventorySlots[3][i].item->srcH;

                        SDL_RenderCopy(renderer, itemSpriteSheet, &itemSrc, &player->inv.quickInventorySlots[i].slot);
                    }
                }
            }
            else if(guiManager->guis[i].type == QUICK_INVENTORY_CURSOR) {
                dest.x = (dest.x*1.00) + player->currentQuickInventorySelection * (guiManager->guis[QUICK_INVENTORY_CURSOR].scale*ORIGINAL_TILE_SIZE);
                SDL_RenderCopy(renderer, spriteSheet, &src, &dest);
            }


        }
    }
}
