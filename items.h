#ifndef ITEMS_H
#define ITEMS_H

#define MAX_ITEM_NAME_LENGTH 16

typedef enum {
    TOOL,
    SEED
} ItemType;

typedef enum {
    HOE,
    BARROT,
    NUMONG
} ItemName;

typedef struct {
    ItemType type;
    ItemName name;
    char displayName[MAX_ITEM_NAME_LENGTH];
    int srcX;
    int srcY;
    int srcW;
    int srcH;
    int qty;
    int invX;
    int invY;

} Item;


#endif
