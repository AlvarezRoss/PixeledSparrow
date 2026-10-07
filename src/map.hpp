#pragma once
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
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


struct AppState;
struct Map;

void DrawMapGrid(AppState& appState);
int InitMap(Map& map, AppState& appState);
int InitMapLayers(std::string& xml, AppState& appState);
void DrawMap(AppState& appState);
void AnimateWater(AppState& appState);