#ifndef GUI_H
#define GUI_H
#include <SDL.h>
#include <stdbool.h>

typedef enum {
    INVENTORY,
    QUICK_INVENTORY
} GUI_Type;

typedef enum {
    Index_INVENTORY = 0,
    Index_QUICK_INVENTORY = 1,
    Index_SIZE = 2
} GUI_Index;

typedef struct {
    GUI_Type type;
    int srcX;
    int srcY;
    int srcW;
    int srcH;
    int x;
    int y;
    int scale;
    bool visible;
} Gui;


typedef struct {

    Gui guis[Index_SIZE];

} GUIManager;


void initGUI(Gui *gui, GUI_Type type);
void updateGUIPositions(GUIManager *guiManager);
void updateGUI(Gui *gui);
void drawGUI(SDL_Renderer *renderer, GUIManager *guiManager, SDL_Texture *spriteSheet);
#endif
