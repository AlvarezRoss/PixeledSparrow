#include "helper.hpp"
#include "map.hpp"
#include <cmath>


void DrawMapGrid(AppState& appState)
{
    SDL_SetRenderDrawColor(appState.renderer,100,100,100,0);
    SDL_GetMouseState(&appState.mouseX,&appState.mouseY);
    SDL_FPoint mousePoint = {appState.mouseX,appState.mouseY};
    for (int x = 0; x < MAP_WIDTH; x++)
    {
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            
            float onScreenX;
            float onScreenY;
            WorldToScreen(appState.camera,static_cast<float>(x*TILE_SIZE),static_cast<float>(y*TILE_SIZE),onScreenX,onScreenY);
            SDL_FRect tile = {onScreenX,onScreenY,TILE_SIZE,TILE_SIZE};
        }
    }
    SDL_SetRenderDrawColor(appState.renderer,0,0,0,0);
}

int InitMap(Map& map, AppState& appState)
{
    // We begin by opening the file and beginning a stream
    map.mapTexture = appState.graphics->mapTexture;

    std::ifstream fileStream(map.path);

    if (!fileStream.is_open()) return -1;


    std::stringstream buffer;
    // This copies the contents of the streambuf into the buffer
    buffer << fileStream.rdbuf();
    // converts the stringbuffer into a string
    std::string xmlMap = buffer.str();
    InitMapLayers(xmlMap,appState);

    return 0;


}


int InitMapLayers(std::string& xml, AppState& appState)
{
    size_t startPosition = 0;
    for (int layer = 0; layer < 4; layer++)
    {
        // Used to find <data.....>
        size_t dataPos = xml.find("<data",startPosition);
        size_t closing = xml.find('>',dataPos);
        // The first value of the map is right after the closing bracket
        startPosition = ++closing;
        size_t endPosition = xml.find('<',startPosition);
        --endPosition;

        std::string layerValues = xml.substr(startPosition,endPosition-startPosition);

        std::stringstream stream(layerValues);
        std::string value;
        int y = 0;
        int x = 0;
        while (std::getline(stream,value,','))
        {
            appState.map.map[layer][y][x] = std::atoi(value.c_str());
            if (layer == WATER_LAYER && appState.map.xRiverIndex == 0 && appState.map.map[layer][y][x] == WATER_FIRST_FRAME + TILED_OFFSET) appState.map.xRiverIndex = x; // finds where the river in the map begins
            ++x;
            if (x == MAP_WIDTH)
            {
                x = 0;
                ++y;
            }
        }        
    }
    return 0;
}

void DrawMap(AppState& appState)
{
    for (int layer = 0; layer < 4; layer ++)
    {
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            for (int x = 0; x < MAP_WIDTH ; x++)
            {
                int tile = appState.map.map[layer][y][x];
                if (tile == 0) continue;
                tile -= TILED_OFFSET;
                float tileX = tile % appState.map.numberOfColumns;
                float tileY = tile / appState.map.numberOfColumns;
                
                const SDL_FRect src = {
                    tileX * TILE_SIZE,
                    tileY * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
                };
                // Transforms position to work position
                float worldX = static_cast<float>(x * TILE_SIZE);
                float worldY = static_cast<float>(y * TILE_SIZE);
                float drawX = 0.0f;
                float drawY = 0.0f;
                WorldToScreen(appState.camera,worldX,worldY,drawX,drawY);
                const SDL_FRect dest = {
                    drawX,
                    drawY,
                    TILE_SIZE,
                    TILE_SIZE
                };

                SDL_RenderTexture(appState.renderer,appState.map.mapTexture,&src,&dest);
            }
        }
    }
}

void AnimateWater(AppState& appState)
{
    const int riverWidth = 4;
    const int animationWait = 60;
    appState.map.frameCounter++;
    if (appState.map.frameCounter < animationWait/appState.map.waterAnimationSpeed) return;
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = appState.map.xRiverIndex ; x < appState.map.xRiverIndex + riverWidth; x++)
        {
            if (appState.map.map[WATER_LAYER][y][x] == WATER_STILL + TILED_OFFSET || appState.map.map[WATER_LAYER][y][x] >= WATER_FOURTH_FRAME + TILED_OFFSET)
            {
                appState.map.map[WATER_LAYER][y][x] = WATER_FIRST_FRAME + TILED_OFFSET;
                continue;
            }
            appState.map.map[WATER_LAYER][y][x]++;
        }
    }
    appState.map.frameCounter = 0;    
}

void UpdateTileIndex(AppState& appState, Entity& player)
{
    player.tilemapIndex.x = std::floor((player.hitBox.x + player.hitBox.w * 0.5f) / TILE_SIZE);
    player.tilemapIndex.y = std::floor((player.hitBox.y + player.hitBox.h * 0.5f) / TILE_SIZE);
 
}

