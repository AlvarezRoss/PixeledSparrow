#pragma once
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include "helper.hpp"

#define MAP_LAYERS 5
#define MAP_WIDTH 100
#define MAP_HEIGHT 100
#define TILE_MAP_WIDTH 1024
#define TILE_MAP_HEIGHT 1024
#define TILE_SIZE 16
#define WORLD_WIDTH (MAP_WIDTH*TILE_SIZE)
#define WORLD_HEIGHT (MAP_HEIGHT*TILE_SIZE)
#define TILED_OFFSET 1
#define MAP_PIXEL_COUNT 1600


#define TILE_SIZE 16
#define GROUND_LAYER 0
#define WATER_LAYER 1
#define BUILDING_LAYER 2
#define DECORATION_LAYER 3


#define WATER_FIRST_FRAME 542
#define WATER_SECOND_FRAME 543
#define WATER_THIRD_FRAME 544
#define WATER_FOURTH_FRAME 545
#define WATER_STILL 470

#define STONE_TILE 961
#define FARM_SOIL 203
#define FARM_SOIL_WATERED 267

#define SMALL_TREE_BASE_TILE 372
#define SMALL_TREE_STUMP_TILE 352

enum MAP_OBJECT_TYPE
{
    TREE_SMALL,
    TREE_LARGE,
    HEDGE,
    ROCK,
    OBJECT_TOTAL,
    OBJECT_NONE
};

struct MapObject
{
    MAP_OBJECT_TYPE type = OBJECT_NONE;
    Vector2 mapIndex = {0.0f,0.0f};
    int Hp = 0;
};

struct Map
{
    int map[MAP_LAYERS][MAP_HEIGHT][MAP_WIDTH];
    std::array<MapObject,20> mapObjects;
    int objectArrayCount = 0;
    const std::string path = "assets/Tiled/PixeledSparrow.tmx";
    const std::string pngPath = "assets/spr_tileset_sunnysideworld_16px.png";
    int numberOfColumns = TILE_MAP_WIDTH/TILE_SIZE;
    SDL_Texture *mapTexture = nullptr;
    int xRiverIndex = 0; // used to find where the big river in the map starts
    int frameCounter = 0; // Used to control water animation
    int waterAnimationSpeed = 10; // Used to control water animation speed change
};

struct AppState;


void DrawMapGrid(AppState& appState);
int InitMap(Map& map, AppState& appState);
int InitMapLayers(std::string& xml, AppState& appState);
void DrawMap(AppState& appState);
void AnimateWater(AppState& appState);
void UpdateTileIndex(AppState& appState, Entity& player);
void SetTileId(Inventory& inventory, Map& map, Entity& player);
void RenderItemInMap(AppState& appState, Entity& entity, Item& item);
void PlaceStructure(Item& item, Map& map, Entity& player);
void PlaceSeed(Item& item, Map& map, Entity& player);
void PlaceItem (Map& map, Entity& player, Inventory& inventory);
void ApplyTempColor(Map& map, Item& item, Entity& player);
void HandleFencePlacement(Map& map, Item& item, int x, int y);
bool IsFence(Map& map, int x, int y);
bool IsTileOccupied(Map& map, int x, int y, int layer);
void GetPlacementTarget(Map& map, Entity& player);
void UpdateTile(Map& map, Entity& player);
bool IsFarmTile(Map& map, int x, int y);
void SetMapObject(Map& map, int x, int y, int layer);
void InitMapObjects(Map& map);
void HandleChop(Map& map, Entity& player);