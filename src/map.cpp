#include "map.hpp"


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
            if (appState.mouseButton != 0 && SDL_PointInRectFloat(&mousePoint,&tile))
            {
                SDL_Log("Clicked on tile x:%d,y:%d\n",x,y);
            }
        }
    }
    SDL_SetRenderDrawColor(appState.renderer,0,0,0,0);
}

int InitMap(Map& map, AppState& appState)
{
    // We begin by opening the file and beginning a stream
    map.mapTexture = IMG_LoadTexture(appState.renderer,map.pngPath.c_str());
    if (map.mapTexture == nullptr) return -1;

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
    for (int layer = 0; layer < 2; layer++)
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
    for (int layer = 0; layer < 2; layer ++)
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