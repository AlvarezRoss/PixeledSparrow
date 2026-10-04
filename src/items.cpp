#include "helper.hpp"
#include "items.hpp"


void InitItems(AppState& appState)
{
    for (int i = 0 ; i < GAME_ITEMS ; i++)
    {
        appState.gameItems[i].count = 0;
        appState.gameItems[i].hp = 0;
        appState.gameItems[i].name = static_cast<ITEM>(i);
        appState.gameItems[i].itemType = static_cast<ITEM_TYPE>(i);
        appState.gameItems[i].texture = appState.graphics->mapTexture;
        // Giving the player two item types just for testing
        switch (appState.gameItems[i].name)
        {
        case ITEM_STONE:
            appState.gameItems[i].tileSetId = STONE_ID;
            break;
        case ITEM_AXE:
            appState.gameItems[i].tileSetId = AXE_ID;
            appState.inventory.items[0] = appState.gameItems[i];
            appState.inventory.inventorySlots[0].item = &appState.inventory.items[0];
            break;
        case ITEM_HOE:
            appState.gameItems[i].tileSetId = HOE_ID;
            appState.inventory.items[2] = appState.gameItems[i];
            appState.inventory.inventorySlots[2].item = &appState.inventory.items[2];
            break;
        case ITEM_WOOD:
            appState.gameItems[i].tileSetId = WOOD_ID;
            appState.inventory.items[1] = appState.gameItems[i];
            appState.inventory.items[1].count = 5; 
            appState.inventory.inventorySlots[1].item = &appState.inventory.items[1];
            break;
        case ITEM_WATERCAN:
            appState.gameItems[i].tileSetId = WATERCAN_ID;
            appState.inventory.items[3] = appState.gameItems[i];
            appState.inventory.inventorySlots[3].item = &appState.inventory.items[3];
            break;
        default:
            break;
        }
    }
    

}
