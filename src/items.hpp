#pragma once

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
    int count;
    int hp;
    int tileSetId;  
};