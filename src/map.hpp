#pragma once
#include "helper.hpp"
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#define TILE_SIZE 16



void DrawMapGrid(AppState& appState);
int InitMap(Map& map, AppState& appState);
int InitMapLayers(std::string& xml, AppState& appState);
void DrawMap(AppState& appState);