#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "map.c"
#include "player.c"
#include "dimensions.h"
#include <SDL_image.h>
#include "tiles.c"
#include "inGameUI.c"
#include "gui.c"

const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

const float gravityPull = 0.6;

SDL_Window* window;
SDL_Renderer *renderer;
bool init() {
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    else {
        window = SDL_CreateWindow("Nagy hazi",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            SDL_Quit();
            return false;
        }
        else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL) {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                SDL_DestroyWindow(window);
                SDL_Quit();
                return false;
            }
        }
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }

    return success;
}

void updateCamera(SDL_Rect *camera, Player *player) {

    camera->x = player->rect.x + (player->rect.w / 2) - (camera->w / 2);
    camera->y = player->rect.y + (player->rect.h / 2) - (camera->h / 2);

    if (camera->x < 0) camera->x = 0;
    if (camera->y < 0) camera->y = 0;

    int worldWidth = TILE_SIZE * 50;
    int worldHeight = TILE_SIZE * 50;

    if (camera->x > worldWidth - camera->w) {
        camera->x = worldWidth - camera->w;
    }
    if (camera->y > worldHeight - camera->h) {
        camera->y = worldHeight - camera->h;
    }

}

int main(int argc, char* argv[]) {

    Map map;
    initMap(&map);

    Player player;
    initPlayer(&player);
    SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    //INVENTORY
    GUIManager guiM;
    initGUIManager(&guiM);


    if(!init()) {
        printf("Couldn't init the game!");
    }
    else {

        //EGÉR KURZOR BEÁLLÍTÁS
        SDL_Surface *cursorSurface = IMG_Load("assets/images/gui/cursor.png");
            if (!cursorSurface) {
                printf("Failed to load cursor image: %s\n", IMG_GetError());
            return -1;
        }

        SDL_Cursor *cursorShovel = SDL_CreateColorCursor(cursorSurface, 0, 0);


        SDL_Texture* tileset = loadTexture("assets/images/tiles.png", renderer);
        SDL_Texture* uiGrids = loadTexture("assets/images/gui/ingame_grid.png", renderer);
        SDL_Texture* gui = loadTexture("assets/images/gui/gui.png", renderer);

        if (tileset == NULL) {
            return -1;
        }

        bool quit = false;
        SDL_Event e;

        Uint32 frameStart;
        int frameTime;

        bool ePressed, iPressed, fPressed = false;

        //MOUSE GRID INGAME GUI
        int offsetX;
        int offsetY;
        int tileX;
        int tileY;

        while (!quit) {
        frameStart = SDL_GetTicks();
        const Uint8 *state = SDL_GetKeyboardState(NULL);

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SCREEN_WIDTH = e.window.data1;
                    SCREEN_HEIGHT = e.window.data2;
                    camera.h = SCREEN_HEIGHT;
                    camera.w = SCREEN_WIDTH;
                    updateGUIPositions(&guiM);
                }
            }

            if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.sym == SDLK_e && !player.editMode && !ePressed && !guiM.guis[Index_INVENTORY].visible) {
                    printf("Edit mode on.\n");
                    ePressed = true;
                    SDL_SetCursor(cursorShovel);
                    player.editMode = true;
                }
                else if(e.key.keysym.sym == SDLK_e && player.editMode && !ePressed) {
                    printf("Edit mode off.\n");
                    ePressed = true;
                    SDL_SetCursor(SDL_GetDefaultCursor());
                    player.editMode = false;
                }
                if(e.key.keysym.sym == SDLK_i && !guiM.guis[Index_INVENTORY].visible && !iPressed) {
                    printf("Inventory open.\n");
                    iPressed = true;
                    player.editMode = false;
                    SDL_SetCursor(SDL_GetDefaultCursor());
                    guiM.guis[Index_INVENTORY].visible = true;
                }
                else if(e.key.keysym.sym == SDLK_i && guiM.guis[Index_INVENTORY].visible && !iPressed) {
                    printf("Inventory closed.\n");
                    iPressed = true;
                    guiM.guis[Index_INVENTORY].visible  = false;
                }
                if(e.key.keysym.sym == SDLK_f && player.editMode && !fPressed) {
                    switch(player.currentEditCursorSize) {
                        case 1:
                            player.currentEditCursorSize = 2;
                        break;
                        case 2:
                            player.currentEditCursorSize = 3;
                        break;
                        case 3:
                            player.currentEditCursorSize = 1;
                        break;
                        default:
                            player.currentEditCursorSize = 1;
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

            if (e.type == SDL_MOUSEBUTTONDOWN && player.editMode) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                offsetX = mouseX + camera.x;
                offsetY = mouseY + camera.y;

                tileX = offsetX / TILE_SIZE;
                tileY = offsetY / TILE_SIZE;

                if (e.button.button == SDL_BUTTON_LEFT) {
                    harrowTiles(&map, tileX, tileY, player.currentEditCursorSize);
                }
                if (e.button.button == SDL_BUTTON_RIGHT) {
                    removeHarrowed(&map, tileX, tileY, player.currentEditCursorSize);
                }
            }
        }


        player.xVelocity = 0;
        player.yVelocity = 0;

        if (state[SDL_SCANCODE_D]) {
            player.xVelocity = player.speed;
        }
        if (state[SDL_SCANCODE_A]) {
            player.xVelocity = -player.speed;
        }
        if (state[SDL_SCANCODE_W]) {
            player.yVelocity = -player.speed;
        }
        if (state[SDL_SCANCODE_S]) {
            player.yVelocity = player.speed;
        }

        //CLEAR PANEL
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        //UPDATES
        updatePlayer(&player);
        updateCamera(&camera, &player);
        updateGUI(&guiM, mouseX, mouseY);


        //RENDER
        //LAYER 0 MAP
        renderMap(renderer, &map, tileset, &camera);
        //LAYER 1 INGAME GUI
        if(player.editMode) {
            offsetX = mouseX + camera.x;
            offsetY = mouseY + camera.y;

            tileX = offsetX / TILE_SIZE;
            tileY = offsetY / TILE_SIZE;
            renderTileHighlight(renderer, tileX, tileY, uiGrids, &camera, &player);
        }
        //LAYER PLAYER
        renderPlayer(renderer, &player, &camera);


        //TOP LAYER GUI
        drawGUI(renderer, &guiM, gui, &player, mouseX, mouseY);

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

        //CLEANUP
        SDL_DestroyTexture(tileset);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return 0;

}
