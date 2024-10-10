#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
#include <stdbool.h>

typedef struct {
    int xVelocity;
    int yVelocity;
    int speed;
    SDL_Rect rect;
    bool editMode;
    int currentEditCursorSize;
} Player;

void initPlayer(Player *player);
void updatePlayer(Player *player);
void renderPlayer(SDL_Renderer *renderer, Player *Player, SDL_Rect *camera);

#endif
