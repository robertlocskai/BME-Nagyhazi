#include "map.h"
#include <stdlib.h>
#include <SDL.h>
#include "dimensions.h"
#include <SDL_image.h>


void initMap(Map *map) {
    printf("Initting map");
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            map->tiles[i][j].type = GRASS;
            map->tiles[i][j].state = 0;
            map->plants[i][j] = NULL;
        }
    }
}

void saveMap(Map *map, const char* folderPath) {


    for(int i = 0; i < MAP_ROWS; i++) {
        for(int j = 0; j < MAP_COLS; j++) {
            if(map->plants[i][j] != NULL) {
                printf("[%d]", map->plants[i][j]->name);
            }
            else {
                printf("[-]");
            }
        }
        printf("\n");
    }

    char fullPath[100];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", folderPath, "tiles.dat");

    FILE* tiles;
    if(fopen_s(&tiles, fullPath, "w+") != 0 || tiles == NULL) {
        printf("Error opening tiles on save.\n");
        return;
    }


    snprintf(fullPath, sizeof(fullPath), "%s/%s", folderPath, "plants.dat");
    FILE* plants;
    if(fopen_s(&plants, fullPath, "w+") != 0 || plants == NULL) {
        printf("Error opening plants on save.\n");
        return;
    }

    for(int i = 0; i < MAP_ROWS; i++) {
        for(int j = 0; j < MAP_COLS; j++) {
            Tile tile = map->tiles[i][j];
            Plant* plant = map->plants[i][j];
            //Print tiles
            printf("Saving tile [%d][%d]: type=%d, state=%d\n", i, j, tile.type, tile.state);
            fprintf_s(tiles, TILE_FORMAT_OUT, tile.type, tile.state);
            //Print plants
            if (plant != NULL) {
                fprintf_s(plants, PLANT_FORMAT_OUT,
                    plant->tileX,
                    plant->tileY,
                    plant->name,
                    plant->displayName,
                    plant->srcX,
                    plant->srcY,
                    plant->srcW,
                    plant->srcH,
                    plant->currentState,
                    plant->states,
                    plant->waterLevel,
                    (long long)plant->plantTimestamp,
                    plant->growthDuration,
                    plant->grown);
                    printf("\n-------------------------Saved a plant.-------------------------\n");
            } else {
                //Marker for no plant
                fprintf_s(plants, "(%d; %d; -1; \"\"; 0; 0; 0; 0; 0; 0; 0; 0; 0; 0)\n", i, j);
            }

        }
    }

    fseek(tiles, 0, SEEK_SET);
    fseek(plants, 0, SEEK_SET);
    fclose(tiles);
    fclose(plants);
}

void loadMap(Map* map, const char* folderPath) {
    initMap(map);

    char fullPath[100];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", folderPath, "tiles.dat");

    FILE* tiles;
    if(fopen_s(&tiles, fullPath, "r") != 0 || tiles == NULL) {
        printf("Error opening tiles.\n");
        return;
    }

    snprintf(fullPath, sizeof(fullPath), "%s/%s", folderPath, "plants.dat");

    FILE* plants;
    if(fopen_s(&plants, fullPath, "r") != 0 || plants == NULL) {
        printf("Error opening plants.\n");
        return;
    }


    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            Tile* tile = &map->tiles[i][j];
            //READING TILES
            if (fscanf_s(tiles, TILE_FORMAT_OUT, &tile->type, &tile->state) != 2) {
                printf("Error reading tile at [%d][%d].\n", i, j);
                fclose(plants);
                fclose(tiles);
                return;
            }

            //READING PLANTS
            Plant* plant = (Plant*)malloc(sizeof(Plant));
            if (plant == NULL) {
                printf("Memory allocation failed for plant at [%d][%d].\n", i, j);
                fclose(tiles);
                fclose(plants);
                return;
            }

            int result = fscanf_s(plants, PLANT_FORMAT_IN,
                &plant->tileX,
                &plant->tileY,
                &plant->name,
                plant->displayName, (unsigned)sizeof(plant->displayName),
                &plant->srcX,
                &plant->srcY,
                &plant->srcW,
                &plant->srcH,
                &plant->currentState,
                &plant->states,
                &plant->waterLevel,
                (long long*)&plant->plantTimestamp,
                &plant->growthDuration,
                (int*)&plant->grown);

            printf("\nResult: %d\n", result);

            if (result == 14) {
                    if(plant->name != -1) {
                        map->plants[i][j] = plant;
                    }
                    else {
                        free(plant);
                        map->plants[i][j] = NULL;
                    }
            }
            else {
                free(plant);
                map->plants[i][j] = NULL;
                fscanf_s(plants, "%*[^\n]\n");
            }
        }
    }
    fclose(tiles);
    fclose(plants);
}

SDL_Rect TILE = { .h = TILE_SIZE, .w = TILE_SIZE, .x = 0, .y = 0 };

bool isWithinBounds(int row, int col) {
    return row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS;
}

bool isGrass(Tile tile) {
    return tile.type == GRASS;
}

