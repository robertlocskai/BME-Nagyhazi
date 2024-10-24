#ifndef BUILDING_H
#define BUILDING_H
#define BUILDING_COUNT 1
#include <SDL.h>
#include "dimensions.h"

typedef enum {
    HOUSE
} BuildingName;

typedef struct {
    BuildingName name;

    //TOP LAYER
    SDL_Rect srcTopLayer;
    int offsetX;
    int offsetY;
    //BOTTOM LAYER
    SDL_Rect srcBottomLayer;
    int bottomOffsetX;
    int bottomOffsetY;

    int posX;
    int posY;

    SDL_Rect buildingDest;

    int colliders[5];

} Building;

typedef struct {

    Building buildings[BUILDING_COUNT];

} BuildingManager;

void initBuilding(Building* building, BuildingName name, Map* map);
#endif