void RenderItemInMap(AppState& appState, Entity& entity, Item& item)
{
    SDL_SetTextureAlphaMod(item.texture,128);
    SDL_FRect src = {
        static_cast<float>(item.tileSetId % appState.map.numberOfColumns * TILE_SIZE),
        static_cast<float>(item.tileSetId / appState.map.numberOfColumns * TILE_SIZE),
        TILE_SIZE,
        TILE_SIZE
    };
    
    Vector2 destPosition{
        static_cast<float>(entity.tilemapIndex.x * TILE_SIZE),
        static_cast<float>(entity.tilemapIndex.y * TILE_SIZE)
    };
    switch (entity.playerDirection)
    {
    case FRONT:
        destPosition.y += TILE_SIZE;
        break;  
    case BACK:
        destPosition.y -= TILE_SIZE;
        break;
    case RIGHT:
        destPosition.x += TILE_SIZE;
        break;
    case LEFT:
        destPosition.x -= TILE_SIZE;
        break;
    default:
        break;
    }
    ApplyTempColor(appState.map,item,entity);
    float drawX;
    float drawY;
    WorldToScreen(appState.camera,destPosition.x,destPosition.y,drawX,drawY);
    SDL_FRect dest{
        drawX,
        drawY,
        TILE_SIZE,
        TILE_SIZE
    };
    SDL_RenderTexture(appState.renderer,item.texture,&src,&dest);
    // Resets texture alpha and color after rendering it
    SDL_SetTextureAlphaMod(item.texture,255);
    SDL_SetTextureColorMod(item.texture,255,255,255);
}


void PlaceItem (Map& map, Entity& player, Inventory& inventory)
{
    if (inventory.itemInUse == nullptr) return;

    switch (inventory.itemInUse->itemType)
    {
    case STRUCTURE:
        PlaceStructure(*inventory.itemInUse,map,player);
        break;
    
    default:
        break;
    }
}

void PlaceStructure(Item& item, Map& map, Entity& player)
{
    GetPlacementTarget(map,player);
    if (IsTileOccupied(map,player.placementTarget.x,player.placementTarget.y,BUILDING_LAYER)) return;
    map.map[BUILDING_LAYER][static_cast<int>(player.placementTarget.y)][static_cast<int>(player.placementTarget.x)] = item.tileSetId + TILED_OFFSET;
    if (item.name == ITEM_FENCE) HandleFencePlacement(map,item,player.placementTarget.x,player.placementTarget.y);
    
}

void ApplyTempColor(Map& map, Item& item, Entity& player)
{
    if (IsTileOccupied(map,player.placementTarget.x,player.placementTarget.y,BUILDING_LAYER)) SDL_SetTextureColorMod(item.texture,34,139,34);
    else SDL_SetTextureColorMod(item.texture,34,139,34);
}

