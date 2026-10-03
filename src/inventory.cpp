
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
        if (inventory.itemSelectorSlots[count].item == nullptr) continue;
        RenderInventoryItems(appState,inventory.itemSelectorSlots[count].dest,inventory.itemSelectorSlots[count].item);
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
            
            if (inventory.inventorySlots[slotCounter].item != nullptr)
                RenderInventoryItems(appState,inventory.inventorySlots[slotCounter].dest,inventory.inventorySlots[slotCounter].item);
            ++slotCounter;
        }
    }
}

void UpdateInventoryState(AppState& appState, SDL_Event& event)
{
    
    if (event.key.repeat) return;
    const bool* keyStates = SDL_GetKeyboardState(nullptr);
    if (keyStates == nullptr) return; 
    if (keyStates[SDL_SCANCODE_I]) appState.inventory.open = !appState.inventory.open;
    
    
}

void ProcessInventory(AppState& appState, Inventory& inventory, SDL_Event& event)
{
    if (event.key.repeat) return;
    if (appState.mouseButton != SDL_BUTTON_LMASK) return;

    const int inventorySize = inventory.inventorySlots.size();
    const SDL_FPoint mousePoint = {appState.mouseX,appState.mouseY}; 
    for (int i = 0; i < inventorySize ; i++)
    {
        if (SDL_PointInRectFloat(&mousePoint,&inventory.inventorySlots[i].dest))
        {
            HandleSelection(inventory,inventory.inventorySlots[i]);
        }
    }
}

void HandleSelection(Inventory& inventory, InventorySlot& slot)
{
    SDL_Log("Called");
    if (inventory.selectedItem == nullptr && slot.item == nullptr) return;
    if (inventory.selectedItem == nullptr)
    {
        inventory.selectedItem = slot.item;
        inventory.srcSlot = &slot;
        return;
    }
    if (slot.item != nullptr) return;
    inventory.srcSlot->item = nullptr;
    slot.item = inventory.selectedItem;
    inventory.selectedItem = nullptr;
}

void RenderInventoryItems(AppState& appState, SDL_FRect& drawRectangle, Item* item)
{
    if(item == nullptr) return;
    float tileX = item->tileSetId % appState.map.numberOfColumns;
    float tileY = item->tileSetId / appState.map.numberOfColumns;
    SDL_FRect src = {tileX * TILE_SIZE,tileY* TILE_SIZE,TILE_SIZE,TILE_SIZE};
    
    if (!SDL_RenderTexture(appState.renderer,item->texture,&src,&drawRectangle))
    {
        SDL_Log("Could not render item texture: %s \n",SDL_GetError());
    }
    return;
}
