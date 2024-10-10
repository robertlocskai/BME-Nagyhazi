#include "keyHandler.h"
#include "player.h";
#include <SDL.h>
#include "gui.c"

SDL_Cursor *cursorShovel;

bool ePressed, iPressed, fPressed = false;
int offsetX;
int offsetY;
int tileX;
int tileY;
SDL_Event e;

void listenToKeys(Player *player, SDL_Rect *camera, GUIManager *guiM, Map *map, bool *quit) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    //PLAYER MOZGÁS
    player->xVelocity = 0;
    player->yVelocity = 0;

    if (state[SDL_SCANCODE_D]) {
        player->xVelocity = player->speed;
    }
    if (state[SDL_SCANCODE_A]) {
        player->xVelocity = -player->speed;
    }
    if (state[SDL_SCANCODE_W]) {
        player->yVelocity = -player->speed;
    }
    if (state[SDL_SCANCODE_S]) {
        player->yVelocity = player->speed;
    }

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            *quit = true;
        }
        else if (e.type == SDL_WINDOWEVENT) {
            if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                SCREEN_WIDTH = e.window.data1;
                SCREEN_HEIGHT = e.window.data2;
                camera->h = SCREEN_HEIGHT;
                camera->w = SCREEN_WIDTH;
                updateGUIPositions(&guiM);
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && player->editMode) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            offsetX = mouseX + camera->x;
            offsetY = mouseY + camera->y;

            tileX = offsetX / TILE_SIZE;
            tileY = offsetY / TILE_SIZE;

            if (e.button.button == SDL_BUTTON_LEFT) {
                harrowTiles(&map, tileX, tileY, player->currentEditCursorSize);
            }
            if (e.button.button == SDL_BUTTON_RIGHT) {
                removeHarrowed(&map, tileX, tileY, player->currentEditCursorSize);
            }
        }

        if(e.type == SDL_KEYDOWN) {
            if(e.key.keysym.sym == SDLK_e && !player->editMode && !ePressed && !guiM->guis[Index_INVENTORY].visible) {
                printf("Edit mode on.\n");
                ePressed = true;
                SDL_SetCursor(cursorShovel);
                player->editMode = true;
            }
            else if(e.key.keysym.sym == SDLK_e && player->editMode && !ePressed) {
                printf("Edit mode off.\n");
                ePressed = true;
                SDL_SetCursor(SDL_GetDefaultCursor());
                player->editMode = false;
            }
            if(e.key.keysym.sym == SDLK_i && !guiM->guis[Index_INVENTORY].visible && !iPressed) {
                printf("Inventory open.\n");
                iPressed = true;
                player->editMode = false;
                SDL_SetCursor(SDL_GetDefaultCursor());
                guiM->guis[Index_INVENTORY].visible = true;
            }
            else if(e.key.keysym.sym == SDLK_i && guiM->guis[Index_INVENTORY].visible && !iPressed) {
                printf("Inventory closed.\n");
                iPressed = true;
                guiM->guis[Index_INVENTORY].visible  = false;
            }
            if(e.key.keysym.sym == SDLK_f && player->editMode && !fPressed) {
                switch(player->currentEditCursorSize) {
                    case 1:
                        player->currentEditCursorSize = 2;
                    break;
                    case 2:
                        player->currentEditCursorSize = 3;
                    break;
                    case 3:
                        player->currentEditCursorSize = 1;
                    break;
                    default:
                        player->currentEditCursorSize = 1;
                }
                fPressed = true;
            }
        }
        if(e.type == SDL_KEYUP) {
            if(e.key.keysym.sym == SDLK_e) {
                ePressed = false;
            }
            if(e.key.keysym.sym == SDLK_i) {
                iPressed = false;
            }
            if(e.key.keysym.sym == SDLK_f) {
                fPressed = false;
            }
        }
    }

}
