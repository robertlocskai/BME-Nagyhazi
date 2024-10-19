#ifndef MAP_H
#define MAP_H
#include <time.h>
#include "items.h"
#include <stdbool.h>

#define MAP_ROWS 50
#define MAP_COLS 50
#define MAX_PLANTS 50

typedef enum {
    GRASS = 1,
    HARROWED = 0,
    HARROWED_EDGE_TOP_LEFT = 2,
    HARROWED_EDGE_TOP = 3,
    HARROWED_EDGE_TOP_RIGHT = 4,
    HARROWED_EDGE_LEFT = 5,
    HARROWED_EDGE_RIGHT = 6,
    HARROWED_EDGE_BOTTOM_LEFT = 7,
    HARROWED_EDGE_BOTTOM = 8,
    HARROWED_EDGE_BOTTOM_RIGHT = 9,
    HARROWED_EDGE_RIGHT_LEFT = 10,
    HARROWED_EDGE_TOP_BOTTOM = 11,
    HARROWED_EDGE_TOP_LEFT_BOTTOM = 12,
    HARROWED_EDGE_TOP_RIGHT_BOTTOM = 13,
    HARROWED_EDGE_ALL = 14,
    HARROWED_EDGE_LEFT_TOP_RIGHT = 15,
    HARROWED_EDGE_LEFT_BOTTOM_RIGHT = 16,
    PLANKS = 17
} TileType;


const char* TILE_FORMAT_OUT = "(%d; %d)\n";
typedef struct {

    TileType type;
    int state;

} Tile;

const char* PLANT_FORMAT_OUT = "(%d; %d; %d; \"%s\"; %d; %d; %d; %d; %d; %d; %d; %lld; %.2f; %d)\n";
const char* PLANT_FORMAT_IN = "(%d; %d; %d; \"%[^\"]\"; %d; %d; %d; %d; %d; %d; %d; %lld; %lf; %d)\n";
typedef struct {
    int tileX;
    int tileY;
    ItemName name;
    char displayName[MAX_ITEM_NAME_LENGTH];
    int srcX;
    int srcY;
    int srcW;
    int srcH;
    int currentState;
    int states;
    int waterLevel;
    time_t plantTimestamp;
    double growthDuration;
    bool grown;
} Plant;

typedef struct {
    Tile tiles[MAP_ROWS][MAP_COLS];
    Plant* plants[MAP_ROWS][MAP_COLS];
} Map;


SDL_Rect TILE;

bool isWithinBounds(int row, int col);
bool isGrass(Tile tile);
void updateTile(Map *map, int row, int col);
void harrowTiles(Map *map, int tileX, int tileY, int currentEditCursorSize);
void removeHarrowed(Map *map, int tileX, int tileY, int currentEditCursorSize);
bool plant(Map *map, int tileX, int tileY, Item *item);
void getTilesetCoords(TileType type, int *x, int *y);
void freeMap(Map *map);

void initMap(Map *map);
void updateMap(Map *map);
void renderMap(SDL_Renderer *renderer, Map *map, SDL_Texture *tileset, SDL_Texture *cropTileset, SDL_Rect *camera);

#endif
