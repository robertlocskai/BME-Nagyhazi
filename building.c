#include "building.h"
#include "map.h"

void initBuilding(Building* building, BuildingName name, Map* map) {
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

            building->posX = TILE_SIZE * 18;
            building->posY = TILE_SIZE * 18;

            building->buildingDest = (SDL_Rect){building->posX + building->offsetX, building->posY+building->offsetY, building->srcTopLayer.w*SCALE, building->srcTopLayer.h*SCALE};

            //HATSO FAL
            building->colliders[0] = addToList(map->colliders, &(SDL_Rect){building->buildingDest.x+SCALE*5, building->buildingDest.y+TILE_SIZE + 8*SCALE, TILE_SIZE*6, SCALE});

            //AJTO BAL
            building->colliders[1] =  addToList(map->colliders, &(SDL_Rect){building->buildingDest.x+SCALE*5, building->buildingDest.y+5*TILE_SIZE + 9*SCALE, TILE_SIZE*3-5*SCALE, SCALE});

            //AJTO JOBB
            building->colliders[2] = addToList(map->colliders, &(SDL_Rect){building->buildingDest.x+SCALE*9+TILE_SIZE*4, building->buildingDest.y+5*TILE_SIZE + 9*SCALE, TILE_SIZE*2-4*SCALE, SCALE});

            //BAL
            building->colliders[3] = addToList(map->colliders, &(SDL_Rect){building->buildingDest.x+SCALE*5, building->buildingDest.y+TILE_SIZE+9*SCALE, SCALE, TILE_SIZE*4});

            //JOBB
            building->colliders[4] = addToList(map->colliders, &(SDL_Rect){building->buildingDest.x+TILE_SIZE*6+4*SCALE, building->buildingDest.y+TILE_SIZE+9*SCALE, SCALE, TILE_SIZE*4});


        break;
    }
}
