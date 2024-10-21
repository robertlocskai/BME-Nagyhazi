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

    SDL_Rect colliders[5];

} Building;

typedef struct {

    Building buildings[BUILDING_COUNT];

} BuildingManager;

void initBuilding(Building* building, BuildingName name);
#endif
