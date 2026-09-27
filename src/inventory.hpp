#pragma once

#include <SDL3/SDL.h>
#include "items.hpp"

#define ITEM_SLOT_X 2
#define ITEM_SLOT_Y 93
#define ITEM_SLOT_WIDTH 44
#define ITEM_SLOT_HEIGHT 52
#define INVENTORY_TOP_TO_BUTTONS 30
#define INVENTORY_OFFSET 20
#define INVENTORY_X 137
#define INVENTORY_Y 12
#define INVENTORY_WIDTH 110
#define INVENTORY_COLUMNS 5
#define INVENTORY_ROWS 5
#define INVENTORY_SCREEN_OFFSET 300
#define SELECTED_BUTTON_OFFSET 7



enum ItemSlotState
{
    ITEM_SLOT_SELECTED,
    ITEM_SLOT_UNSELECTED,
    ITEM_SLOT_EMPTY
};

struct InventorySlot
{
    Item *item = nullptr;
    float worldX;
    float worldY;
    float drawX;
    float drawY;
    float indexX;
    float indexY;
    SDL_FRect dest;
};

struct Inventory
{
    std::array<InventorySlot,25> inventorySlots = {};
    std::array<InventorySlot,7> itemSelectorSlots = {};
    std::array<Item,20> items = {};
    SDL_Texture *inventoryTexture = nullptr;
    SDL_Texture *itemSelectionTexture = nullptr;
    SDL_Texture * itemSlotTexture = nullptr;
    bool open = false;
    int selectedItemIndex = 0; // Used in the selector bar
};

// Forward declaration

struct Entity;
struct Graphics;
struct AppState;

void InitInventory(AppState& appState, Graphics& graphics);
void RenderItemSelector(AppState& appState);
void RenderItemSlots(AppState& appState, Inventory& inventory);
void RenderInventoryWindow(AppState& appState, Inventory& inventory);
void UpdateInventoryState(AppState& appState);
void HandleItemSelection(AppState& appState, Inventory& inventory);