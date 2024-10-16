#include "map.h"
#include <stdlib.h>
#include <SDL.h>
#include "dimensions.h"
#include <SDL_image.h>


void initMap(Map *map) {
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            map->tiles[i][j].type = GRASS;
            map->tiles[i][j].state = 0;  // Initialize state to 0 or any default value
        }
    }
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

void getTilesetCoords(TileType type, int *x, int *y) {
    *x = (type % TILESET_COLS) * ORIGINAL_TILE_SIZE;
    *y = (type / TILESET_COLS) * ORIGINAL_TILE_SIZE;
}

void renderMap(SDL_Renderer *renderer, Map *map, SDL_Texture *tileset, SDL_Rect *camera) {
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
            }
        }
    }
}
