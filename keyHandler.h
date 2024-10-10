#ifndef KEYHANDLER_H
#define KEYHANDLER_H

#include <SDL.h>
#include <stdbool.h>

void listenToKeys(Player *player, SDL_Rect *camera, GUIManager *guiM, Map *map, bool *quit);

#endif
