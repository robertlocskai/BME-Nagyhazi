#include "gui.h"
#include <SDL.h>
#include "dimensions.h"
#include <stdbool.h>


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
            gui->srcW = 10 * ORIGINAL_TILE_SIZE;
            gui->srcX = 0;
            gui->srcY = 4 * ORIGINAL_TILE_SIZE;
            gui->x = SCREEN_WIDTH/2 - gui->srcW*gui->scale/2;
            gui->y = SCREEN_HEIGHT - gui->srcH*gui->scale - 16;
            gui->type = givenType;
            gui->visible = true;
        break;
        default:

    }
}

void updateGUIPositions(GUIManager *guiManager) {
    for (int i = 0; i < sizeof(guiManager->guis) / sizeof(guiManager->guis[0]); i++) {
        if (guiManager->guis[i].type == QUICK_INVENTORY) {
            guiManager->guis[i].x = SCREEN_WIDTH / 2 - guiManager->guis[i].srcW * guiManager->guis[i].scale / 2;
            guiManager->guis[i].y = SCREEN_HEIGHT - guiManager->guis[i].srcH * guiManager->guis[i].scale - 16;
        }
    }
}

void initGUIManager(GUIManager *guiManager) {
    Gui inventory;
    initGUI(&inventory, INVENTORY);
    Gui quickInventory;
    initGUI(&quickInventory, QUICK_INVENTORY);

    guiManager->guis[Index_INVENTORY]=inventory;
    guiManager->guis[Index_QUICK_INVENTORY]=quickInventory;

}
void updateGUI(Gui *gui) {

}
void drawGUI(SDL_Renderer *renderer, GUIManager *guiManager, SDL_Texture *spriteSheet) {
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

            SDL_RenderCopy(renderer, spriteSheet, &src, &dest);
        }
    }

}
