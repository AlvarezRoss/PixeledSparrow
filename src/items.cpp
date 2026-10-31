#include "helper.hpp"
#include "items.hpp"
#include "map.hpp"


void InitItem(AppState& appState, Item& item, ITEM itemName)
{
    switch (itemName)
    {
    case ITEM_AXE:
        appState.gameItems[appState.currentItemIndex].itemType = TOOL;
        appState.gameItems[appState.currentItemIndex].tileSetId = AXE_ID;
        break;
    case ITEM_CARROT_SEED:
        appState.gameItems[appState.currentItemIndex].itemType = SEED;
        appState.gameItems[appState.currentItemIndex].tileSetId = CARROT_SEED;
        break;
    case ITEM_FENCE:
        appState.gameItems[appState.currentItemIndex].itemType = STRUCTURE;
        appState.gameItems[appState.currentItemIndex].tileSetId = FENCE_BASE_ID;
        break;
    case ITEM_HOE:
        appState.gameItems[appState.currentItemIndex].itemType = TOOL;
        appState.gameItems[appState.currentItemIndex].tileSetId = HOE_ID;
        break;
    case ITEM_WATERCAN:
        appState.gameItems[appState.currentItemIndex].itemType = TOOL;
        appState.gameItems[appState.currentItemIndex].tileSetId = WATERCAN_ID;
        break;
    case ITEM_STONE:
        appState.gameItems[appState.currentItemIndex].itemType = MATERIAL;
        appState.gameItems[appState.currentItemIndex].tileSetId = STONE_ID;
        break;
    case ITEM_WOOD:
        appState.gameItems[appState.currentItemIndex].itemType = MATERIAL;
        appState.gameItems[appState.currentItemIndex].tileSetId = WOOD_ID;
        break;
    default:
        return;
    }

    appState.gameItems[appState.currentItemIndex].name = itemName;
    appState.gameItems[appState.currentItemIndex].texture = appState.graphics->mapTexture;
    appState.currentItemIndex ++;
}
// To be called from app init function
void InitGameItems(AppState& appState)
{
    int arrayLen = appState.gameItems.size();
    for(int i = 0; i < arrayLen; i++)
    {
        if (i == GAME_ITEMS) return;
        InitItem(appState,appState.gameItems[i],static_cast<ITEM>(i));
    }
}
// This function is used to set the data from the game items array to an item that will be added to the player's invetory
void SetItemData(Item& item, ITEM itemName, AppState& appState)
{
    item.texture = appState.graphics->mapTexture;
    item.name = itemName;
    for (int i = 0; i < appState.currentItemIndex; ++i)
    {
        if (appState.gameItems[i].name == itemName)
        {
            item.tileSetId = appState.gameItems[i].tileSetId;
            item.itemType = appState.gameItems[i].itemType;
        }
    }
}