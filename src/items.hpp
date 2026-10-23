#pragma once

#define STONE_ID 689
#define WOOD_ID 753
#define AXE_ID 2922
#define HOE_ID 3114
#define WATERCAN_ID 2858
#define FENCE_BASE_ID 233
#define FENCE_VERTICAL_ID 232
#define FENCE_HORIZONTAL_ID 169
#define FENCE_CORNER_RIGHT_UP_ID 105
#define FENCE_CORNER_LEFT_UP_ID 106
#define FENCE_CORNER_RIGHT_DOWN_ID 102
#define FENCE_CORNER_LEFT_DOWN_ID 103
#define FENCE_UP 296
#define FENCE_DOWN 104
#define FENCE_LEFT 170
#define FENCE_RIGHT 166
#define FENCE_CENTRE 168


#define CARROT_SEED 755
#define CARROT_STAGE_1 819


enum ITEM_TYPE
{
    SEED,
    TOOL,
    STRUCTURE
};

enum ITEM
{
    ITEM_WATERCAN,
    ITEM_AXE,
    ITEM_HOE,
    ITEM_WOOD,
    ITEM_STONE,
    ITEM_FENCE,
    ITEM_CARROT_SEED
};

struct Item
{
    ITEM_TYPE itemType;
    ITEM name;
    int count = 0;
    int hp = 0;
    int tileSetId = 0;
    SDL_Texture* texture;  
};

struct AppState;

void InitItems(AppState& appState);