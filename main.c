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
#include "building.c"

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
    //initMap(&map);
    loadMap(&map, "saves/debugmap/");

    Player player;
    initPlayer(&player);
    SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    //INVENTORY
    GUIManager guiM;
    initGUIManager(&guiM);

    //BUILDINGS
    BuildingManager buildingM;
    //INITTING BUILDING
    Building house;
    initBuilding(&house, HOUSE, &map);
    //ADD TO BUILDINGS
    buildingM.buildings[0] = house;


    if(!init()) {
        printf("Couldn't init the game!");
    }
    else {

        SDL_Texture* tileset = loadTexture("assets/images/tiles.png", renderer);
        SDL_Texture* cropTileset = loadTexture("assets/images/crops.png", renderer);
        SDL_Texture* uiGrids = loadTexture("assets/images/gui/ingame_grid.png", renderer);
        SDL_Texture* gui = loadTexture("assets/images/gui/gui.png", renderer);
        SDL_Texture* items = loadTexture("assets/images/gui/items.png", renderer);
        SDL_Texture* house = loadTexture("assets/images/house.png", renderer);

        if (tileset == NULL) {
            return -1;
        }

        bool quit = false;
        SDL_Event e;

        Uint32 frameStart;
        int frameTime;

        bool ePressed, tabPressed, fPressed = false;

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
                saveMap(&map, "saves/debugmap/");
                freeInventory(&player);
                freeMap(&map);
                quit = 1;
            }
            else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SCREEN_WIDTH = e.window.data1;
                    SCREEN_HEIGHT = e.window.data2;
                    camera.h = SCREEN_HEIGHT;
                    camera.w = SCREEN_WIDTH;
                    updateGUIPositions(&guiM);
                    setDimensionsOfHotbar(&player.inv);
                }
            }

            if (e.type == SDL_MOUSEWHEEL) {
                if (e.wheel.y > 0) {
                    player.currentQuickInventorySelection = player.currentQuickInventorySelection > 0 ? player.currentQuickInventorySelection-1 : 7;
                    player.currentEditCursorSize = 1;
                } else if (e.wheel.y < 0) {
                    player.currentQuickInventorySelection = player.currentQuickInventorySelection < 7 ? player.currentQuickInventorySelection+1 : 0;
                    player.currentEditCursorSize = 1;
                }
            }

            if(e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_1:
                        player.currentEditCursorSize = 1;
                        player.currentQuickInventorySelection = 0;
                    break;
                    case SDLK_2:
                        player.currentEditCursorSize = 1;
                        player.currentQuickInventorySelection = 1;
                    break;
                    case SDLK_3:
                        player.currentEditCursorSize = 1;
                        player.currentQuickInventorySelection = 2;
                    break;
                    case SDLK_4:
                        player.currentEditCursorSize = 1;
                        player.currentQuickInventorySelection = 3;
                    break;
                    case SDLK_5:
                        player.currentEditCursorSize = 1;
                        player.currentQuickInventorySelection = 4;
                    break;
                    case SDLK_6:
                        player.currentEditCursorSize = 1;
                        player.currentQuickInventorySelection = 5;
                    break;
                    case SDLK_7:
                        player.currentEditCursorSize = 1;
                        player.currentQuickInventorySelection = 6;
                    break;
                    case SDLK_8:
                        player.currentEditCursorSize = 1;
                        player.currentQuickInventorySelection = 7;
                    break;
                    default:
                        //itt nem kéne semmit elvileg
                }
                if(e.key.keysym.sym == SDLK_e && !player.editMode && !ePressed && !guiM.guis[Index_INVENTORY].visible) {
                    printf("Edit mode on.\n");
                    ePressed = true;
                    player.editMode = true;
                }
                else if(e.key.keysym.sym == SDLK_e && player.editMode && !ePressed) {
                    printf("Edit mode off.\n");
                    ePressed = true;
                    player.editMode = false;
                    player.currentEditCursorSize = 1;
                }
                if(e.key.keysym.sym == SDLK_TAB && !guiM.guis[Index_INVENTORY].visible && !tabPressed) {
                    printf("Inventory open.\n");
                    tabPressed = true;
                    player.editMode = false;
                    player.currentEditCursorSize = 1;
                    guiM.guis[Index_INVENTORY].visible = true;
                }
                else if(e.key.keysym.sym == SDLK_TAB && guiM.guis[Index_INVENTORY].visible && !tabPressed) {
                    printf("Inventory closed.\n");
                    tabPressed = true;
                    guiM.guis[Index_INVENTORY].visible  = false;
                    if(player.cursorHeldItem) {
                        for(int i = 0; i < 4; i++) {
                            for(int j = 0; j < 8; j++) {
                                if(player.inv.inventorySlots[i][j].item==NULL) {
                                    player.inv.inventorySlots[i][j].item = player.cursorHeldItem;
                                    player.cursorHeldItem = NULL;
                                    break;
                                }
                            }
                        }
                    }
                }
                if(e.key.keysym.sym == SDLK_f && player.editMode && !fPressed) {
                        if(player.inv.inventorySlots[3][player.currentQuickInventorySelection].item != NULL) {
                            switch(player.inv.inventorySlots[3][player.currentQuickInventorySelection].item->type) {
                                case TOOL:
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
                                break;
                            }
                        }
                        else {
                            printf("Nincs a kezedben item.");
                        }
                    fPressed = true;
                }
            }
            if(e.type == SDL_KEYUP) {
                if(e.key.keysym.sym == SDLK_e) {
                    ePressed = false;
                }
                if(e.key.keysym.sym == SDLK_TAB) {
                    tabPressed = false;
                }
                if(e.key.keysym.sym == SDLK_f) {
                    fPressed = false;
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && player.editMode) {
                //NEM NULL AZ ITEM POINTER KULONBEN KICRASHEL A GECIBE
                if(player.inv.inventorySlots[3][player.currentQuickInventorySelection].item != NULL) {
                        int mouseX, mouseY;
                        SDL_GetMouseState(&mouseX, &mouseY);

                        offsetX = mouseX + camera.x;
                        offsetY = mouseY + camera.y;

                        tileX = offsetX / TILE_SIZE;
                        tileY = offsetY / TILE_SIZE;
                        switch(player.inv.inventorySlots[3][player.currentQuickInventorySelection].item->type) {
                            case TOOL:
                                if(player.inv.inventorySlots[3][player.currentQuickInventorySelection].item->name == HOE) {
                                    if (e.button.button == SDL_BUTTON_LEFT) {
                                        harrowTiles(&map, tileX, tileY, player.currentEditCursorSize);
                                    }
                                    if (e.button.button == SDL_BUTTON_RIGHT) {
                                        removeHarrowed(&map, tileX, tileY, player.currentEditCursorSize);
                                    }
                                }
                            break;
                            case SEED:
                                if (e.button.button == SDL_BUTTON_LEFT) {
                                    if(player.inv.inventorySlots[3][player.currentQuickInventorySelection].item->name == NUMONG) {
                                       printf("Trying to plant Numong...");
                                    }
                                    if(plant(&map, tileX, tileY, player.inv.inventorySlots[3][player.currentQuickInventorySelection].item)) {
                                        removeItemFromInventory(&player.inv, 3, player.currentQuickInventorySelection);
                                    }
                                }
                            break;
                        }
                }
                else {
                    printf("Nincs item a kezedben.\n");
                            placePlank(&map, tileX, tileY);
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
        updatePlayer(&player, &map);
        updateCamera(&camera, &player);
        updateGUI(&guiM, mouseX, mouseY);


        //RENDER
        //LAYER 0 MAP
        renderMap(renderer, &map, tileset, cropTileset, &camera);
        //LAYER 1 INGAME GUI
        if(player.editMode) {
            offsetX = mouseX + camera.x;
            offsetY = mouseY + camera.y;

            tileX = offsetX / TILE_SIZE;
            tileY = offsetY / TILE_SIZE;
            renderTileHighlight(renderer, tileX, tileY, uiGrids, &camera, &player);
        }


        //RENDER HOUSE BOTTOM LAYER
        for(int i = 0; i < BUILDING_COUNT; i++) {
            SDL_Rect buildingDest;
            buildingDest.w = buildingM.buildings[i].srcBottomLayer.w*SCALE;
            buildingDest.h = buildingM.buildings[i].srcBottomLayer.h*SCALE;
            buildingDest.x = (TILE_SIZE * 18 + buildingM.buildings[i].offsetX) + buildingM.buildings[i].bottomOffsetX - camera.x;
            buildingDest.y = (TILE_SIZE * 18 + buildingM.buildings[i].offsetY) + buildingM.buildings[i].bottomOffsetY - camera.y;
            SDL_SetTextureAlphaMod(house, 255);
            SDL_RenderCopy(renderer, house, &buildingM.buildings[i].srcBottomLayer, &buildingDest);
        }


        //RENDER HOUSE TOP LAYER
        for(int i = 0; i < BUILDING_COUNT; i++) {
            SDL_Rect buildingDest = buildingM.buildings[i].buildingDest;
            buildingDest.x = buildingM.buildings[i].buildingDest.x - camera.x;
            buildingDest.y = buildingM.buildings[i].buildingDest.y - camera.y;

            if(buildingDest.y+buildingDest.h > player.rect.y + player.rect.h - camera.y &&
                buildingDest.y < player.rect.y - camera.y &&
                buildingDest.x+5*SCALE <player.rect.x - camera.x &&
                buildingDest.x+buildingDest.w-5*SCALE > player.rect.x + player.rect.w - camera.x
               ) {
                SDL_SetTextureAlphaMod(house, 70);
                //LAYER PLAYER
                renderPlayer(renderer, &player, &camera);
                SDL_RenderCopy(renderer, house, &buildingM.buildings[i].srcTopLayer, &buildingDest);
            }
            else if(buildingDest.y+buildingDest.h > player.rect.y + player.rect.h - camera.y &&
                buildingDest.y < player.rect.y - camera.y
               ) {
                //LAYER PLAYER
                renderPlayer(renderer, &player, &camera);
                SDL_SetTextureAlphaMod(house, 255);
                SDL_RenderCopy(renderer, house, &buildingM.buildings[i].srcTopLayer, &buildingDest);
            }
            else {
                SDL_SetTextureAlphaMod(house, 255);
                SDL_RenderCopy(renderer, house, &buildingM.buildings[i].srcTopLayer, &buildingDest);
                //LAYER PLAYER
                renderPlayer(renderer, &player, &camera);
            }

            for(int j = 0; j < map.colliders->size; j++) {
                SDL_RenderDrawRect(renderer, &(SDL_Rect){((SDL_Rect*)get(map.colliders, j))->x - camera.x, ((SDL_Rect*)get(map.colliders, j))->y - camera.y, ((SDL_Rect*)get(map.colliders, j))->w, ((SDL_Rect*)get(map.colliders, j))->h});
            }
        }

        //TOP LAYER GUI
        drawGUI(renderer, &guiM, gui, items, &player, mouseX, mouseY);

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
        //CLEANUP
        SDL_DestroyTexture(tileset);
        SDL_DestroyTexture(cropTileset);
        SDL_DestroyTexture(uiGrids);
        SDL_DestroyTexture(gui);
        SDL_DestroyTexture(items);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    return 0;

}