void updateTile(Map *map, int row, int col) {
    if (!isWithinBounds(row, col) || isGrass(map->tiles[row][col])) return;

    bool topGrass = isWithinBounds(row-1, col) && isGrass(map->tiles[row-1][col]);
    bool bottomGrass = isWithinBounds(row+1, col) && isGrass(map->tiles[row+1][col]);
    bool leftGrass = isWithinBounds(row, col-1) && isGrass(map->tiles[row][col-1]);
    bool rightGrass = isWithinBounds(row, col+1) && isGrass(map->tiles[row][col+1]);

    if(topGrass && leftGrass && bottomGrass && rightGrass) map->tiles[row][col].type = HARROWED_EDGE_ALL;
    else if(topGrass && leftGrass && bottomGrass) map->tiles[row][col].type = HARROWED_EDGE_TOP_LEFT_BOTTOM;
    else if(topGrass && rightGrass && bottomGrass) map->tiles[row][col].type = HARROWED_EDGE_TOP_RIGHT_BOTTOM;
    else if(topGrass && leftGrass && rightGrass) map->tiles[row][col].type = HARROWED_EDGE_LEFT_TOP_RIGHT;
    else if(leftGrass && rightGrass && bottomGrass) map->tiles[row][col].type = HARROWED_EDGE_LEFT_BOTTOM_RIGHT;
    else if (topGrass && leftGrass) map->tiles[row][col].type = HARROWED_EDGE_TOP_LEFT;
    else if (topGrass && rightGrass) map->tiles[row][col].type = HARROWED_EDGE_TOP_RIGHT;
    else if (bottomGrass && leftGrass) map->tiles[row][col].type = HARROWED_EDGE_BOTTOM_LEFT;
    else if (bottomGrass && rightGrass) map->tiles[row][col].type = HARROWED_EDGE_BOTTOM_RIGHT;
    else if (topGrass && bottomGrass) map->tiles[row][col].type = HARROWED_EDGE_TOP_BOTTOM;
    else if (leftGrass && rightGrass) map->tiles[row][col].type = HARROWED_EDGE_RIGHT_LEFT;
    else if (topGrass) map->tiles[row][col].type = HARROWED_EDGE_TOP;
    else if (bottomGrass) map->tiles[row][col].type = HARROWED_EDGE_BOTTOM;
    else if (leftGrass) map->tiles[row][col].type = HARROWED_EDGE_LEFT;
    else if (rightGrass) map->tiles[row][col].type = HARROWED_EDGE_RIGHT;
    else map->tiles[row][col].type = HARROWED;
}

void harrowTiles(Map *map, int tileX, int tileY, int currentEditCursorSize) {
    if(currentEditCursorSize==3) {
        for (int i = tileY - 1; i <= tileY + 1; i++) {
            for (int j = tileX - 1; j <= tileX + 1; j++) {
                if (isWithinBounds(i, j)) {
                    map->tiles[i][j].type = HARROWED;
                }
            }
        }

        for (int i = tileY - 2; i <= tileY + 2; i++) {
            for (int j = tileX - 2; j <= tileX + 2; j++) {
                updateTile(map, i, j);
            }
        }
    }
    else if(currentEditCursorSize==2) {
        for(int i = tileY; i <= tileY + 1; i++) {
            for(int j = tileX; j <= tileX+1; j++) {
                if(isWithinBounds(i, j)) {
                    map->tiles[i][j].type = HARROWED;
                }
            }
        }
        for(int i = tileY-1; i <= tileY + 2; i++) {
            for(int j = tileX-1; j <= tileX + 2; j++) {
                updateTile(map, i, j);
            }
        }
    }
    else {
        if (isWithinBounds(tileY, tileX)) {
            map->tiles[tileY][tileX].type = HARROWED;
        }
        for (int i = tileY - 1; i <= tileY + 1; i++) {
            for (int j = tileX - 1; j <= tileX + 1; j++) {
                updateTile(map, i, j);
            }
        }
    }
}

void removeHarrowed(Map *map, int tileX, int tileY, int currentEditCursorSize) {
    if(currentEditCursorSize==3) {
        for (int i = tileY - 1; i <= tileY + 1; i++) {
            for (int j = tileX - 1; j <= tileX + 1; j++) {
                if (isWithinBounds(i, j)) {
                    map->tiles[i][j].type = GRASS;
                }
            }
        }

        for (int i = tileY - 2; i <= tileY + 2; i++) {
            for (int j = tileX - 2; j <= tileX + 2; j++) {
                updateTile(map, i, j);
            }
        }
    }
    else if(currentEditCursorSize==2) {
        for(int i = tileY; i <= tileY + 1; i++) {
            for(int j = tileX; j <= tileX+1; j++) {
                if(isWithinBounds(i, j)) {
                    map->tiles[i][j].type = GRASS;
                }
            }
        }
        for(int i = tileY-1; i <= tileY + 2; i++) {
            for(int j = tileX-1; j <= tileX + 2; j++) {
                updateTile(map, i, j);
            }
        }
    }
    else {
        if (isWithinBounds(tileY, tileX)) {
            map->tiles[tileY][tileX].type = GRASS;
        }
        for (int i = tileY - 1; i <= tileY + 1; i++) {
            for (int j = tileX - 1; j <= tileX + 1; j++) {
                updateTile(map, i, j);
            }
        }
    }
}