void HandleFencePlacement(Map& map, Item& item, int x, int y)
{
    const int north = 2;
    const int south = 4;
    const int east = 7;
    const int west = 8;
    int tally = 0;
    if (map.map[BUILDING_LAYER][y-1][x] != 0 && IsFence(map,x,y-1)) tally += north;
    if (map.map[BUILDING_LAYER][y+1][x] != 0 && IsFence(map,x,y+1)) tally += south;
    if (map.map[BUILDING_LAYER][y][x+1] != 0 && IsFence(map,x+1,y)) tally += west;
    if (map.map[BUILDING_LAYER][y][x-1] != 0 && IsFence(map,x-1,y)) tally += east;
    switch (tally)
    {
    case 21 : 
        if (map.map[BUILDING_LAYER][y][x] == FENCE_CENTRE + TILED_OFFSET) return; 
        map.map[BUILDING_LAYER][y][x] = FENCE_CENTRE + TILED_OFFSET;
        HandleFencePlacement(map,item,x+1,y);
        HandleFencePlacement(map,item,x-1,y);
        HandleFencePlacement(map,item,x,y+1);
        HandleFencePlacement(map,item,x,y-1);
        break;
    case 2:
        if (map.map[BUILDING_LAYER][y][x] == FENCE_UP + TILED_OFFSET) return;
        map.map[BUILDING_LAYER][y][x] = FENCE_UP + TILED_OFFSET;
        HandleFencePlacement(map,item,x,y-1);
        break;
    case 4:
        if (map.map[BUILDING_LAYER][y][x] == FENCE_DOWN + TILED_OFFSET) return;
        map.map[BUILDING_LAYER][y][x] = FENCE_DOWN + TILED_OFFSET;
        HandleFencePlacement(map,item,x,y+1);
        break;
    case 7:
        if (map.map[BUILDING_LAYER][y][x] == FENCE_LEFT + TILED_OFFSET) return;
        map.map[BUILDING_LAYER][y][x] = FENCE_LEFT + TILED_OFFSET;
        HandleFencePlacement(map,item,x-1,y);
        break;
    case 8:
        if (map.map[BUILDING_LAYER][y][x] == FENCE_RIGHT + TILED_OFFSET) return;
        map.map[BUILDING_LAYER][y][x] = FENCE_RIGHT + TILED_OFFSET;
        HandleFencePlacement(map,item,x+1,y);
        break;
    case 6:
        if (map.map[BUILDING_LAYER][y][x] == FENCE_VERTICAL_ID + TILED_OFFSET) return;
        map.map[BUILDING_LAYER][y][x] = FENCE_VERTICAL_ID + TILED_OFFSET;
        HandleFencePlacement(map,item,x,y+1);
        HandleFencePlacement(map,item,x,y-1);
        break;
    case 9:
        if (map.map[BUILDING_LAYER][y][x] == FENCE_CORNER_RIGHT_DOWN_ID + TILED_OFFSET) return;
        map.map[BUILDING_LAYER][y][x] = FENCE_CORNER_RIGHT_DOWN_ID + TILED_OFFSET;
        HandleFencePlacement(map,item,x,y-1);
        HandleFencePlacement(map,item,x-1,y);
        break;
    case 10:
        if (map.map[BUILDING_LAYER][y][x] == FENCE_CORNER_LEFT_DOWN_ID + TILED_OFFSET) return;
        map.map[BUILDING_LAYER][y][x] = FENCE_CORNER_LEFT_DOWN_ID + TILED_OFFSET;
        HandleFencePlacement(map,item,x,y-1);
        HandleFencePlacement(map,item,x+1,y);
        break;
    case 11:
        if (map.map[BUILDING_LAYER][y][x] == FENCE_CORNER_RIGHT_UP_ID + TILED_OFFSET) return;
        map.map[BUILDING_LAYER][y][x] = FENCE_CORNER_RIGHT_UP_ID + TILED_OFFSET;
        HandleFencePlacement(map,item,x-1,y);
        HandleFencePlacement(map,item,x,y+1);
        break;
    case 12:
        if (map.map[BUILDING_LAYER][y][x] == FENCE_CORNER_LEFT_UP_ID + TILED_OFFSET) return;
        map.map[BUILDING_LAYER][y][x] = FENCE_CORNER_LEFT_UP_ID + TILED_OFFSET;
        HandleFencePlacement(map,item,x,y+1);
        HandleFencePlacement(map,item,x+1,y);
        break;
    case 15:
        if (map.map[BUILDING_LAYER][y][x] == FENCE_HORIZONTAL_ID + TILED_OFFSET) return;
        map.map[BUILDING_LAYER][y][x] = FENCE_HORIZONTAL_ID + TILED_OFFSET;
        HandleFencePlacement(map,item,x+1,y);
        HandleFencePlacement(map,item,x-1,y);
        break;
    default:
        break;
    }
       
}

bool IsFence(Map& map, int x, int y)
{
    int tile = map.map[BUILDING_LAYER][y][x];

    switch (tile - TILED_OFFSET)
    {
    case FENCE_BASE_ID:
    case FENCE_CENTRE:
    case FENCE_CORNER_LEFT_DOWN_ID:
    case FENCE_CORNER_LEFT_UP_ID:
    case FENCE_CORNER_RIGHT_DOWN_ID:
    case FENCE_CORNER_RIGHT_UP_ID:
    case FENCE_HORIZONTAL_ID:
    case FENCE_DOWN:
    case FENCE_LEFT:
    case FENCE_UP:
    case FENCE_RIGHT:
    case FENCE_VERTICAL_ID:
    case 0:
        return true;
        break;
    
    default:
        return false;
        break;
    }

    return false;
}

bool IsTileOccupied(Map& map, int x, int y, int layer)
{
    if (layer < 0 || layer > MAP_LAYERS) return true;
    if (x < 0 || x > MAP_WIDTH) return true;
    if (y < 0 || y > MAP_HEIGHT) return true;

    if (map.map[layer][y][x] != 0) return true;
    return false;
}

void GetPlacementTarget(Map& map, Entity& player)
{
   switch (player.playerDirection)
    {
    case FRONT:
        if (player.tilemapIndex.y + 1 > MAP_HEIGHT) player.placementTarget = {player.tilemapIndex.x,player.tilemapIndex.y};
        else player.placementTarget = {player.tilemapIndex.x,player.tilemapIndex.y + 1};
        break;
    case BACK:
        if (player.tilemapIndex.y - 1 < 0) player.placementTarget = {player.tilemapIndex.x,player.tilemapIndex.y};
        else player.placementTarget = {player.tilemapIndex.x,player.tilemapIndex.y - 1};
        break;
    case LEFT:
        if (player.tilemapIndex.x - 1 < 0) player.placementTarget = {player.tilemapIndex.x,player.tilemapIndex.y};
        else player.placementTarget = {player.tilemapIndex.x - 1,player.tilemapIndex.y};
        break;
    case RIGHT:
        if (player.tilemapIndex.x + 1 > MAP_WIDTH) player.placementTarget = {player.tilemapIndex.x,player.tilemapIndex.y};
        else player.placementTarget = {player.tilemapIndex.x + 1,player.tilemapIndex.y};
        break;
    default:
        player.placementTarget = {player.tilemapIndex.x,player.tilemapIndex.y};
        break;
    }
}
