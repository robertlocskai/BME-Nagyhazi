#include "building.h"

void initBuilding(Building* building, BuildingName name) {
    switch(name) {
        case HOUSE:
            building->name = name;
            building->srcTopLayer = (SDL_Rect){0, 0, ORIGINAL_TILE_SIZE*6+10, ORIGINAL_TILE_SIZE*5+11};
            building->offsetX = -(5*SCALE);
            building->offsetY = -(9*SCALE);
            //BOTTOM LAYER
            building->srcBottomLayer = (SDL_Rect){7*ORIGINAL_TILE_SIZE, 0, ORIGINAL_TILE_SIZE+4, 2};
            building->bottomOffsetX = ORIGINAL_TILE_SIZE*3*SCALE + 3*SCALE;
            building->bottomOffsetY = ORIGINAL_TILE_SIZE*5*SCALE + 8*SCALE;
        break;
    }
}
