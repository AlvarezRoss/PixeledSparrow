#pragma once

#define STONE_ID 689
#define WOOD_ID 753
#define AXE_ID 2922
#define HOE_ID 3114
#define WATERCAN_ID 2858


enum ITEM_TYPE
{
    SEED,
    TOOL
};

enum ITEM
{
    ITEM_WATERCAN,
    ITEM_AXE,
    ITEM_HOE,
    ITEM_WOOD,
    ITEM_STONE
};

struct Item
{
    ITEM_TYPE itemType;
    ITEM name;
    int count;
    int hp;
    int tileSetId;
    SDL_Texture* texture;  
};

struct AppState;

void InitItems(AppState& appState);