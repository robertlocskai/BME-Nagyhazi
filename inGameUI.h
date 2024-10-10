#ifndef INGAMEUI_H
#define INGAMEUI_H
#include "Player.h"

void renderTileHighlight(SDL_Renderer *renderer, int tileX, int tileY, SDL_Texture *spritesheet, SDL_Rect *camera, Player *player);

#endif