Plant* initPlant(ItemName name, int tileX, int tileY) {
    Plant* newPlant = (Plant *)malloc(sizeof(Plant));
    if (newPlant == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    newPlant->srcW = ORIGINAL_TILE_SIZE;
    newPlant->srcH = ORIGINAL_TILE_SIZE;
    newPlant->tileX = tileX;
    newPlant->tileY = tileY;
    newPlant->waterLevel = 100;
    newPlant->currentState = 1;
    newPlant->states = 5;
    newPlant->plantTimestamp = time(NULL);
    newPlant->grown = false;

    switch(name) {
        case BARROT:
            newPlant->name = name;
            strcpy(newPlant->displayName, "Barrot");
            newPlant->srcX = 0;
            newPlant->srcY = 0;
            newPlant->growthDuration = 120;
        break;
        case NUMONG:
            newPlant->name = name;
            strcpy(newPlant->displayName, "Numong");
            newPlant->srcX = 0;
            newPlant->srcY = ORIGINAL_TILE_SIZE*2;
            newPlant->growthDuration = 60;
        break;
    }


    return newPlant;
}


bool plant(Map *map, int tileX, int tileY, Item *item) {
    if(map->tiles[tileY][tileX].type==HARROWED ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_ALL ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_BOTTOM ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_BOTTOM_LEFT ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_BOTTOM_RIGHT ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_LEFT ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_LEFT_BOTTOM_RIGHT ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_LEFT_TOP_RIGHT ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_RIGHT ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_RIGHT_LEFT ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_TOP ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_TOP_BOTTOM ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_TOP_LEFT ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_TOP_LEFT_BOTTOM ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_TOP_RIGHT ||
       map->tiles[tileY][tileX].type==HARROWED_EDGE_TOP_RIGHT_BOTTOM
       ) {
            map->plants[tileY][tileX] = initPlant(item->name, tileX, tileY);
            return true;
       }
       else {
            printf("Csak felszantott foldre ultethetsz more.");
            return false;
       }
    return false;
}

void getTilesetCoords(TileType type, int *x, int *y) {
    *x = (type % TILESET_COLS) * ORIGINAL_TILE_SIZE;
    *y = (type / TILESET_COLS) * ORIGINAL_TILE_SIZE;
}

void freeMap(Map *map) {
    for(int i = 0; i < MAP_COLS; i++) {
        for(int j = 0; j < MAP_ROWS; j++) {
            if(map->plants[i][j] != NULL) {
                free(map->plants[i][j]);
                map->plants[i][j] = NULL;
            }
        }
    }
}

void renderMap(SDL_Renderer *renderer, Map *map, SDL_Texture *tileset, SDL_Texture *cropTileset, SDL_Rect *camera) {
    SDL_Rect src, dest;
    dest.w = TILE_SIZE;
    dest.h = TILE_SIZE;

    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            dest.x = (TILE_SIZE * j) - camera->x;
            dest.y = (TILE_SIZE * i) - camera->y;

            if (dest.x + dest.w > 0 && dest.x < camera->w &&
                dest.y + dest.h > 0 && dest.y < camera->h) {

                getTilesetCoords(map->tiles[i][j].type, &src.x, &src.y);
                src.w = ORIGINAL_TILE_SIZE;
                src.h = ORIGINAL_TILE_SIZE;

                SDL_RenderCopy(renderer, tileset, &src, &dest);

                if(map->plants[i][j] != NULL) {
                    SDL_Rect cropSrc;

                    double time_diff = difftime(time(NULL), map->plants[i][j]->plantTimestamp);
                    if(!map->plants[i][j]->grown) {
                        if(time_diff>=map->plants[i][j]->growthDuration) {
                            map->plants[i][j]->currentState = 5;
                            map->plants[i][j]->grown = true;
                        }
                        else if(time_diff>map->plants[i][j]->growthDuration/map->plants[i][j]->states*map->plants[i][j]->currentState) {
                            map->plants[i][j]->currentState++;
                        }
                    }

                    dest.y -= 10;

                    cropSrc.x=map->plants[i][j]->srcX + (map->plants[i][j]->currentState-1) * ORIGINAL_TILE_SIZE;
                    cropSrc.y=map->plants[i][j]->srcY;
                    cropSrc.w=map->plants[i][j]->srcW;
                    cropSrc.h=map->plants[i][j]->srcH;

                    SDL_RenderCopy(renderer, cropTileset, &cropSrc ,&dest);
                }
            }
        }
    }
}

void placePlank(Map* map, int tileX, int tileY) {
    map->tiles[tileY][tileX].type = PLANKS;
}
