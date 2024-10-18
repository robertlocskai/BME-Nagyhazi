#include "items.h"

Item* initItem(ItemName name) {

    Item* item = (Item*)malloc(sizeof(Item));
    if (item == NULL) {
        return NULL;
    }

     switch(name) {
        case HOE:
            item->type = TOOL;
            item->name = name;
            strcpy(item->displayName, "Hoe");
            item->srcX = 0;
            item->srcY = 0;
            item->srcW = ORIGINAL_TILE_SIZE;
            item->srcH = ORIGINAL_TILE_SIZE;
            item->qty = 1;
        break;
        case BARROT:
            item->type = SEED;
            item->name = name;
            strcpy(item->displayName, "Barrot");
            item->srcX = 0;
            item->srcY = ORIGINAL_TILE_SIZE;
            item->srcW = ORIGINAL_TILE_SIZE;
            item->srcH = ORIGINAL_TILE_SIZE;
            item->qty = 1;
        break;
        case NUMONG:
            item->type = SEED;
            item->name = name;
            strcpy(item->displayName, "Numong");
            item->srcX = ORIGINAL_TILE_SIZE*2;
            item->srcY = ORIGINAL_TILE_SIZE;
            item->srcW = ORIGINAL_TILE_SIZE;
            item->srcH = ORIGINAL_TILE_SIZE;
            item->qty = 1;
        break;
     }

    printf("Created item at address: %p\n", (void*)item);
    return item;

}
