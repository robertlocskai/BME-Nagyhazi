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

            //HATSO FAL
            building->colliders[0] = (SDL_Rect){SCALE*5, TILE_SIZE + 4*SCALE, TILE_SIZE*6, TILE_SIZE/3};

            //AJTO BAL
            building->colliders[1] = (SDL_Rect){SCALE*5, 5*TILE_SIZE + 9*SCALE, TILE_SIZE*3-2*SCALE, TILE_SIZE/3};

            //AJTO JOBB
            building->colliders[2] = (SDL_Rect){SCALE*7+TILE_SIZE*4, 5*TILE_SIZE + 9*SCALE, TILE_SIZE*2, TILE_SIZE/3};

            //BAL
            building->colliders[3] = (SDL_Rect){SCALE*2, TILE_SIZE+9*SCALE, TILE_SIZE/4, TILE_SIZE*4};

            //JOBB
            building->colliders[4] = (SDL_Rect){TILE_SIZE*6+4*SCALE, TILE_SIZE+9*SCALE, TILE_SIZE/4, TILE_SIZE*4};

        break;
    }
}
