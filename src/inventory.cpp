
#include "helper.hpp"
#include "inventory.hpp"

void InitInventory(AppState& appState, Graphics& graphics)
{
    appState.inventory.itemSelectionTexture = graphics.itemSelector;
    appState.inventory.inventoryTexture = graphics.inventoryUi;
    appState.inventory.itemSlotTexture = graphics.inventorySlot;
    appState.inventory.open = false;
}

void RenderItemSelector(AppState& appState)
{
    const float yDistanceFromPlayer = 2;

    float uiLayerWidth = appState.graphics->itemSelector->w;
    float uiLayerHeight = appState.graphics->itemSelector->h;
    SDL_FRect src = {0,0,uiLayerWidth,uiLayerHeight};

    appState.uiX = appState.entities[appState.playerIndex].x - uiLayerWidth/2;
    appState.uiY = appState.entities[appState.playerIndex].y + uiLayerHeight * yDistanceFromPlayer;

    float drawX;
    float drawY;
    WorldToScreen(appState.camera,appState.uiX,appState.uiY,drawX,drawY);

    SDL_FRect dest = {drawX,drawY,uiLayerWidth,uiLayerHeight};

    if (!SDL_RenderTexture(appState.renderer,appState.graphics->itemSelector,&src,&dest))
    {
        SDL_Log("Cannot render UI Layer: %s",SDL_GetError());
    }
    return;
}

void RenderItemSlots(AppState& appState, Inventory& inventory)
{
    int slotCount = std::size(inventory.itemSelectorSlots);
    for (int count = 0; count < slotCount; count++)
    {
        SDL_FRect src = {ITEM_SLOT_X,ITEM_SLOT_Y,ITEM_SLOT_WIDTH,ITEM_SLOT_HEIGHT};
        if (inventory.selectedItemIndex == count)
        {
            src.x = ITEM_SLOT_X + ITEM_SLOT_WIDTH*2 + SELECTED_BUTTON_OFFSET;
        }

        inventory.itemSelectorSlots[count].worldX = appState.uiX + INVENTORY_OFFSET + ITEM_SLOT_WIDTH * count;
        inventory.itemSelectorSlots[count].worldY = appState.uiY + INVENTORY_TOP_TO_BUTTONS;
        WorldToScreen(appState.camera,
                        inventory.itemSelectorSlots[count].worldX,
                        inventory.itemSelectorSlots[count].worldY,
                        inventory.itemSelectorSlots[count].drawX,
                        inventory.itemSelectorSlots[count].drawY);
        inventory.itemSelectorSlots[count].dest = {inventory.itemSelectorSlots[count].drawX,inventory.itemSelectorSlots[count].drawY,
                                                    ITEM_SLOT_WIDTH,ITEM_SLOT_HEIGHT};

        if(!SDL_RenderTexture(appState.renderer,inventory.itemSlotTexture,&src,&inventory.itemSelectorSlots[count].dest))
        {
            SDL_Log("Cannot render Item Selection Slots number %d : %s",count,SDL_GetError());
        }
    }
    return;
}


void RenderInventoryWindow(AppState& appState, Inventory& inventory)
{
    SDL_FRect src = {ITEM_SLOT_X,ITEM_SLOT_Y,ITEM_SLOT_WIDTH,ITEM_SLOT_HEIGHT};
    int slotCounter = 0;
    float initPosition = appState.entities[appState.playerIndex].x - INVENTORY_SCREEN_OFFSET;
    for (int y = 0; y < INVENTORY_ROWS; y++)
    {
        for (int x = 0; x < INVENTORY_COLUMNS; x++)
        {
            if (slotCounter > 25) return;
            inventory.inventorySlots[slotCounter].worldX = initPosition + ITEM_SLOT_WIDTH * x;
            inventory.inventorySlots[slotCounter].worldY = appState.entities[appState.playerIndex].y - appState.graphics->inventorySlot->h + ITEM_SLOT_HEIGHT * y;
            WorldToScreen(appState.camera,inventory.inventorySlots[slotCounter].worldX,inventory.inventorySlots[slotCounter].worldY,
                        inventory.inventorySlots[slotCounter].drawX,inventory.inventorySlots[slotCounter].drawY);

            inventory.inventorySlots[slotCounter].dest = {inventory.inventorySlots[slotCounter].drawX,inventory.inventorySlots[slotCounter].drawY,
                                                        ITEM_SLOT_WIDTH,ITEM_SLOT_HEIGHT};
            
            
            SDL_RenderTexture(appState.renderer,appState.graphics->inventorySlot,&src,&inventory.inventorySlots[slotCounter].dest);
            ++slotCounter;
        }
    }
}

void UpdateInventoryState(AppState& appState)
{
    const bool* keyStates = SDL_GetKeyboardState(nullptr);

    if (keyStates == nullptr) return;
    if (keyStates[SDL_SCANCODE_I]) appState.inventory.open != appState.inventory.open;
}

void HandleItemSelection(AppState& appState, Inventory& inventory)
{
    const bool* keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_1]) inventory.selectedItemIndex = 0;
    else if (keys[SDL_SCANCODE_2]) inventory.selectedItemIndex = 1;
    else if (keys[SDL_SCANCODE_3]) inventory.selectedItemIndex = 2;
    else if (keys[SDL_SCANCODE_4]) inventory.selectedItemIndex = 3;
    else if (keys[SDL_SCANCODE_5]) inventory.selectedItemIndex = 4;
    else if (keys[SDL_SCANCODE_6]) inventory.selectedItemIndex = 5;
    else if (keys[SDL_SCANCODE_7]) inventory.selectedItemIndex = 6;
}